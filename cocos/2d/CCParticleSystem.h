/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2017 Chukong Technologies Inc.

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
#ifndef __CCPARTICLE_SYSTEM_H__
#define __CCPARTICLE_SYSTEM_H__

#include "base/CCProtocols.h"
#include "2d/CCNode.h"
#include "base/CCValue.h"

NS_CC_BEGIN

/**
 * @addtogroup _2d
 * @{
 */

class ParticleBatchNode;

/** @struct sParticle
Structure that contains the values of each particle.
*/

struct particle_point
{
    float x;
    float y;
};

class CC_DLL ParticleData
{
public:
    float* posx;
    float* posy;
    float* startPosX;
    float* startPosY;

    float* colorR;
    float* colorG;
    float* colorB;
    float* colorA;
    
    float* deltaColorR;
    float* deltaColorG;
    float* deltaColorB;
    float* deltaColorA;
    
    float* size;
    float* deltaSize;
    float* rotation;
    float* deltaRotation;
    float* timeToLive;
    unsigned int* atlasIndex;
    
    //! Mode A: gravity, direction, radial accel, tangential accel
    struct{
        float* dirX;
        float* dirY;
        float* radialAccel;
        float* tangentialAccel;
    } modeA;
    
    //! Mode B: radius mode
    struct{
        float* angle;
        float* degreesPerSecond;
        float* radius;
        float* deltaRadius;
    } modeB;
    
    unsigned int maxCount;
    ParticleData();
    bool init(int count);
    void release();
    unsigned int getMaxCount() { return maxCount; }
    
    void copyParticle(int p1, int p2)
    {
        posx[p1] = posx[p2];
        posy[p1] = posy[p2];
        startPosX[p1] = startPosX[p2];
        startPosY[p1] = startPosY[p2];
        
        colorR[p1] = colorR[p2];
        colorG[p1] = colorG[p2];
        colorB[p1] = colorB[p2];
        colorA[p1] = colorA[p2];
        
        deltaColorR[p1] = deltaColorR[p2];
        deltaColorG[p1] = deltaColorG[p2];
        deltaColorB[p1] = deltaColorB[p2];
        deltaColorA[p1] = deltaColorA[p2];
        
        size[p1] = size[p2];
        deltaSize[p1] = deltaSize[p2];
        
        rotation[p1] = rotation[p2];
        deltaRotation[p1] = deltaRotation[p2];
        
        timeToLive[p1] = timeToLive[p2];
        
        atlasIndex[p1] = atlasIndex[p2];
        
        modeA.dirX[p1] = modeA.dirX[p2];
        modeA.dirY[p1] = modeA.dirY[p2];
        modeA.radialAccel[p1] = modeA.radialAccel[p2];
        modeA.tangentialAccel[p1] = modeA.tangentialAccel[p2];
        
        modeB.angle[p1] = modeB.angle[p2];
        modeB.degreesPerSecond[p1] = modeB.degreesPerSecond[p2];
        modeB.radius[p1] = modeB.radius[p2];
        modeB.deltaRadius[p1] = modeB.deltaRadius[p2];
        
    }
};



//typedef void (*CC_UPDATE_PARTICLE_IMP)(id, SEL, tParticle*, Vec2);

class Texture2D;

/** @class ParticleSystem
 * @brief Particle System base class.
Attributes of a Particle System:
- emission rate of the particles
- Gravity Mode (Mode A):
- gravity
- direction
- speed +-  variance
- tangential acceleration +- variance
- radial acceleration +- variance
- Radius Mode (Mode B):
- startRadius +- variance
- endRadius +- variance
- rotate +- variance
- Properties common to all modes:
- life +- life variance
- start spin +- variance
- end spin +- variance
- start size +- variance
- end size +- variance
- start color +- variance
- end color +- variance
- life +- variance
- blending function
- texture

Cocos2d also supports particles generated by Particle Designer (http://particledesigner.71squared.com/).
'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
cocos2d uses a another approach, but the results are almost identical. 

Cocos2d supports all the variables used by Particle Designer plus a bit more:
- spinning particles (supported when using ParticleSystemQuad)
- tangential acceleration (Gravity mode)
- radial acceleration (Gravity mode)
- radius direction (Radius mode) (Particle Designer supports outwards to inwards direction only)

It is possible to customize any of the above mentioned properties in runtime. Example:

@code
emitter.radialAccel = 15;
emitter.startSpin = 0;
@endcode

*/

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#ifdef RELATIVE
#undef RELATIVE
#endif
#endif

class CC_DLL ParticleSystem : public Node, public TextureProtocol, public PlayableProtocol
{
public:
    /** Mode
     * @js cc.ParticleSystem.MODE_GRAVITY;
     */
    enum class Mode
    {
        GRAVITY,
        RADIUS,
    };
    
    /** PositionType
     Possible types of particle positions.
     * @js cc.ParticleSystem.TYPE_FREE
     */
    enum class PositionType
    {
        FREE, /** Living particles are attached to the world and are unaffected by emitter repositioning. */
        
        RELATIVE, /** Living particles are attached to the world but will follow the emitter repositioning.
                   Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.*/
        
        GROUPED, /** Living particles are attached to the emitter and are translated along with it. */

    };
    
    //* @enum
    enum {
        /** The Particle emitter lives forever. */
        DURATION_INFINITY = -1,
        
        /** The starting size of the particle is equal to the ending size. */
        START_SIZE_EQUAL_TO_END_SIZE = -1,
        
        /** The starting radius of the particle is equal to the ending radius. */
        START_RADIUS_EQUAL_TO_END_RADIUS = -1,
    };
    
    /** Creates an initializes a ParticleSystem from a plist file.
    This plist files can be created manually or with Particle Designer:
    http://particledesigner.71squared.com/
     @since v2.0
     *
     * @param plistFile Particle plist file name.
     * @return An autoreleased ParticleSystem object.
     */
    static ParticleSystem * create(const std::string& plistFile);

    /** Create a system with a fixed number of particles.
     *
     * @param numberOfParticles A given number of particles.
     * @return An autoreleased ParticleSystemQuad object.
     * @js NA
     */
    static ParticleSystem* createWithTotalParticles(int numberOfParticles);

    /** Gets all ParticleSystem references
     */
    static Vector<ParticleSystem*>& getAllParticleSystems();
public:
    void addParticles(int count);
    
    void stopSystem();
    /** Kill all living particles.
     */
    void resetSystem();
    /** Whether or not the system is full.
     *
     * @return True if the system is full.
     */
    bool isFull();

    /** Update the verts position data of particle,
     should be overridden by subclasses. 
     */
    virtual void updateParticleQuads();
    /** Update the VBO verts buffer which does not use batch node,
     should be overridden by subclasses. */
    virtual void postStep();

    /** Call the update method with no time..
     */
    virtual void updateWithNoTime();

    /** Whether or not the particle system removed self on finish.
     *
     * @return True if the particle system removed self on finish.
     */
    virtual bool isAutoRemoveOnFinish() const;
    
    /** Set the particle system auto removed it self on finish.
     *
     * @param var True if the particle system removed self on finish.
     */
    virtual void setAutoRemoveOnFinish(bool var);

    virtual void setDoneCallback(std::function<void(ParticleSystem*)> cb);
    
    // mode A
    /** Gets the gravity.
     *
     * @return The gravity.
     */
    virtual const Vec2& getGravity();
    /** Sets the gravity.
     *
     * @param g The gravity.
     */
    virtual void setGravity(const Vec2& g);
    /** Gets the speed.
     *
     * @return The speed.
     */
    virtual float getSpeed() const;
    /** Sets the speed.
     *
     * @param speed The speed.
     */
    virtual void setSpeed(float speed);
    /** Gets the speed variance.
     *
     * @return The speed variance.
     */
    virtual float getSpeedVar() const;
    /** Sets the speed variance.
     *
     * @param speed The speed variance.
     */
    virtual void setSpeedVar(float speed);
    /** Gets the tangential acceleration.
     *
     * @return The tangential acceleration.
     */
    virtual float getTangentialAccel() const;
    /** Sets the tangential acceleration.
     *
     * @param t The tangential acceleration.
     */
    virtual void setTangentialAccel(float t);
    /** Gets the tangential acceleration variance.
     *
     * @return The tangential acceleration variance.
     */
    virtual float getTangentialAccelVar() const;
    /** Sets the tangential acceleration variance.
     *
     * @param t The tangential acceleration variance.
     */
    virtual void setTangentialAccelVar(float t);
    /** Gets the radial acceleration.
     *
     * @return The radial acceleration.
     */
    virtual float getRadialAccel() const;
    /** Sets the radial acceleration.
     *
     * @param t The radial acceleration.
     */
    virtual void setRadialAccel(float t);
    /** Gets the radial acceleration variance.
     *
     * @return The radial acceleration variance.
     */
    virtual float getRadialAccelVar() const;
    /** Sets the radial acceleration variance.
     *
     * @param t The radial acceleration variance.
     */
    virtual void setRadialAccelVar(float t);
    /** Whether or not the rotation of each particle to its direction.
     *
     * @return True if the rotation is the direction.
     */
    virtual bool getRotationIsDir() const;
    /** Sets the rotation of each particle to its direction.
     *
     * @param t True if the rotation is the direction.
     */
    virtual void setRotationIsDir(bool t);
    // mode B
    /** Gets the start radius.
     *
     * @return The start radius.
     */
    virtual float getStartRadius() const;
    /** Sets the start radius.
     *
     * @param startRadius The start radius.
     */
    virtual void setStartRadius(float startRadius);
    /** Gets the start radius variance.
     *
     * @return The start radius variance.
     */
    virtual float getStartRadiusVar() const;
    /** Sets the start radius variance.
     *
     * @param startRadiusVar The start radius variance.
     */
    virtual void setStartRadiusVar(float startRadiusVar);
    /** Gets the end radius.
     *
     * @return The end radius.
     */
    virtual float getEndRadius() const;
    /** Sets the end radius.
     *
     * @param endRadius The end radius.
     */
    virtual void setEndRadius(float endRadius);
    /** Gets the end radius variance.
     *
     * @return The end radius variance.
     */
    virtual float getEndRadiusVar() const;
    /** Sets the end radius variance.
     *
     * @param endRadiusVar The end radius variance.
     */
    virtual void setEndRadiusVar(float endRadiusVar);
    /** Gets the number of degrees to rotate a particle around the source pos per second.
     *
     * @return The number of degrees to rotate a particle around the source pos per second.
     */
    virtual float getRotatePerSecond() const;
    /** Sets the number of degrees to rotate a particle around the source pos per second.
     *
     * @param degrees The number of degrees to rotate a particle around the source pos per second.
     */
    virtual void setRotatePerSecond(float degrees);
    /** Gets the rotate per second variance.
     *
     * @return The rotate per second variance.
     */
    virtual float getRotatePerSecondVar() const;
    /** Sets the rotate per second variance.
     *
     * @param degrees The rotate per second variance.
     */
    virtual void setRotatePerSecondVar(float degrees);

    virtual void setScale(float s) override;
    virtual void setRotation(float newRotation) override;
    virtual void setScaleX(float newScaleX) override;
    virtual void setScaleY(float newScaleY) override;

    /** Whether or not the particle system is active.
     *
     * @return True if the particle system is active.
     */
    virtual bool isActive() const;
    /** Whether or not the particle system is blend additive.
     *
     * @return True if the particle system is blend additive.
     */
    virtual bool isBlendAdditive() const;
    /** Sets the particle system blend additive.
     *
     * @param value True if the particle system is blend additive.
     */
    virtual void setBlendAdditive(bool value);

    /** Gets the batch node.
     *
     * @return The batch node.
     */
    virtual ParticleBatchNode* getBatchNode() const;
    /** Sets the batch node.
     *
     * @param batchNode The batch node.
     */
    virtual void setBatchNode(ParticleBatchNode* batchNode);
    
    /** Gets the index of system in batch node array.
     *
     * @return The index of system in batch node array.
     */
    int getAtlasIndex() const { return _atlasIndex; }
    /** Sets the index of system in batch node array.
     *
     * @param index The index of system in batch node array.
     */
    void setAtlasIndex(int index) { _atlasIndex = index; }

    /** Gets the Quantity of particles that are being simulated at the moment.
     *
     * @return The Quantity of particles that are being simulated at the moment.
     */
    unsigned int getParticleCount() const { return _particleCount; }
    
    /** Gets how many seconds the emitter will run. -1 means 'forever'.
     *
     * @return The seconds that the emitter will run. -1 means 'forever'.
     */
    float getDuration() const { return _duration; }
    /** Sets how many seconds the emitter will run. -1 means 'forever'.
     *
     * @param duration The seconds that the emitter will run. -1 means 'forever'.
     */
    void setDuration(float duration) { _duration = duration; }
    
    /** Gets the source position of the emitter.
     *
     * @return The source position of the emitter.
     */
    const Vec2& getSourcePosition() const { return _sourcePosition; }
    /** Sets the source position of the emitter.
     *
     * @param pos The source position of the emitter.
     */
    void setSourcePosition(const Vec2& pos) { _sourcePosition = pos; }
    
    /** Gets the position variance of the emitter.
     *
     * @return The position variance of the emitter.
     */
    const Vec2& getPosVar() const { return _posVar; }
    /** Sets the position variance of the emitter.
     *
     * @param pos The position variance of the emitter.
     */
    void setPosVar(const Vec2& pos) { _posVar = pos; }

    /** Gets the life of each particle.
     *
     * @return The life of each particle.
     */
    float getLife() const { return _life; }
    /** Sets the life of each particle.
     *
     * @param life The life of each particle.
     */
    void setLife(float life) { _life = life; }

    /** Gets the life variance of each particle.
     *
     * @return The life variance of each particle.
     */
    float getLifeVar() const { return _lifeVar; }
    /** Sets the life variance of each particle.
     *
     * @param lifeVar The life variance of each particle.
     */
    void setLifeVar(float lifeVar) { _lifeVar = lifeVar; }

    /** Gets the angle of each particle. 
     *
     * @return The angle of each particle.
     */
    float getAngle() const { return _angle; }
    /** Sets the angle of each particle.
     *
     * @param angle The angle of each particle.
     */
    void setAngle(float angle) { _angle = angle; }

    /** Gets the angle variance of each particle.
     *
     * @return The angle variance of each particle.
     */
    float getAngleVar() const { return _angleVar; }
    /** Sets the angle variance of each particle.
     *
     * @param angleVar The angle variance of each particle.
     */
    void setAngleVar(float angleVar) { _angleVar = angleVar; }
    
    /** Switch between different kind of emitter modes:
     - kParticleModeGravity: uses gravity, speed, radial and tangential acceleration.
     - kParticleModeRadius: uses radius movement + rotation.
     *
     * @return The mode of the emitter.
     */
    Mode getEmitterMode() const { return _emitterMode; }
    /** Sets the mode of the emitter.
     *
     * @param mode The mode of the emitter.
     */
    void setEmitterMode(Mode mode) { _emitterMode = mode; }
    
    /** Gets the start size in pixels of each particle.
     *
     * @return The start size in pixels of each particle.
     */
    float getStartSize() const { return _startSize; }
    /** Sets the start size in pixels of each particle.
     *
     * @param startSize The start size in pixels of each particle.
     */
    void setStartSize(float startSize) { _startSize = startSize; }

    /** Gets the start size variance in pixels of each particle.
     *
     * @return The start size variance in pixels of each particle.
     */
    float getStartSizeVar() const { return _startSizeVar; }
    /** Sets the start size variance in pixels of each particle.
     *
     * @param sizeVar The start size variance in pixels of each particle.
     */
    void setStartSizeVar(float sizeVar) { _startSizeVar = sizeVar; }

    /** Gets the end size in pixels of each particle.
     *
     * @return The end size in pixels of each particle.
     */
    float getEndSize() const { return _endSize; }
    /** Sets the end size in pixels of each particle.
     *
     * @param endSize The end size in pixels of each particle.
     */
    void setEndSize(float endSize) { _endSize = endSize; }

    /** Gets the end size variance in pixels of each particle.
     *
     * @return The end size variance in pixels of each particle.
     */
    float getEndSizeVar() const { return _endSizeVar; }
    /** Sets the end size variance in pixels of each particle.
     *
     * @param sizeVar The end size variance in pixels of each particle.
     */
    void setEndSizeVar(float sizeVar) { _endSizeVar = sizeVar; }

    /** Gets the start color of each particle.
     *
     * @return The start color of each particle.
     */
    const Color4F& getStartColor() const { return _startColor; }
    /** Sets the start color of each particle.
     *
     * @param color The start color of each particle.
     */
    void setStartColor(const Color4F& color) { _startColor = color; }

    /** Gets the start color variance of each particle.
     *
     * @return The start color variance of each particle.
     */
    const Color4F& getStartColorVar() const { return _startColorVar; }
    /** Sets the start color variance of each particle.
     *
     * @param color The start color variance of each particle.
     */
    void setStartColorVar(const Color4F& color) { _startColorVar = color; }

    /** Gets the end color and end color variation of each particle.
     *
     * @return The end color and end color variation of each particle.
     */
    const Color4F& getEndColor() const { return _endColor; }
    /** Sets the end color and end color variation of each particle.
     *
     * @param color The end color and end color variation of each particle.
     */
    void setEndColor(const Color4F& color) { _endColor = color; }

    /** Gets the end color variance of each particle.
     *
     * @return The end color variance of each particle.
     */
    const Color4F& getEndColorVar() const { return _endColorVar; }
    /** Sets the end color variance of each particle.
     *
     * @param color The end color variance of each particle.
     */
    void setEndColorVar(const Color4F& color) { _endColorVar = color; }

    /** Gets the start spin of each particle.
     *
     * @return The start spin of each particle.
     */
    float getStartSpin() const { return _startSpin; }
    /** Sets the start spin of each particle.
     *
     * @param spin The start spin of each particle.
     */
    void setStartSpin(float spin) { _startSpin = spin; }

    /** Gets the start spin variance of each particle.
     *
     * @return The start spin variance of each particle.
     */
    float getStartSpinVar() const { return _startSpinVar; }
    /** Sets the start spin variance of each particle.
     *
     * @param pinVar The start spin variance of each particle.
     */
    void setStartSpinVar(float pinVar) { _startSpinVar = pinVar; }

    /** Gets the end spin of each particle.
     *
     * @return The end spin of each particle.
     */
    float getEndSpin() const { return _endSpin; }
    /** Sets the end spin of each particle.
     *
     * @param endSpin The end spin of each particle.
     */
    void setEndSpin(float endSpin) { _endSpin = endSpin; }

    /** Gets the end spin variance of each particle.
     *
     * @return The end spin variance of each particle.
     */
    float getEndSpinVar() const { return _endSpinVar; }
    /** Sets the end spin variance of each particle.
     *
     * @param endSpinVar The end spin variance of each particle.
     */
    void setEndSpinVar(float endSpinVar) { _endSpinVar = endSpinVar; }

    /** Gets the emission rate of the particles.
     *
     * @return The emission rate of the particles.
     */
    float getEmissionRate() const { return _emissionRate; }
    /** Sets the emission rate of the particles.
     *
     * @param rate The emission rate of the particles.
     */
    void setEmissionRate(float rate) { _emissionRate = rate; }

    /** Gets the maximum particles of the system.
     *
     * @return The maximum particles of the system.
     */
    virtual int getTotalParticles() const;
    /** Sets the maximum particles of the system.
     *
     * @param totalParticles The maximum particles of the system.
     */
    virtual void setTotalParticles(int totalParticles);

    /** does the alpha value modify color */
    void setOpacityModifyRGB(bool opacityModifyRGB) override { _opacityModifyRGB = opacityModifyRGB; }
    bool isOpacityModifyRGB() const override { return _opacityModifyRGB; }
    CC_DEPRECATED_ATTRIBUTE bool getOpacityModifyRGB() const { return isOpacityModifyRGB(); }
    
    /** Gets the particles movement type: Free or Grouped.
     @since v0.8
     *
     * @return The particles movement type.
     */
    PositionType getPositionType() const { return _positionType; }
    /** Sets the particles movement type: Free or Grouped.
    @since v0.8
     *
     * @param type The particles movement type.
     */
    void setPositionType(PositionType type) { _positionType = type; }
    
    // Overrides
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    virtual Texture2D* getTexture() const override;
    virtual void setTexture(Texture2D *texture) override;
    /**
    *@code
    *When this function bound into js or lua,the parameter will be changed
    *In js: var setBlendFunc(var src, var dst)
    *In lua: local setBlendFunc(local src, local dst)
    *@endcode
    */
    virtual void setBlendFunc(const BlendFunc &blendFunc) override;
    /**
    * @js NA
    * @lua NA
    */
    virtual const BlendFunc &getBlendFunc() const override;

    /* BPC_PATCH start */
    bool reinitWithDictionary(cocos2d::ValueMap &dictionary);
    /* BPC_PATCH end */
    
    const std::string& getResourceFile() const { return _plistFile; }

    /// @{
    /// @name implement Playable Protocol
    virtual void start() override;
    virtual void stop() override;
    /// @} end of PlayableProtocol
    
    void setSourcePositionCompatible(bool sourcePositionCompatible) { _sourcePositionCompatible = sourcePositionCompatible; }
    bool isSourcePositionCompatible() const { return _sourcePositionCompatible; }
    
CC_CONSTRUCTOR_ACCESS:
    /**
     * @js ctor
     */
    ParticleSystem();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~ParticleSystem();

    /** initializes a ParticleSystem*/
    bool init() override;
    /** initializes a ParticleSystem from a plist file.
     This plist files can be created manually or with Particle Designer:
     http://particledesigner.71squared.com/
     @since v0.99.3
     */
    bool initWithFile(const std::string& plistFile);
    
    /** initializes a QuadParticleSystem from a Dictionary.
     @since v0.99.3
     */
    bool initWithDictionary(ValueMap& dictionary);
    
    /** initializes a particle system from a NSDictionary and the path from where to load the png
     @since v2.1
     */
    bool initWithDictionary(ValueMap& dictionary, const std::string& dirname);
    
    //! Initializes a system with a fixed number of particles
    virtual bool initWithTotalParticles(int numberOfParticles);
    
    /** Are the emissions paused
     @return True if the emissions are paused, else false
     */
    virtual bool isPaused() const;
    
    /* Pause the emissions*/
    virtual void pauseEmissions();
    
    /* UnPause the emissions*/
    virtual void resumeEmissions();

protected:
    virtual void updateBlendFunc();
    
private:
    friend class EngineDataManager;
    /** Internal use only, it's used by EngineDataManager class for Android platform */
    static void setTotalParticleCountFactor(float factor);
    
protected:

    /** whether or not the particles are using blend additive.
     If enabled, the following blending function will be used.
     @code
     source blend function = GL_SRC_ALPHA;
     dest blend function = GL_ONE;
     @endcode
     */
    bool _isBlendAdditive;

    /** whether or not the node will be auto-removed when it has no particles left.
     By default it is false.
     @since v0.8
     */
    bool _isAutoRemoveOnFinish;
    
    std::function<void(ParticleSystem*)> _doneCallback {nullptr};

    std::string _plistFile;
    //! time elapsed since the start of the system (in seconds)
    float _elapsed;

    // Different modes
    //! Mode A:Gravity + Tangential Accel + Radial Accel
    struct {
        /** Gravity value. Only available in 'Gravity' mode. */
        Vec2 gravity;
        /** speed of each particle. Only available in 'Gravity' mode.  */
        float speed;
        /** speed variance of each particle. Only available in 'Gravity' mode. */
        float speedVar;
        /** tangential acceleration of each particle. Only available in 'Gravity' mode. */
        float tangentialAccel;
        /** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
        float tangentialAccelVar;
        /** radial acceleration of each particle. Only available in 'Gravity' mode. */
        float radialAccel;
        /** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
        float radialAccelVar;
        /** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
        bool rotationIsDir;
    } modeA;

    //! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
    struct {
        /** The starting radius of the particles. Only available in 'Radius' mode. */
        float startRadius;
        /** The starting radius variance of the particles. Only available in 'Radius' mode. */
        float startRadiusVar;
        /** The ending radius of the particles. Only available in 'Radius' mode. */
        float endRadius;
        /** The ending radius variance of the particles. Only available in 'Radius' mode. */
        float endRadiusVar;
        /** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
        float rotatePerSecond;
        /** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
        float rotatePerSecondVar;
    } modeB;
    
    //particle data
    ParticleData _particleData;

    //Emitter name
    std::string _configName;

    // color modulate
    //    BOOL colorModulate;

    //! How many particles can be emitted per second
    float _emitCounter;

    // Optimization
    //CC_UPDATE_PARTICLE_IMP    updateParticleImp;
    //SEL                        updateParticleSel;

    /** weak reference to the SpriteBatchNode that renders the Sprite */
    ParticleBatchNode* _batchNode;

    // index of system in batch node array
    int _atlasIndex;

    //true if scaled or rotated
    bool _transformSystemDirty;
    // Number of allocated particles
    int _allocatedParticles;

    /** Is the emitter active */
    bool _isActive;
    
    /** Quantity of particles that are being simulated at the moment */
    int _particleCount;
    /** The factor affects the total particle count, its value should be 0.0f ~ 1.0f, default 1.0f*/
    static float __totalParticleCountFactor;
    
    /** How many seconds the emitter will run. -1 means 'forever' */
    float _duration;
    /** sourcePosition of the emitter */
    Vec2 _sourcePosition;
    /** Position variance of the emitter */
    Vec2 _posVar;
    /** life, and life variation of each particle */
    float _life;
    /** life variance of each particle */
    float _lifeVar;
    /** angle and angle variation of each particle */
    float _angle;
    /** angle variance of each particle */
    float _angleVar;

    /** Switch between different kind of emitter modes:
     - kParticleModeGravity: uses gravity, speed, radial and tangential acceleration
     - kParticleModeRadius: uses radius movement + rotation
     */
    Mode _emitterMode;

    /** start size in pixels of each particle */
    float _startSize;
    /** size variance in pixels of each particle */
    float _startSizeVar;
    /** end size in pixels of each particle */
    float _endSize;
    /** end size variance in pixels of each particle */
    float _endSizeVar;
    /** start color of each particle */
    Color4F _startColor;
    /** start color variance of each particle */
    Color4F _startColorVar;
    /** end color and end color variation of each particle */
    Color4F _endColor;
    /** end color variance of each particle */
    Color4F _endColorVar;
    //* initial angle of each particle
    float _startSpin;
    //* initial angle of each particle
    float _startSpinVar;
    //* initial angle of each particle
    float _endSpin;
    //* initial angle of each particle
    float _endSpinVar;
    /** emission rate of the particles */
    float _emissionRate;
    /** maximum particles of the system */
    int _totalParticles;
    /** conforms to CocosNodeTexture protocol */
    Texture2D* _texture;
    /** conforms to CocosNodeTexture protocol */
    BlendFunc _blendFunc;
    /** does the alpha value modify color */
    bool _opacityModifyRGB;
    /** does FlippedY variance of each particle */
    int _yCoordFlipped;


    /** particles movement type: Free or Grouped
     @since v0.8
     */
    PositionType _positionType;
    
    /** is the emitter paused */
    bool _paused;
    
    /** is sourcePosition compatible */
    bool _sourcePositionCompatible;

    static Vector<ParticleSystem*> __allInstances;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(ParticleSystem);
};

// end of _2d group
/// @}

NS_CC_END

#endif //__CCPARTICLE_SYSTEM_H__
