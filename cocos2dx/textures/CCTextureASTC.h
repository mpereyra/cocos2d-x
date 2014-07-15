//
//  CCTextureASTC.h
//  cocos2dx
//
//  Created by Peng Wei on 7/7/14.
//  Copyright (c) 2014 厦门雅基软件有限公司. All rights reserved.
//

#ifndef __cocos2dx__CCTextureASTC__
#define __cocos2dx__CCTextureASTC__

#include "CCStdC.h"
#include "CCGL.h"
#include "cocoa/CCObject.h"
#include "cocoa/CCArray.h"
#include "CCTexture2D.h"

/**
 * @addtogroup textures
 * @{
 */

namespace cocos2d {
//Doesn't support mipmaps.
//We have some mipmap support in CCTexturePVR, but I'm pretty sure we don't use it.
class CCTextureASTC : public CCObject
{
public:
    CCTextureASTC();
    virtual ~CCTextureASTC();
    
    bool initWithContentsOfFile(const char* path);
    bool initWithContentsOfFileAsync(const char* path);
    bool createGLTexture();
    static CCTextureASTC* create(const char* path);
    
    // properties
    
    inline unsigned int getName() { return m_uName; }
    inline unsigned int getWidth() { return m_uWidth; }
    inline unsigned int getHeight() { return m_uHeight; }
    inline CCTexture2DPixelFormat getFormat() { return m_eFormat; }
    inline bool isRetainName() { return m_bRetainName; }
    inline void setRetainName(bool retainName) { m_bRetainName = retainName; }
    inline void deleteData() { CC_SAFE_DELETE_ARRAY(m_data); }
private:
    bool unpackData(unsigned char* data);
protected:
    unsigned char* m_data;
    unsigned int m_dataLen = 0;
    unsigned int m_uWidth, m_uHeight;
    GLuint m_uName;
    
    // cocos2d integration
    bool m_bRetainName;
    CCTexture2DPixelFormat m_eFormat;
};

// end of textures group
/// @}

}

#endif /* defined(__cocos2dx__CCTextureASTC__) */
