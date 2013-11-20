#include "CCDDS.h"
#include "CCGL.h"
#include "platform/CCFileUtils.h"
#include "CCTexture2D.h"
#include <cstring>
using namespace cocos2d;

#define DDS_MAGIC 0x20534444
#define ID_DXT1   0x31545844
#define ID_DXT3   0x33545844
#define ID_DXT5   0x35545844
#define ID_DX10   0x30315844
#define ID_ATC    0x20435441
#define ID_ATCA   0x41435441
#define ID_ATCI   0x49435441


#define DDS_HEADERSIZE             128

#define DDSD_CAPS                  0x00000001
#define DDSD_HEIGHT                0x00000002
#define DDSD_WIDTH                 0x00000004
#define DDSD_PITCH                 0x00000008
#define DDSD_PIXELFORMAT           0x00001000
#define DDSD_MIPMAPCOUNT           0x00020000
#define DDSD_LINEARSIZE            0x00080000
#define DDSD_DEPTH                 0x00800000

#define DDPF_ALPHAPIXELS           0x00000001
#define DDPF_ALPHA                 0x00000002
#define DDPF_FOURCC                0x00000004
#define DDPF_PALETTEINDEXED8       0x00000020
#define DDPF_RGB                   0x00000040
#define DDPF_LUMINANCE             0x00020000

#define DDSCAPS_COMPLEX            0x00000008
#define DDSCAPS_TEXTURE            0x00001000
#define DDSCAPS_MIPMAP             0x00400000

#define DDSCAPS2_CUBEMAP           0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000
#define DDSCAPS2_VOLUME            0x00200000


CCDDS::CCDDS() {
    m_internallyAllocated = false;
    m_textureData = NULL;
    m_originalData = NULL;
    m_compressionType = DDS_COMPRESS_NONE;
    m_glFormat = 0;
    m_blockSize = 16;
    memset(&m_headerInfo, 0, sizeof(m_headerInfo));
}

CCDDS::~CCDDS() {
    // If we had to allocate in this class we must free
    if(m_internallyAllocated) {
        delete [] m_originalData;
    }
}

bool CCDDS::initWithData(void* buffer, unsigned long size) {
    if(!buffer || size < DDS_HEADERSIZE)
        return false;
    
    m_originalData = (unsigned char*)buffer;
    
    // This pointer will be used to move across the data buffer
    unsigned char* dataWalker = (unsigned char*)m_originalData;
    
    // Test to see if this is infact actually a DDS
    int32_t magicNumber = *(int32_t*)dataWalker;
    if(magicNumber != DDS_MAGIC) {
        CCLOG("Data passed to CCDDS was not in DDS format");
        return false;
    }
    
    
    // Move past the magic number
    dataWalker+=sizeof(int32_t);
    m_headerInfo = *(DDSSurfaceDesc2*)dataWalker;
    
    if((m_headerInfo.pixelFormat.flags & DDPF_FOURCC) == 0) {
        CCLOG("Uncompressed format inside DDS file, currently unsupported, pixelFormat[0x%08X]", m_headerInfo.pixelFormat);
        return false;
    }
    
#ifdef CC_DDS_VERBOSE
    logDescription();
#endif
    
    if(m_headerInfo.pixelFormat.fourCC == ID_DXT1) {
        m_compressionType = DDS_COMPRESS_S3;
        m_glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        m_blockSize = 8;
    }
    else if(m_headerInfo.pixelFormat.fourCC == ID_DXT3) {
        m_compressionType = DDS_COMPRESS_S3;
        m_glFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    }
    else if(m_headerInfo.pixelFormat.fourCC == ID_DXT5) {
        m_compressionType = DDS_COMPRESS_S3;
        m_glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    }
#ifdef ANDROID
    else if(m_headerInfo.pixelFormat.fourCC == ID_ATCI) {
        m_compressionType = DDS_COMPRESS_ATI;
        m_glFormat = GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD;
    }
    else if(m_headerInfo.pixelFormat.fourCC == ID_ATCA) {
        m_compressionType = DDS_COMPRESS_ATI;
        m_glFormat = GL_ATC_RGBA_EXPLICIT_ALPHA_AMD;
    }
#endif
    else if(m_headerInfo.pixelFormat.fourCC == ID_ATC) {
        // We don't support this because it lacks an alpha channel
        CCLOG("ATC formatted DDS, currently unsupported");
        return false;
    }
    else if(m_headerInfo.pixelFormat.fourCC == ID_DX10) {
        CCLOG("DX10 formatted DDS, currently unsupported");
        return false;
    }
    else {
        CCLOG("Unknown compressed format inside DDS");
        return false;
    }
    
    dataWalker+=sizeof(DDSSurfaceDesc2);
    m_textureData = dataWalker;
    return true;
}

CCDDS* CCDDS::ddsWithData(void* buffer, unsigned long size) {
    CCDDS* newDDS = new CCDDS();
    if(newDDS->initWithData(buffer, size)) {
        newDDS->autorelease();
        return newDDS;
    }
    delete newDDS;
    return NULL;
}

bool CCDDS::initWithContentsOfFile(const char* path) {
    // We own the original data, so clean it up
    m_internallyAllocated = true;
    unsigned long size = 0;
	m_originalData = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &size);
    return initWithData(m_originalData, size);
}


// We cannot autorelease outside the main thread
CCDDS* CCDDS::ddsWithContentsOfFileAsync(const char* path) {
    
    CCDDS* newDDS = new CCDDS();
    if(newDDS->initWithContentsOfFile(path)) {
        return newDDS;
    }
    delete newDDS;
    return NULL;
}

CCDDS* CCDDS::ddsWithContentsOfFile(const char* path) {
    
    CCDDS* newDDS = new CCDDS();
    if(newDDS->initWithContentsOfFile(path)) {
        newDDS->autorelease();
        return newDDS;
    }
    delete newDDS;
    return NULL;
}


void CCDDS::logDescription(void) {
    
    char strFourCC[5];
    strFourCC[0]=m_headerInfo.pixelFormat.fourCC&255;
    strFourCC[1]=(m_headerInfo.pixelFormat.fourCC>>8)&255;
    strFourCC[2]=(m_headerInfo.pixelFormat.fourCC>>16)&255;
    strFourCC[3]=(m_headerInfo.pixelFormat.fourCC>>24)&255;
    strFourCC[4]=0;
   
    CCLOG("CCDDS: %p", this);
    CCLOG("----------------------");
    CCLOG("DDSSurfaceDesc2.size: %d", m_headerInfo.size);
    CCLOG("DDSSurfaceDesc2.flags: 0x%08X", m_headerInfo.flags);
    CCLOG("DDSSurfaceDesc2.height: %d", m_headerInfo.height);
    CCLOG("DDSSurfaceDesc2.width: %d", m_headerInfo.width);
    CCLOG("DDSSurfaceDesc2.pitchOrLinearSize: %d", m_headerInfo.pitchOrLinearSize);
    CCLOG("DDSSurfaceDesc2.depth: %d", m_headerInfo.depth);
    CCLOG("DDSSurfaceDesc2.mipCount: %d\n", m_headerInfo.mipCount);
    
    CCLOG("DDPixelFormat.size: %d", m_headerInfo.pixelFormat.size);
    CCLOG("DDPixelFormat.flags: 0x%08X", m_headerInfo.pixelFormat.flags);
    CCLOG("DDPixelFormat.fourCC: 0x%08X | \'%s\'", m_headerInfo.pixelFormat.fourCC, strFourCC);
    CCLOG("DDPixelFormat.rgbBitCount: %d", m_headerInfo.pixelFormat.rgbBitCount);
    CCLOG("DDPixelFormat.rBitMask: 0x%08X", m_headerInfo.pixelFormat.rBitMask);
    CCLOG("DDPixelFormat.gBitMask: 0x%08X", m_headerInfo.pixelFormat.gBitMask);
    CCLOG("DDPixelFormat.bBitMask: 0x%08X", m_headerInfo.pixelFormat.bBitMask);
    CCLOG("DDPixelFormat.rgbAlphaBitMask: 0x%08X\n", m_headerInfo.pixelFormat.rgbAlphaBitMask);
    
    CCLOG("DDCaps2.caps1: 0x%08X", m_headerInfo.capabilities.caps1);
    CCLOG("DDCaps2.caps2: 0x%08X", m_headerInfo.capabilities.caps2);
}
