#ifndef _BPC_RETRY_GL_
#include <CCGL.h>
#include <CCDirector.h>
#include <sprite_nodes/CCSpriteFrameCache.h>
#include <textures/CCTextureCache.h>
#include <platform/CCPlatformMacros.h>

#define bpcRetry(func) \
    do { \
        func; \
        GLenum const err(::glGetError()); \
        if(err == GL_OUT_OF_MEMORY) { \
            CCLOG("OpenGL out of memory error, attempting to clear caches to recover"); \
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames(); \
            cocos2d::CCDirector::sharedDirector()->purgeCachedData(); \
            cocos2d::CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo(); \
            func; \
        }\
    } while(false)

#endif //_BPC_RETRY_GL_