#include "CCTextureATC.h"
#include "CCDDS.h"
#include "BPCRetry.h"

using namespace cocos2d;

CCTextureATC::CCTextureATC() {
    m_textureName = 0;
    m_dds = NULL;
}

CCTextureATC::~CCTextureATC() {
    deleteData();
}

void CCTextureATC::deleteData() {
    if(m_dds) {
        m_dds->release();
    }
    m_dds = NULL;
}

// We cannot autorelease outside the main thread
bool CCTextureATC::initWithDDSAsync(CCDDS* dds) {
    m_pixelFormat = kCCTexture2DPixelFormat_ATC_RGBA;
    m_width  = dds->getHeaderInfo().width;
    m_height = dds->getHeaderInfo().height;
    m_glFormat = dds->getGLFormat();
    m_dds = dds;
    m_dds->retain();
    return true;
}

bool CCTextureATC::initWithDDS(CCDDS* dds) {
    m_pixelFormat = kCCTexture2DPixelFormat_ATC_RGBA;
    m_width  = dds->getHeaderInfo().width;
    m_height = dds->getHeaderInfo().height;
    m_glFormat = dds->getGLFormat();
    m_dds = dds;
    bool textureCreated = createGLTexture();
    m_dds = NULL;
    return textureCreated;
}

// We cannot autorelease outside the main thread
CCTextureATC* CCTextureATC::atcTextureWithDDSAsync(CCDDS* dds) {
    CCTextureATC* newTexture = new CCTextureATC();
    if(newTexture->initWithDDSAsync(dds)) {
        return newTexture;
    }
    delete newTexture;
    return NULL;
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
    if (m_textureName == 0) {
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, m_textureName);
    
    bpcRetry(glCompressedTexImage2D(GL_TEXTURE_2D, 0, m_glFormat, m_width, m_height, 0, \
                           m_dds->getHeaderInfo().pitchOrLinearSize, m_dds->getTextureData()));
    
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
        CCLOG("cocos2d: TextureATC: Error uploading compressed texture level: %u . glError: 0x%04X", (unsigned int)0, err);
        return false;
    }
    
    return true;
}
