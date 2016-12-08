//
//  ParticleAssetCreator.h
//  cocos2d_libs
//
//  Created by Harry 😎 on 11/18/16.
//
//

#ifndef ParticleAssetCreator_h
#define ParticleAssetCreator_h

#include "base/CCRef.h"

NS_CC_BEGIN


class Sprite3D;
class Texture2D;

class AssetCreatorDelegate
{
public:
    virtual ~AssetCreatorDelegate() {}
    virtual std::string getSprite3DFilename(const std::string& assetName) = 0;
    virtual Texture2D* createTexture(const std::string& assetName) = 0;
    virtual Sprite3D* createSprite3D(const std::string& assetName) = 0;
};

class CC_DLL ParticleAssetCreator : public Ref, public AssetCreatorDelegate
{
public:
    void setDelegate(AssetCreatorDelegate* delegate);
    
    std::string getSprite3DFilename(const std::string& assetName) override;
    Texture2D* createTexture(const std::string& assetName) override;
    Sprite3D* createSprite3D(const std::string& assetName) override;
    
    static ParticleAssetCreator* getInstance();
    static void destroyInstance();
    
    
private:
    ParticleAssetCreator() = default;
    ~ParticleAssetCreator() = default;
    
    AssetCreatorDelegate* m_delegate = nullptr;
    static ParticleAssetCreator* s_sharedCreator;
};

NS_CC_END

#endif /* ParticleAssetCreator_h */
