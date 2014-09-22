/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "ccGLStateCache.h"
#include "CCGLProgram.h"
#include "CCDirector.h"
#include "ccConfig.h"

// extern
#include "kazmath/GL/matrix.h"
#include "kazmath/kazmath.h"

NS_CC_BEGIN

#if BPC_STATE_CACHE
static Bpc::AbstractStateCache* bpcStateCache = nullptr;
#else
static GLuint    s_uCurrentProjectionMatrix = -1;
static bool        s_bVertexAttribPosition = false;
static bool        s_bVertexAttribColor = false;
static bool        s_bVertexAttribTexCoords = false;


#if CC_ENABLE_GL_STATE_CACHE

#define kCCMaxActiveTexture 16

static GLuint    s_uCurrentShaderProgram = -1;
static GLuint    s_uCurrentBoundTexture[kCCMaxActiveTexture] =  {(GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
static GLenum    s_eCurrentActiveTexture = (GL_TEXTURE0 - GL_TEXTURE0);
static GLenum    s_eBlendingSource = -1;
static GLenum    s_eBlendingDest = -1;
static int      s_eGLServerState = 0;

#endif // CC_ENABLE_GL_STATE_CACHE

#endif // BPC_STATE_CACHE
// GL State Cache functions

void ccGLInvalidateStateCache( void )
{
#if BPC_STATE_CACHE
    bpcStateCache->clearCache();
#else
    kmGLFreeAll();
    s_uCurrentProjectionMatrix = -1;
    s_bVertexAttribPosition = false;
    s_bVertexAttribColor = false;
    s_bVertexAttribTexCoords = false;
#if CC_ENABLE_GL_STATE_CACHE
    s_uCurrentShaderProgram = -1;
    for( int i=0; i < kCCMaxActiveTexture; i++ )
    {
        s_uCurrentBoundTexture[i] = -1;
    }
    s_eCurrentActiveTexture = (GL_TEXTURE0 - GL_TEXTURE0);
    s_eBlendingSource = -1;
    s_eBlendingDest = -1;
    s_eGLServerState = 0;
#endif
#endif
}

void ccGLDeleteProgram( GLuint program )
{
#if BPC_STATE_CACHE
    glDeleteProgram( program ); // TODO
#else
#if CC_ENABLE_GL_STATE_CACHE
    if( program == s_uCurrentShaderProgram )
        s_uCurrentShaderProgram = -1;
#endif // CC_ENABLE_GL_STATE_CACHE

    glDeleteProgram( program );
#endif
}

void ccGLUseProgram( GLuint program )
{
#if BPC_STATE_CACHE
    glUseProgram(program); // TODO
#else
#if CC_ENABLE_GL_STATE_CACHE
    if( program != s_uCurrentShaderProgram ) {
        s_uCurrentShaderProgram = program;
        glUseProgram(program);
    }
#else
    glUseProgram(program);
#endif // CC_ENABLE_GL_STATE_CACHE
#endif
}


void ccGLBlendFunc(GLenum sfactor, GLenum dfactor)
{
#if BPC_STATE_CACHE
    bpcStateCache->setBlendFunc(sfactor, dfactor);
#else
#if CC_ENABLE_GL_STATE_CACHE
    if( sfactor != s_eBlendingSource || dfactor != s_eBlendingDest ) {
        s_eBlendingSource = sfactor;
        s_eBlendingDest = dfactor;
        glBlendFunc( sfactor, dfactor );
    }
#else
    glBlendFunc( sfactor, dfactor );
#endif // CC_ENABLE_GL_STATE_CACHE
#endif
}

GLenum ccGLGetActiveTexture( void )
{
#if BPC_STATE_CACHE
    return bpcStateCache->getActiveTextureUnit();
#else
#if CC_ENABLE_GL_STATE_CACHE
    return s_eCurrentActiveTexture + GL_TEXTURE0;
#else
    GLenum activeTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&activeTexture);
    return activeTexture;
#endif
#endif
}

void ccGLActiveTexture( GLenum textureEnum )
{
#if BPC_STATE_CACHE
    bpcStateCache->activateGLTextureUnit(textureEnum);
#else
#if CC_ENABLE_GL_STATE_CACHE
    CCAssert( (textureEnum - GL_TEXTURE0) < kCCMaxActiveTexture, "cocos2d ERROR: Increase kCCMaxActiveTexture to kCCMaxActiveTexture!");
    if( (textureEnum - GL_TEXTURE0) != s_eCurrentActiveTexture ) {
        s_eCurrentActiveTexture = (textureEnum - GL_TEXTURE0);
        glActiveTexture( textureEnum );
    }
#else
    glActiveTexture( textureEnum );
#endif
#endif
}

void ccGLBindTexture2D( GLuint textureId )
{
#if BPC_STATE_CACHE
    bpcStateCache->bindGLTexture(GL_TEXTURE_2D, textureId);
#else
#if CC_ENABLE_GL_STATE_CACHE
    if( s_uCurrentBoundTexture[ s_eCurrentActiveTexture ] != textureId )
    {
        s_uCurrentBoundTexture[ s_eCurrentActiveTexture ] = textureId;
        glBindTexture(GL_TEXTURE_2D, textureId );
    }
#else
    glBindTexture(GL_TEXTURE_2D, textureId );
#endif
#endif
}


void ccGLDeleteTexture( GLuint textureId )
{
#if BPC_STATE_CACHE
    bpcStateCache->deleteTexture(textureId);
#else
#if CC_ENABLE_GL_STATE_CACHE
    if( textureId == s_uCurrentBoundTexture[ s_eCurrentActiveTexture ] )
       s_uCurrentBoundTexture[ s_eCurrentActiveTexture ] = -1;
#endif
    glDeleteTextures(1, &textureId );
#endif
}

void ccGLEnable( ccGLServerState flags )
{
#if BPC_STATE_CACHE
    if( flags & CC_GL_BLEND )
        bpcStateCache->setEnabled(GL_BLEND);
    else
        bpcStateCache->setDisabled(GL_BLEND);
#else
#if CC_ENABLE_GL_STATE_CACHE

    int enabled = 0;

    /* GL_BLEND */
    if( (enabled = (flags & CC_GL_BLEND)) != (s_eGLServerState & CC_GL_BLEND) ) {
        if( enabled ) {
            glEnable( GL_BLEND );
            s_eGLServerState |= CC_GL_BLEND;
        } else {
            glDisable( GL_BLEND );
            s_eGLServerState &=  ~CC_GL_BLEND;
        }
    }

#else
    if( flags & CC_GL_BLEND )
        glEnable( GL_BLEND );
    else
        glDisable( GL_BLEND );
#endif
#endif
}

//#pragma mark - GL Vertex Attrib functions

void ccGLEnableVertexAttribs( unsigned int flags )
{
#if BPC_STATE_CACHE
    bool enablePosition = flags & kCCVertexAttribFlag_Position;
    bool enableColor = (flags & kCCVertexAttribFlag_Color) != 0 ? true : false;
    bool enableTexCoords = (flags & kCCVertexAttribFlag_TexCoords) != 0 ? true : false;

    if(enablePosition)
        bpcStateCache->setVertexAttribEnabled(kCCVertexAttrib_Position);
    else
        bpcStateCache->setVertexAttribDisabled(kCCVertexAttrib_Position);

    if(enableColor)
        bpcStateCache->setVertexAttribEnabled(kCCVertexAttrib_Color);
    else
        bpcStateCache->setVertexAttribDisabled(kCCVertexAttrib_Color);

    if(enableTexCoords)
        bpcStateCache->setVertexAttribEnabled(kCCVertexAttrib_TexCoords);
    else
        bpcStateCache->setVertexAttribDisabled(kCCVertexAttrib_TexCoords);
#else
    /* Position */
    bool enablePosition = flags & kCCVertexAttribFlag_Position;

    if( enablePosition != s_bVertexAttribPosition ) {
        if( enablePosition )
            glEnableVertexAttribArray( kCCVertexAttrib_Position );
        else
            glDisableVertexAttribArray( kCCVertexAttrib_Position );

        s_bVertexAttribPosition = enablePosition;
    }

    /* Color */
    bool enableColor = (flags & kCCVertexAttribFlag_Color) != 0 ? true : false;

    if( enableColor != s_bVertexAttribColor ) {
        if( enableColor )
            glEnableVertexAttribArray( kCCVertexAttrib_Color );
        else
            glDisableVertexAttribArray( kCCVertexAttrib_Color );

        s_bVertexAttribColor = enableColor;
    }

    /* Tex Coords */
    bool enableTexCoords = (flags & kCCVertexAttribFlag_TexCoords) != 0 ? true : false;

    if( enableTexCoords != s_bVertexAttribTexCoords ) {
        if( enableTexCoords )
            glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );
        else
            glDisableVertexAttribArray( kCCVertexAttrib_TexCoords );

        s_bVertexAttribTexCoords = enableTexCoords;
    }
#endif
}

//#pragma mark - GL Uniforms functions

void ccSetProjectionMatrixDirty( void )
{
#if BPC_STATE_CACHE
#else
    s_uCurrentProjectionMatrix = -1;
#endif
}

#if BPC_STATE_CACHE
void setStateCache(Bpc::AbstractStateCache* sharedCache) {
    bpcStateCache = sharedCache;
}
#endif
NS_CC_END
