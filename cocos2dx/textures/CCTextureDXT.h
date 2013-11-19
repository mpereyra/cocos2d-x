// This class creates a CCTexture2D from a compressed S3 file


#ifndef __CCS3TEXTURE_H__
#define __CCS3TEXTURE_H__
#include "cocoa/CCObject.h"
#include "CCGL.h"
#include "CCTexture2D.h"
#include <stdint.h>

namespace cocos2d {
    class CCDDS;
    class CCTextureDXT : public CCObject {
        
    private:
        CCDDS* m_dds;
        uint32_t m_width;
        uint32_t m_height;
        CCTexture2DPixelFormat m_pixelFormat;
        int m_glFormat;
        GLuint m_textureName;
        bool m_bRetainName;
        
        void deleteData();
        
    public:
        CCTextureDXT();
        virtual ~CCTextureDXT();
        
        bool initWithDDSAsync(CCDDS* dds);
        bool initWithDDS(CCDDS* dds);
        static CCTextureDXT* dxtTextureWithDDSAsync(CCDDS* dds);
        static CCTextureDXT* dxtTextureWithDDS(CCDDS* dds);
        
        uint32_t getWidth() { return m_width; }
        uint32_t getHeight() { return m_height; }
        CCTexture2DPixelFormat getPixelFormat() { return m_pixelFormat; }
        GLuint getName() { return m_textureName; }
        
        bool getRetainName() { return m_bRetainName; }
        void setRetainName(bool retain) { m_bRetainName = retain; }
        
        bool createGLTexture();
        
    };
}
#endif
