// This class creates a CCTexture2D from a compressed ATITC file


#ifndef __CCATITEXTURE_H__
#define __CCATITEXTURE_H__
#include "cocoa/CCObject.h"
#include "CCGL.h"
#include "CCTexture2D.h"
#include <stdint.h>

namespace cocos2d {
    class CCDDS;
    class CCTextureATC : public CCObject {
        
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
        CCTextureATC();
        virtual ~CCTextureATC();
        
        bool initWithDDS(CCDDS* dds);
        bool initWithDDSAsync(CCDDS* dds);
        static CCTextureATC* atcTextureWithDDS(CCDDS* dds);
        static CCTextureATC* atcTextureWithDDSAsync(CCDDS* dds);
        
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
