#include "CCTextureDXT.h"
#include "CCDDS.h"
#include "platform/CCPlatformMacros.h"

using namespace cocos2d;

CCTextureDXT::CCTextureDXT() :
m_textureName(0) {
}

CCTextureDXT::~CCTextureDXT() {
}

bool CCTextureDXT::initWithDDS(CCDDS* dds) {
    
    // format not correct, but avoids assert in
    // CCTexture2D::bitsPerPixelForFormat
    m_pixelFormat = kCCTexture2DPixelFormat_PVRTC4;
    m_width  = dds->getHeaderInfo().width;
    m_height = dds->getHeaderInfo().height;
    m_glFormat = dds->getGLFormat();
    m_dds = dds;
    return createGLTexture();
}

CCTextureDXT* CCTextureDXT::dxtTextureWithDDS(CCDDS* dds) {
    CCTextureDXT* newTexture = new CCTextureDXT();
    if(newTexture->initWithDDS(dds)) {
        newTexture->autorelease();
        return newTexture;
    }
    delete newTexture;
    return NULL;
}

bool CCTextureDXT::createGLTexture() {
    
	GLenum err;
    
    if (m_textureName != 0)
    {
        glDeleteTextures(1, &m_textureName);
    }
    
    glGenTextures(1, &m_textureName);
    glBindTexture(GL_TEXTURE_2D, m_textureName);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    unsigned int nBytes = ((m_width+3)/4)*((m_height+3)/4) * m_dds->getBlockSize();;
    
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, m_glFormat, m_width, m_height, 0, \
                           nBytes, m_dds->getTextureData());
    
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
        CCLOG("cocos2d: TextureDXT: Error uploading compressed texture level: %u . glError: 0x%04X", (unsigned int)0, err);
        return false;
    }
    
    return true;
}