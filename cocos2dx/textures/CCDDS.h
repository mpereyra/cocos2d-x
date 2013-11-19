// This class loads a DDS file into memory, that is all.
// It does NOT load the texture inside the DDS
#ifndef __CCDDSFILE_H__
#define __CCDDSFILE_H__

#include "cocoa/CCObject.h"
#include "ccMacros.h"
#include <stdint.h>

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

#define FOURCC(a, b, c, d)       \
((uint32_t)((uint32_t)(a)) | \
((uint32_t)(b) <<  8)      | \
((uint32_t)(c) << 16)      | \
((uint32_t)(d) << 24))

namespace cocos2d {
    
    
    typedef enum
    {
        DDS_COMPRESS_NONE = 0,
        DDS_COMPRESS_S3,   /* DXT1, DTX3, DTC5 */
        DDS_COMPRESS_ATI,  /* ATCI, ATCA  */
        DDS_COMPRESS_MAX
    } DDSCompressionType;
    
    struct DDPixelFormat {
        int32_t size;
        int32_t flags;
        int32_t fourCC;
        int32_t rgbBitCount;
        int32_t rBitMask;
        int32_t gBitMask;
        int32_t bBitMask;
        int32_t rgbAlphaBitMask;
    };
    
    struct DDCaps2 {
        int32_t caps1;
        int32_t caps2;
        int32_t reserved[2];
    };
    
    struct DDSSurfaceDesc2 {
        int32_t size;
        int32_t flags;
        int32_t height;
        int32_t width;
        int32_t pitchOrLinearSize;
        int32_t depth;
        int32_t mipCount;
        int32_t reserved[11];
        DDPixelFormat pixelFormat;
        DDCaps2 capabilities;
        int32_t reserved2;
    };
    
    
    class CCDDS : public CCObject {
        
    private:
        static const int MAX_MIPMAP_LEVEL=16;
        
        // The surface description is really the important information that CCDDS provides
        DDSSurfaceDesc2     m_headerInfo;
        unsigned int        m_blockSize;
        
        // To quickly find out if it is a S3 or ATC compression format
        DDSCompressionType  m_compressionType;
        // The actually GL parameter we want to pass when creating the texture
        int                 m_glFormat;
        
        // The actual texture data, as is.
        unsigned char*  m_textureData;
        
        // We need to retain this because we store a pointer to somewhere in the middle of it
        // (m_textureData)
        unsigned char* m_originalData;
        // This flag will be set on ddsWithContentsOfFile, but not when called with external data
        // (ddsWithData)
        bool  m_internallyAllocated;
        
    public:
        CCDDS();
        virtual ~CCDDS();
        
        bool initWithData(void* buffer, unsigned long size);
        bool initWithContentsOfFile(const char* path);
        static CCDDS* ddsWithData(void* buffer, unsigned long size);
        static CCDDS* ddsWithContentsOfFileAsync(const char* path);
        static CCDDS* ddsWithContentsOfFile(const char* path);
        
        void* getTextureData() { return m_textureData; }
        const DDSSurfaceDesc2& getHeaderInfo() { return m_headerInfo; }
        DDSCompressionType getCompressionType() { return m_compressionType; }
        int getGLFormat() { return m_glFormat; }
        unsigned int getBlockSize() { return m_blockSize; }
        
        
        void logDescription(void);
    };
}
#endif // __CCDDSFILE_H__
