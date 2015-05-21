//  MusicStream.h
//  Family Guy
//
//  Created by August Junkala on 5/18/15.
//  Copyright (c) 2015 TinyCo. All rights reserved.
//

#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>

#pragma once
class MusicStreamer : public IXAudio2VoiceCallback {
public:
    MusicStreamer(_In_ const WCHAR* url, IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, float volume);
    ~MusicStreamer();

    void pauseMusic();
    void resumeMusic();
    void stopMusic();

    void setMusicVolume(float volume);
    
    /** Voice Callbacks */
    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32);
    STDMETHOD_(void, OnVoiceProcessingPassEnd)();
    STDMETHOD_(void, OnStreamEnd)();
    STDMETHOD_(void, OnBufferStart)(void*);
    STDMETHOD_(void, OnBufferEnd)(void* pContext);
    STDMETHOD_(void, OnLoopEnd)(void*);
    STDMETHOD_(void, OnVoiceError)(void*, HRESULT);
private:
    enum State {
        Constructed,
        Initialising,
        Invalid,
        Live,
        Paused,
        Stopping,
        Stopped
    };

    void initialiseFile(_In_ const WCHAR* url);
    void initialiseVoice(IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, float volume);
    void runStream();
    void addBuffer();
    uint32 readChunk(XAUDIO2_BUFFER& chunk);
    void setState(State state);
    bool isAlive() const;

    // File processing
    Platform::String^ m_installedLocationPath;

    Microsoft::WRL::ComPtr<IMFSourceReader> m_reader{nullptr};

    WAVEFORMATEX m_waveFormat;
    uint32 m_maxStreamLengthInBytes;

    // Voice
    IXAudio2SourceVoice* m_sourceVoice{nullptr};
    uint32 m_sampleRate;
    uint32 m_bufferQueueThreshold{5}; // This should not exceed 64 (throws errors on Nokia)
    uint32 m_bufferSize{8192};     // Ideally sized so there is enough date per buffer to keep up in reading. Note loop re-position supposedly takes as much as 60ms
    std::queue<XAUDIO2_BUFFER> m_bufferQueue; // We can't delete the date until it has played.
    bool m_looping{true};

    // Buffer maintenance
    std::thread m_processor;

    std::condition_variable m_bufferWait;
    std::mutex m_bufferMutex;

    std::atomic<State> m_state{Constructed};
    
    // Voice callback
    HANDLE hBufferEndEvent;
};

