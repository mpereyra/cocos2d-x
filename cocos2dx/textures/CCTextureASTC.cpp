//
//  CCTextureASTC.cpp
//  cocos2dx
//
//  Created by Peng Wei on 7/7/14.
//  Copyright (c) 2014 厦门雅基软件有限公司. All rights reserved.
//

#include "CCTextureASTC.h"
#include "ccMacros.h"
#include "CCConfiguration.h"
#include "support/ccUtils.h"
#include "CCStdC.h"
#include "platform/CCFileUtils.h"
#include "support/zip_support/ZipUtils.h"
#include "shaders/ccGLStateCache.h"
#include <ctype.h>
#include <cctype>
#include "BPCRetry.h"
/* ASTC texture compression internal formats. */
//only supports 4bpp
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR            (0x93B3)

namespace cocos2d {

/* ASTC header declaration. */
typedef struct
{
    unsigned char  magic[4];
    unsigned char  blockdim_x;
    unsigned char  blockdim_y;
    unsigned char  blockdim_z;
    unsigned char  xsize[3];   /* x-size = xsize[0] + xsize[1] + xsize[2] */
    unsigned char  ysize[3];   /* x-size, y-size and z-size are given in texels */
    unsigned char  zsize[3];   /* block count is inferred */
} astc_header;


CCTextureASTC::CCTextureASTC()
: m_data(0)
, m_uWidth(0)
, m_uHeight(0)
, m_bRetainName(false)
, m_uName(0)
, m_eFormat(kCCTexture2DPixelFormat_PVRTC4)
{
}

CCTextureASTC::~CCTextureASTC()
{
    
    if (m_uName != 0 && ! m_bRetainName)
    {
        ccGLDeleteTexture(m_uName);
    }
    deleteData();
}

bool CCTextureASTC::unpackData(unsigned char* data)
{
    
    /* Number of blocks in the x, y and z direction. */
    int xblocks = 0;
    int yblocks = 0;
    int zblocks = 0;
    
    /* Number of bytes for each dimension. */
    int xsize = 0;
    int ysize = 0;
    int zsize = 0;
    
    /* Traverse the file structure. */
    astc_header* astc_data_ptr = (astc_header*) data;
    if(!(astc_data_ptr->blockdim_x==6 && astc_data_ptr->blockdim_y==5)){
        CCLOG("Unsupported astc block dimensions; only 6*5 block (4bpp) is supported");
        return false;
    }
    
    /* Merge x,y,z-sizes from 3 chars into one integer value. */
    xsize = astc_data_ptr->xsize[0] + (astc_data_ptr->xsize[1] << 8) + (astc_data_ptr->xsize[2] << 16);
    ysize = astc_data_ptr->ysize[0] + (astc_data_ptr->ysize[1] << 8) + (astc_data_ptr->ysize[2] << 16);
    zsize = astc_data_ptr->zsize[0] + (astc_data_ptr->zsize[1] << 8) + (astc_data_ptr->zsize[2] << 16);
    m_uWidth=xsize;
    m_uHeight=ysize;
    /* Compute number of blocks in each direction. */
    xblocks = (xsize + astc_data_ptr->blockdim_x - 1) / astc_data_ptr->blockdim_x;
    yblocks = (ysize + astc_data_ptr->blockdim_y - 1) / astc_data_ptr->blockdim_y;
    zblocks = (zsize + astc_data_ptr->blockdim_z - 1) / astc_data_ptr->blockdim_z;
    
    /* Each block is encoded on 16 bytes, so calculate total compressed image data size. */
    m_dataLen = xblocks * yblocks * zblocks << 4;
    return true;
}
bool CCTextureASTC::createGLTexture(){
    /* We now have file contents in memory so let's fill a texture object with the data. */
    if (m_uName != 0)
    {
        ccGLDeleteTexture(m_uName);
    }
    glGenTextures(1, &m_uName);
    ccGLBindTexture2D(m_uName);
    
    /* Upload texture data to ES. */
    glCompressedTexImage2D(GL_TEXTURE_2D,
                                    0,
                                    GL_COMPRESSED_RGBA_ASTC_6x5_KHR,
                                    m_uWidth,
                                    m_uHeight,
                                    0,
                                    m_dataLen,
                                    m_data+sizeof(astc_header));
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    return true;
}

bool CCTextureASTC::initWithContentsOfFile(const char* path)
{
    int len = 0;
    std::string lowerCase(path);
    if (lowerCase.find(".ccz") != std::string::npos)
    {
        len = ZipUtils::ccInflateCCZFile(path, &m_data);
    }
    else if (lowerCase.find(".gz") != std::string::npos)
    {
        len = ZipUtils::ccInflateGZipFile(path, &m_data);
    }
    else
    {
        m_data = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", (unsigned long *)(&len));
    }
    if(len<0){
        this->release();
        return false;
    }
    m_uName = 0;
    m_uWidth = m_uHeight = 0;
    if (!unpackData(m_data) ||!createGLTexture())
    {
        this->release();
        return false;
    }
    
    return true;
}
    
    bool CCTextureASTC::initWithContentsOfFileAsync(const char* path)
    {
        int len = 0;
        std::string lowerCase(path);
        if (lowerCase.find(".ccz") != std::string::npos)
        {
            len = ZipUtils::ccInflateCCZFile(path, &m_data);
        }
        else if (lowerCase.find(".gz") != std::string::npos)
        {
            len = ZipUtils::ccInflateGZipFile(path, &m_data);
        }
        else
        {
            m_data = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", (unsigned long *)(&len));
        }
        
        if(len<0){
            deleteData();
            this->release();
            return false;
        }
        m_uName = 0;
        m_uWidth = m_uHeight = 0;
        if (!unpackData(m_data))
        {
            deleteData();
            this->release();
            return false;
        }
        /* Keep our GL texture name alive, even after we're destroyed. */
        setRetainName(true);
        
        return true;
    }


CCTextureASTC * CCTextureASTC::create(const char* path)
{
    CCTextureASTC * pTexture = new CCTextureASTC();
    if (pTexture)
    {
        if (pTexture->initWithContentsOfFile(path))
        {
            pTexture->autorelease();
        }
        else
        {
            delete pTexture;
            pTexture = NULL;
        }
    }
    
    return pTexture;
}
}
