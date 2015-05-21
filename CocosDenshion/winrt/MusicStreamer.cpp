//  MusicStream.h
//  Family Guy
//
//  Created by August Junkala on 5/18/15.
//  Copyright (c) 2015 TinyCo. All rights reserved.
//

#include "pch.h"
#include "MusicStreamer.h"

// TODO: Surely there is a better way to check this.
extern bool windows_is_low_memory_device;

/* MS loves exceptions. I don't. Helpful macros to catch
   COM errors, invalidate the stream and gives us a chance to clean up cleanly */
#define INVALID_RETURN_FAIL(hr) if(FAILED(hr)) { setState(Invalid); return; }
#define INVALID_BREAK_FAIL(hr) if(FAILED(hr)) { setState(Invalid); break; }

MusicStreamer::MusicStreamer(_In_ const WCHAR* url, IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, float volume) : 
        hBufferEndEvent(CreateEventExW(NULL, FALSE, FALSE, NULL)) {
    // Setup
    ZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
    Windows::Storage::StorageFolder^ installedLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;
    m_installedLocationPath = Platform::String::Concat(installedLocation->Path, "\\Assets\\Resources\\");

    // Initialise file
    initialiseFile(url);
    if(m_state == Initialising)
        initialiseVoice(engine, masteringVoice, volume);

    if(m_state == Initialising) {
        m_processor = std::thread(&MusicStreamer::runStream, this);
    }
}

MusicStreamer::~MusicStreamer() {
    try {
        stopMusic();

        if(m_sourceVoice != nullptr) {
            m_sourceVoice->DestroyVoice();
            m_sourceVoice = nullptr;
        }

        // Purge our queue
        while(m_bufferQueue.empty() == false) {
            delete[] m_bufferQueue.front().pAudioData;
            m_bufferQueue.pop();
        }

        CloseHandle(hBufferEndEvent);

        if(m_state != Constructed)
            MFShutdown();
    } catch(...) {
        // DestroyVoice and CloseHandle can throw exceptions, but we are on our way out.
    }
}

void MusicStreamer::pauseMusic() {
    if(m_state != Live)
        return;

    m_sourceVoice->Stop();
    setState(Paused);
}

void MusicStreamer::resumeMusic() {
    if(m_state != Paused)
        return;

    m_sourceVoice->Start();
    setState(Live);
}

void MusicStreamer::stopMusic() {
    setState(Stopping);
    if(m_processor.joinable())
        m_processor.join();
}

void MusicStreamer::setMusicVolume(float volume) {
    if(!isAlive())
        return;

    HRESULT hr = m_sourceVoice->SetVolume(volume);
    if(FAILED(hr))
        setState(Invalid);
}

/** Void callbacks*/
COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnVoiceProcessingPassStart(UINT32) {
}

COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnVoiceProcessingPassEnd() {

}

COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnStreamEnd() {
    // All buffers processed; can safely exit
    setState(Stopping);
}

COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnBufferStart(void*) {
    ResetEvent(hBufferEndEvent);
}

COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnBufferEnd(void* pContext) {
    {
        // Done this buffer; clean it up.
        std::lock_guard<std::mutex> lock(m_bufferMutex);
        m_bufferQueue.front().AudioBytes = 0;
        delete[] m_bufferQueue.front().pAudioData;
        m_bufferQueue.front().pAudioData = nullptr;

        m_bufferQueue.pop();
    }

    // Wake in case we can process more stream
    m_bufferWait.notify_all();

    //Trigger the event for the music stream.
    if(pContext == 0) {
        SetEvent(hBufferEndEvent);
    }
}

COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnLoopEnd(void*) {
    // Manual looping. Ignore
}

COM_DECLSPEC_NOTHROW void _stdcall MusicStreamer::OnVoiceError(void*, HRESULT) {
    // Issue in the voice. Essentially ignorable since the engine error will also be thrown.
}

/** Internal */
void MusicStreamer::initialiseFile(_In_ const WCHAR* url) {
    Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;
    Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

    INVALID_RETURN_FAIL(MFStartup(MF_VERSION));
    setState(Initialising);

    WCHAR filePath[MAX_PATH] = {0};
    if((wcslen(url) > 1 && url[1] == ':')) {
        // path start with "x:", is absolute path
        wcscat_s(filePath, url);
    } else if(wcslen(url) > 0
        && (L'/' == url[0] || L'\\' == url[0])) {
        // path start with '/' or '\', is absolute path without driver name
        wcscat_s(filePath, m_installedLocationPath->Data());
        // remove '/' or '\\'
        wcscat_s(filePath, (const WCHAR*)url[1]);
    } else {
        wcscat_s(filePath, m_installedLocationPath->Data());
        wcscat_s(filePath, url);
    }

    INVALID_RETURN_FAIL(MFCreateSourceReaderFromURL(filePath, nullptr, &m_reader));

    // Set the decoded output format as PCM
    // XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
    // When using MF, this sample always decodes into PCM.

    INVALID_RETURN_FAIL(MFCreateMediaType(&mediaType));
    INVALID_RETURN_FAIL(mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio));
    INVALID_RETURN_FAIL(mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM));
    INVALID_RETURN_FAIL(m_reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.Get()));

    // Get the complete WAVEFORMAT from the Media Type
    INVALID_RETURN_FAIL(m_reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &outputMediaType));

    if(windows_is_low_memory_device) {
        outputMediaType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1);
        outputMediaType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, 2);
        outputMediaType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, 22050);
        outputMediaType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 22050 * 2);

        INVALID_RETURN_FAIL(m_reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, outputMediaType.Get()));
    }

    uint32 formatSize = 0;
    WAVEFORMATEX* waveFormat;
    INVALID_RETURN_FAIL(MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &formatSize));
    CopyMemory(&m_waveFormat, waveFormat, sizeof(m_waveFormat));
    CoTaskMemFree(waveFormat);

    // Get the total length of the stream in bytes
    PROPVARIANT var;
    INVALID_RETURN_FAIL(m_reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var));
    LONGLONG duration = var.uhVal.QuadPart;
    double durationInSeconds = (duration / static_cast<double>(10000000)); // duration is in 100ns units, convert to seconds
    m_maxStreamLengthInBytes = static_cast<unsigned int>(durationInSeconds * m_waveFormat.nAvgBytesPerSec);

    // Round up the buffer size to the nearest four bytes
    m_maxStreamLengthInBytes = (m_maxStreamLengthInBytes + 3) / 4 * 4;
}

void MusicStreamer::initialiseVoice(IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, float volume) {
    // Setup for sending data
    XAUDIO2_SEND_DESCRIPTOR descriptors[1];
    descriptors[0].pOutputVoice = masteringVoice;
    descriptors[0].Flags = 0;

    XAUDIO2_VOICE_SENDS sends = {0};
    sends.SendCount = 1;
    sends.pSends = descriptors;

    INVALID_RETURN_FAIL(engine->CreateSourceVoice(&m_sourceVoice, &m_waveFormat, 0, 1.0f, this, &sends));

    //fix bug: set a initial volume
    INVALID_RETURN_FAIL(m_sourceVoice->SetVolume(volume));

    m_sampleRate = m_waveFormat.nSamplesPerSec;
}

void MusicStreamer::runStream() {
    INVALID_RETURN_FAIL(m_sourceVoice->Start());

    setState(Live);

    while(isAlive()) {
        std::unique_lock<std::mutex> lock(m_bufferMutex);

        bool buffer_full = false;
        {
            XAUDIO2_VOICE_STATE state;
            m_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);

            buffer_full = (state.BuffersQueued >= m_bufferQueueThreshold);
        }
        if(m_state == Paused || buffer_full) {
            m_bufferWait.wait(lock, [this] {
                if(m_state == Paused)
                    return false; // Paused, keep on waiting

                if(m_state != Live)
                    return true; // No longer live, exit

                // Live, maybe it is time for more data.
                XAUDIO2_VOICE_STATE state;
                m_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);

                return (state.BuffersQueued < m_bufferQueueThreshold);
            });
        }

        if(m_state != Live)
            break; // Been told to exit.

        // Read more data
        addBuffer();
    }

    if(m_sourceVoice != nullptr) {
        INVALID_RETURN_FAIL(m_sourceVoice->Stop());
        INVALID_RETURN_FAIL(m_sourceVoice->FlushSourceBuffers());
    }

    setState(Stopped);
}

void MusicStreamer::addBuffer() {
    // Setup chunk
    XAUDIO2_BUFFER chunk;
    ZeroMemory(&chunk, sizeof(chunk));
    chunk.pContext = this;
    chunk.LoopCount = 0; // This is for looping over the chunk; not general looping.

    readChunk(chunk);

    // Push to voice
    m_bufferQueue.push(chunk);
    INVALID_RETURN_FAIL(m_sourceVoice->SubmitSourceBuffer(&chunk));
}

uint32 MusicStreamer::readChunk(XAUDIO2_BUFFER& chunk) {
    uint8* buffer = new uint8[m_bufferSize];

    // Read some data
    uint32 read = 0;

    while(read < m_bufferSize) {
        Microsoft::WRL::ComPtr<IMFSample> sample;
        Microsoft::WRL::ComPtr<IMFMediaBuffer> mediaBuffer;
        BYTE *audioData = nullptr;
        DWORD sampleBufferLength = 0;
        DWORD flags = 0;

        INVALID_BREAK_FAIL(m_reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample));

        if(sample == nullptr && (flags & MF_SOURCE_READERF_ENDOFSTREAM) == 0) {
            // Something is wrong. Done reading.
            break;
        }

        if(sample != nullptr) {
            // A sample was read, process it
            INVALID_BREAK_FAIL(sample->ConvertToContiguousBuffer(&mediaBuffer));
            INVALID_BREAK_FAIL(mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength));

            // If buffer isn't large enough, dump sample
            if(read + sampleBufferLength) {
                // Seeking can be very expensive. Rather than step back (or skip audio) we'll lengthen our
                // buffer and consume the data now. We should only ever do this one per chunk.
                uint8* temp = new uint8[read + sampleBufferLength];
                memcpy(temp, buffer, read);
                delete[] buffer;
                buffer = temp;
            }

            // Copy the newly read bytes into the buffer
            CopyMemory(buffer + read, audioData, sampleBufferLength);
            read += sampleBufferLength;
        }

        if(flags & MF_SOURCE_READERF_ENDOFSTREAM) {
            if(m_looping) {
                PROPVARIANT position;
                PropVariantInit(&position);
                position.vt = VT_I8;
                position.lVal = 0;

                HRESULT hr = m_reader->SetCurrentPosition(GUID_NULL, position);
                if(FAILED(hr)) {
                    PropVariantClear(&position);
                    setState(Invalid);
                    break;
                }

                continue;
            } else {
                // At the end of the stream. Mark and break
                chunk.Flags = XAUDIO2_END_OF_STREAM;
                break;
            }
        }
    }

    if(m_state == Invalid) {
        delete[] buffer;
        read = 0;
        buffer = nullptr;
    }

    // Assign the bytes we read to our chunk.
    chunk.AudioBytes = read;
    chunk.pAudioData = buffer;

    return read;
}

void MusicStreamer::setState(MusicStreamer::State state) {
    bool wake_processor = state != m_state;
    if(!wake_processor || m_state == Invalid || m_state == Stopped)
        return;

    m_state = state;

    if(wake_processor)
        m_bufferWait.notify_all();
}

bool MusicStreamer::isAlive() const {
    return (m_state == Live || m_state == Paused);
}
