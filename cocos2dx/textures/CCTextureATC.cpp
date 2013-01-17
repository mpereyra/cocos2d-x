#include "CCTextureATC.h"
#include "CCDDS.h"


using namespace cocos2d;

CCTextureATC::CCTextureATC() {
    m_textureName = 0;
}

CCTextureATC::~CCTextureATC() {
}

bool CCTextureATC::initWithDDS(CCDDS* dds) {
    
    // Both variants of supported ATI texture compressions are 8bpp and have an alpha channel
    m_pixelFormat = kCCTexture2DPixelFormat_RGBA8888;
    m_width  = dds->getHeaderInfo().width;
    m_height = dds->getHeaderInfo().height;
    m_glFormat = dds->getGLFormat();
    m_dds = dds;
    return createGLTexture();
}

CCTextureATC* CCTextureATC::atcTextureWithDDS(CCDDS* dds) {
    CCTextureATC* newTexture = new CCTextureATC();
    if(newTexture->initWithDDS(dds)) {
        newTexture->autorelease();
        return newTexture;
    }
    delete newTexture;
    return NULL;
}

bool CCTextureATC::createGLTexture() {
    
	GLenum err;
    
    if (m_textureName != 0)
    {
        glDeleteTextures(1, &m_textureName);
    }
    
    glGenTextures(1, &m_textureName);
    glBindTexture(GL_TEXTURE_2D, m_textureName);
    
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, m_glFormat, m_width, m_height, 0, \
                           m_dds->getHeaderInfo().pitchOrLinearSize, m_dds->getTextureData());
    
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
       // CCLOG("cocos2d: TextureATC: Error uploading compressed texture level: %u . glError: 0x%04X", (unsigned int)0, err);
        return false;
    }
    
    return true;
}