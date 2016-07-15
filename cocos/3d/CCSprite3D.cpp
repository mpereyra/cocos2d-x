/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.

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

#include "3d/CCSprite3D.h"
#include "3d/CCObjLoader.h"
#include "3d/CCMeshSkin.h"
#include "3d/CCBundle3D.h"
#include "3d/CCSprite3DMaterial.h"
#include "3d/CCAttachNode.h"
#include "3d/CCMesh.h"

#include "base/CCDirector.h"
#include "base/CCAsyncTaskPool.h"
#include "2d/CCLight.h"
#include "2d/CCCamera.h"
#include "base/ccMacros.h"
#include "platform/CCPlatformMacros.h"
#include "platform/CCFileUtils.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgramCache.h"

#include "deprecated/CCString.h" // For StringUtils::format

NS_CC_BEGIN

std::string s_attributeNames[] = {GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::ATTRIBUTE_NAME_TEX_COORD1, GLProgram::ATTRIBUTE_NAME_TEX_COORD2,GLProgram::ATTRIBUTE_NAME_TEX_COORD3,GLProgram::ATTRIBUTE_NAME_NORMAL, GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT, GLProgram::ATTRIBUTE_NAME_BLEND_INDEX};

Sprite3D* Sprite3D::create()
{
    //
    auto sprite = new (std::nothrow) Sprite3D();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite3D* Sprite3D::create(const std::string &modelPath)
{
    if (modelPath.length() < 4)
        CCASSERT(false, "invalid filename for Sprite3D");
    
    auto sprite = new (std::nothrow) Sprite3D();
    if (sprite && sprite->initWithFile(modelPath))
    {
        sprite->_contentSize = sprite->getBoundingBox().size;
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
Sprite3D* Sprite3D::create(const std::string &modelPath, const std::string &texturePath)
{
    auto sprite = create(modelPath);
    if (sprite)
    {
        sprite->setTexture(texturePath);
    }
    
    return sprite;
}

void Sprite3D::createAsync(const std::string &modelPath, const std::function<void(Sprite3D*, void*)>& callback, void* callbackparam)
{
    createAsync(modelPath, "", callback, callbackparam);
}

void Sprite3D::createAsync(const std::string &modelPath, const std::string &texturePath, const std::function<void(Sprite3D*, void*)>& callback, void* callbackparam)
{
    Sprite3D *sprite = new (std::nothrow) Sprite3D();
    if (sprite->loadFromCache(modelPath))
    {
        sprite->autorelease();
        if (!texturePath.empty())
            sprite->setTexture(texturePath);
        callback(sprite, callbackparam);
        return;
    }
    
    sprite->_asyncLoadParam.afterLoadCallback = callback;
    sprite->_asyncLoadParam.texPath = texturePath;
    sprite->_asyncLoadParam.modlePath = modelPath;
    sprite->_asyncLoadParam.callbackParam = callbackparam;
    sprite->_asyncLoadParam.materialdatas = new (std::nothrow) MaterialDatas();
    sprite->_asyncLoadParam.meshdatas = new (std::nothrow) MeshDatas();
    sprite->_asyncLoadParam.nodeDatas = new (std::nothrow) NodeDatas();
    AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, CC_CALLBACK_1(Sprite3D::afterAsyncLoad, sprite), (void*)(&sprite->_asyncLoadParam), [sprite]()
    {
        sprite->_asyncLoadParam.result = sprite->loadFromFile(sprite->_asyncLoadParam.modlePath, sprite->_asyncLoadParam.nodeDatas, sprite->_asyncLoadParam.meshdatas, sprite->_asyncLoadParam.materialdatas);
    });
    
}

void Sprite3D::afterAsyncLoad(void* param)
{
    Sprite3D::AsyncLoadParam* asyncParam = (Sprite3D::AsyncLoadParam*)param;
    autorelease();
    if (asyncParam)
    {
        if (asyncParam->result)
        {
            _meshes.clear();
            _meshVertexDatas.clear();
            
            removeAllAttachNode();
            
            //create in the main thread
            auto& meshdatas = asyncParam->meshdatas;
            auto& materialdatas = asyncParam->materialdatas;
            auto&   nodeDatas = asyncParam->nodeDatas;
            if (initFrom(*nodeDatas, *meshdatas, *materialdatas))
            {
                auto spritedata = Sprite3DCache::getInstance()->getSpriteData(asyncParam->modlePath);
                if (spritedata == nullptr)
                {
                    //add to cache
                    auto data = new (std::nothrow) Sprite3DCache::Sprite3DData();
                    data->materialdatas.reset(materialdatas);
                    data->nodedatas.reset(nodeDatas);
                    data->meshVertexDatas = _meshVertexDatas;
                    for (const auto mesh : _meshes) {
                        data->glProgramStates.pushBack(mesh->getGLProgramState());
                    }
                    
                    Sprite3DCache::getInstance()->addSprite3DData(asyncParam->modlePath, data);
                    if (meshdatas) {
                        delete meshdatas;
                        meshdatas = nullptr;
                    }
                    materialdatas = nullptr;
                    nodeDatas = nullptr;
                }
            }
            delete meshdatas;
            delete materialdatas;
            delete nodeDatas;
            
            if (asyncParam->texPath != "")
            {
                setTexture(asyncParam->texPath);
            }
        }
        else
        {
            CCLOG("file load failed: %s ", asyncParam->modlePath.c_str());
        }
        asyncParam->afterLoadCallback(this, asyncParam->callbackParam);
    }
}

bool Sprite3D::loadFromCache(const std::string& path)
{
    auto spritedata = Sprite3DCache::getInstance()->getSpriteData(path);
    if (spritedata)
    {
        for (auto it : spritedata->meshVertexDatas) {
            _meshVertexDatas.pushBack(it);
        }
        
        // BPC PATCH BEGIN
        if(_retainSkeleton == false) {
            CC_SAFE_RELEASE_NULL(_skeleton);
            _skeleton = Skeleton3D::create(spritedata->nodedatas->skeleton);
            CC_SAFE_RETAIN(_skeleton);
        }
        // BPC PATCH END
        
        for(const auto& it : spritedata->nodedatas->nodes)
        {
            if(it)
            {
                createNode(it, this, *(spritedata->materialdatas), spritedata->nodedatas->nodes.size() == 1);
            }
        }
        
        for(const auto& it : spritedata->nodedatas->skeleton)
        {
            if(it)
            {
                createAttachSprite3DNode(it,*(spritedata->materialdatas));
            }
        }
        
        for (ssize_t i = 0; i < _meshes.size(); i++) {
            _meshes.at(i)->setGLProgramState(spritedata->glProgramStates.at(i));
        }
        return true;
    }
    
    return false;
}

bool Sprite3D::loadFromFile(const std::string& path, NodeDatas* nodedatas, MeshDatas* meshdatas,  MaterialDatas* materialdatas)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    
    std::string ext = path.substr(path.length() - 4, 4);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    if (ext == ".obj")
    {
        return Bundle3D::loadObj(*meshdatas, *materialdatas, *nodedatas, fullPath);
    }
    else if (ext == ".c3b" || ext == ".c3t")
    {
        //load from .c3b or .c3t
        auto bundle = Bundle3D::createBundle();
        if (!bundle->load(fullPath))
        {
            Bundle3D::destroyBundle(bundle);
            return false;
        }
        
        auto ret = bundle->loadMeshDatas(*meshdatas)
            && bundle->loadMaterials(*materialdatas) && bundle->loadNodes(*nodedatas);
        Bundle3D::destroyBundle(bundle);
        
        return ret;
    }
    return false;
}

Sprite3D::Sprite3D()
: _skeleton(nullptr)
, _blend(BlendFunc::ALPHA_NON_PREMULTIPLIED)
, _aabbDirty(true)
, _lightMask(-1)
, _shaderUsingLight(false)
, _forceDepthWrite(false)
, _forceCullFace(false)
{
}

Sprite3D::~Sprite3D()
{
    _meshes.clear();
    _meshVertexDatas.clear();
    CC_SAFE_RELEASE_NULL(_skeleton);
    removeAllAttachNode();
}

bool Sprite3D::init()
{
    if(Node::init())
    {
        return true;
    }
    return false;
}

bool Sprite3D::initWithFile(const std::string &path)
{
    _meshes.clear();
    _meshVertexDatas.clear();
    
    removeAllAttachNode();
    
    if (loadFromCache(path))
        return true;
    
    std::unique_ptr<MeshDatas> meshdatas{ new MeshDatas() };
    std::unique_ptr<MaterialDatas> materialdatas{ new MaterialDatas() };
    std::unique_ptr<NodeDatas> nodeDatas{ new NodeDatas() };
    if (loadFromFile(path, nodeDatas.get(), meshdatas.get(), materialdatas.get()))
    {
        if (initFrom(*nodeDatas, *meshdatas, *materialdatas))
        {
            //add to cache
            auto data = new (std::nothrow) Sprite3DCache::Sprite3DData();
            data->materialdatas = std::move(materialdatas);
            data->nodedatas = std::move(nodeDatas);
            data->meshVertexDatas = _meshVertexDatas;
            for (const auto mesh : _meshes) {
                data->glProgramStates.pushBack(mesh->getGLProgramState());
            }

            Sprite3DCache::getInstance()->addSprite3DData(path, data);
            return true;
        }
    }

    return false;
}

bool Sprite3D::initFrom(const NodeDatas& nodeDatas, const MeshDatas& meshdatas, const MaterialDatas& materialdatas)
{
    for(const auto& it : meshdatas.meshDatas)
    {
        if(it)
        {
//            Mesh* mesh = Mesh::create(*it);
//            _meshes.pushBack(mesh);
            auto meshvertex = MeshVertexData::create(*it);
            _meshVertexDatas.pushBack(meshvertex);
        }
    }
    
    // BPC PATCH BEGIN
    if(_retainSkeleton == false) {
        CC_SAFE_RELEASE_NULL(_skeleton);
         _skeleton = Skeleton3D::create(nodeDatas.skeleton);
        CC_SAFE_RETAIN(_skeleton);
    }
    // BPC PATCH END
    
    for(const auto& it : nodeDatas.nodes)
    {
        if(it)
        {
            createNode(it, this, materialdatas, nodeDatas.nodes.size() == 1);
        }
    }
    for(const auto& it : nodeDatas.skeleton)
    {
        if(it)
        {
             createAttachSprite3DNode(it,materialdatas);
        }
    }
    genGLProgramState();
    
    return true;
}
Sprite3D* Sprite3D::createSprite3DNode(NodeData* nodedata,ModelData* modeldata,const MaterialDatas& matrialdatas)
{
    auto sprite = new (std::nothrow) Sprite3D();
    if (sprite)
    {
        sprite->setName(nodedata->id);
        auto mesh = Mesh::create(nodedata->id, getMeshIndexData(modeldata->subMeshId));
        if (modeldata->matrialId == "" && matrialdatas.materials.size())
        {
            const NTextureData* textureData = matrialdatas.materials[0].getTextureData(NTextureData::Usage::Diffuse);
            if (!textureData->filename.empty())
                mesh->setTexture(textureData->filename);
        }
        else
        {
            const NMaterialData*  materialData=matrialdatas.getMaterialData(modeldata->matrialId);
            if(materialData)
            {
                const NTextureData* textureData = materialData->getTextureData(NTextureData::Usage::Diffuse);
                if(textureData && !textureData->filename.empty())
                {
                    auto tex = Director::getInstance()->getTextureCache()->addImage(textureData->filename);
                    if(tex)
                    {
                        Texture2D::TexParams    texParams;
                        texParams.minFilter = GL_LINEAR;
                        texParams.magFilter = GL_LINEAR;
                        texParams.wrapS = textureData->wrapS;
                        texParams.wrapT = textureData->wrapT;
                        tex->setTexParameters(texParams);
                        mesh->setTexture(tex);
                        mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);
                    }

                }
            }
        }
        sprite->setAdditionalTransform(&nodedata->transform);
        sprite->addMesh(mesh);
        sprite->autorelease();
        sprite->genGLProgramState(); 
    }
    return   sprite;
}
void Sprite3D::createAttachSprite3DNode(NodeData* nodedata,const MaterialDatas& matrialdatas)
{
    for(const auto& it : nodedata->modelNodeDatas)
    {
        if(it && getAttachNode(nodedata->id))
        {
            auto sprite = createSprite3DNode(nodedata,it,matrialdatas);
            if (sprite)
            {
                getAttachNode(nodedata->id)->addChild(sprite);
            } 
        }
    }
    for(const auto& it : nodedata->children)
    {
        createAttachSprite3DNode(it,matrialdatas);
    }
}
void Sprite3D::genGLProgramState(bool useLight)
{
    _shaderUsingLight = useLight;
    
    std::unordered_map<const MeshVertexData*, GLProgramState*> glProgramestates;
    for(auto& mesh : _meshVertexDatas)
    {
        bool textured = mesh->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        bool hasSkin = mesh->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_BLEND_INDEX)
        && mesh->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT);
        bool hasNormal = mesh->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_NORMAL);
        
        GLProgram* glProgram = nullptr;
        const char* shader = nullptr;
        if(textured)
        {
            if (hasSkin)
            {
                if (hasNormal && _shaderUsingLight)
                    shader = GLProgram::SHADER_3D_SKINPOSITION_NORMAL_TEXTURE;
                else
                    shader = GLProgram::SHADER_3D_SKINPOSITION_TEXTURE;
            }
            else
            {
                if (hasNormal && _shaderUsingLight)
                    shader = GLProgram::SHADER_3D_POSITION_NORMAL_TEXTURE;
                else
                    shader = GLProgram::SHADER_3D_POSITION_TEXTURE;
            }
        }
        else
        {
            shader = GLProgram::SHADER_3D_POSITION;
        }
        if (shader)
            glProgram = GLProgramCache::getInstance()->getGLProgram(shader);
        
        auto programstate = GLProgramState::create(glProgram);
        
        // BPC PATCH - Gave some fragment shaders an alpha uniform, initializing it here.
        programstate->setUniformFloat("alpha", 1.0);
        //END PATCH
        
        long offset = 0;
        auto attributeCount = mesh->getMeshVertexAttribCount();
        for (auto k = 0; k < attributeCount; k++) {
            auto meshattribute = mesh->getMeshVertexAttrib(k);
            programstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                                 meshattribute.size,
                                                 meshattribute.type,
                                                 GL_FALSE,
                                                 mesh->getVertexBuffer()->getSizePerVertex(),
                                                 (GLvoid*)offset);
            offset += meshattribute.attribSizeBytes;
        }
        
        glProgramestates[mesh] = programstate;
    }
    
    for (auto& it : _meshes) {
        auto glProgramState = glProgramestates[it->getMeshIndexData()->getMeshVertexData()];
        it->setGLProgramState(glProgramState);
    }
}

void Sprite3D::createNode(NodeData* nodedata, Node* root, const MaterialDatas& matrialdatas, bool singleSprite)
{
    Node* node=nullptr;
    for(const auto& it : nodedata->modelNodeDatas)
    {
        if(it)
        {
            if(it->bones.size() > 0 || singleSprite)
            {
                auto mesh = Mesh::create(nodedata->id, getMeshIndexData(it->subMeshId));
                if(mesh)
                {
                    _meshes.pushBack(mesh);
                    if (_skeleton && it->bones.size())
                    {
                        auto skin = MeshSkin::create(_skeleton, it->bones, it->invBindPose);
                        mesh->setSkin(skin);
                    }
                    mesh->_visibleChanged = std::bind(&Sprite3D::onAABBDirty, this);

                    if (it->matrialId == "" && matrialdatas.materials.size())
                    {
                        const NTextureData* textureData = matrialdatas.materials[0].getTextureData(NTextureData::Usage::Diffuse);
                        mesh->setTexture(textureData->filename);
                    }
                    else
                    {
                        const NMaterialData*  materialData=matrialdatas.getMaterialData(it->matrialId);
                        if(materialData)
                        {
                            const NTextureData* textureData = materialData->getTextureData(NTextureData::Usage::Diffuse);
                            if(textureData && !textureData->filename.empty())
                            {
                                auto tex = Director::getInstance()->getTextureCache()->addImage(textureData->filename);
                                if(tex)
                                {
                                    Texture2D::TexParams    texParams;
                                    texParams.minFilter = GL_LINEAR;
                                    texParams.magFilter = GL_LINEAR;
                                    texParams.wrapS = textureData->wrapS;
                                    texParams.wrapT = textureData->wrapT;
                                    tex->setTexParameters(texParams);
                                    mesh->setTexture(tex);
                                    mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);
                                }

                            }
                        }
                    }
                }
            }
            else
            {
                auto sprite = createSprite3DNode(nodedata,it,matrialdatas);
                if (sprite)
                {
                    if(root)
                    {
                        root->addChild(sprite);
                    } 
                }
                node=sprite;
            } 
        }
    }
    if(nodedata->modelNodeDatas.size() ==0 )
    {
        node= Node::create();
        if(node)
        {
            node->setName(nodedata->id);
            node->setAdditionalTransform(&nodedata->transform);
            if(root)
            {
                root->addChild(node);
            } 
        }
    }
    for(const auto& it : nodedata->children)
    {
        createNode(it,node, matrialdatas, nodedata->children.size() == 1);
    }
}

MeshIndexData* Sprite3D::getMeshIndexData(const std::string& indexId) const
{
    for (auto it : _meshVertexDatas) {
        auto index = it->getMeshIndexDataById(indexId);
        if (index)
            return index;
    }
    return nullptr;
}

void  Sprite3D::addMesh(Mesh* mesh)
{
    auto meshVertex = mesh->getMeshIndexData()->_vertexData;
    _meshVertexDatas.pushBack(meshVertex);
    _meshes.pushBack(mesh);
}

void Sprite3D::setTexture(const std::string& texFile)
{
    auto tex = Director::getInstance()->getTextureCache()->addImage(texFile);
    setTexture(tex);
}

void Sprite3D::setTexture(Texture2D* texture)
{
    for (auto& state : _meshes) {
        state->setTexture(texture);
    }
}
AttachNode* Sprite3D::getAttachNode(const std::string& boneName)
{
    auto it = _attachments.find(boneName);
    if (it != _attachments.end())
        return it->second;
    
    if (_skeleton)
    {
        auto bone = _skeleton->getBoneByName(boneName);
        if (bone)
        {
            auto attachNode = AttachNode::create(bone);
            addChild(attachNode);
            _attachments[boneName] = attachNode;
            return attachNode;
        }
    }
    
    return nullptr;
}

void Sprite3D::removeAttachNode(const std::string& boneName)
{
    auto it = _attachments.find(boneName);
    if (it != _attachments.end())
    {
        removeChild(it->second);
        _attachments.erase(it);
    }
}

void Sprite3D::removeAllAttachNode()
{
    for (auto& it : _attachments) {
        removeChild(it.second);
    }
    _attachments.clear();
}
#if (!defined NDEBUG) || (defined CC_MODEL_VIEWER) 
//Generate a dummy texture when the texture file is missing
static Texture2D * getDummyTexture()
{
    auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("/dummyTexture");
    if(!texture)
    {
        unsigned char data[] ={255,0,0,255};//1*1 pure red picture
        Image * image =new (std::nothrow) Image();
        image->initWithRawData(data,sizeof(data),1,1,sizeof(unsigned char));
        texture=Director::getInstance()->getTextureCache()->addImage(image,"/dummyTexture");
        image->release();
    }
    return texture;
}
#endif

void Sprite3D::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    flags |= FLAGS_RENDER_AS_3D;
    
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    bool visibleByCamera = isVisitableByVisitingCamera();
    int i = 0;
    
    if(!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            auto node = _children.at(i);
            
            if (node && node->getLocalZOrder() < 0)
                node->visit(renderer, _modelViewTransform, flags);
            else
                break;
        }
        // self draw
        if (visibleByCamera)
            this->draw(renderer, _modelViewTransform, flags);
        
        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            (*it)->visit(renderer, _modelViewTransform, flags);
    }
    else if (visibleByCamera)
    {
        this->draw(renderer, _modelViewTransform, flags);
    }
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

/* BPC Patch */
void Sprite3D::setGlobalZOrder(float globalZOrder) {
    Node::setGlobalZOrder(globalZOrder);
}
/* BPC Patch */

void Sprite3D::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
    // camera clipping
    if(m_allowCulling && !Camera::getVisitingCamera()->isVisibleInFrustum(&this->getAABB()))
        return;
#endif
    
    if (_skeleton)
        _skeleton->updateBoneMatrix();
    
    Color4F color(getDisplayedColor());
    color.a = getDisplayedOpacity() / 255.0f;
    
    //check light and determine the shader used
    const auto& lights = Director::getInstance()->getRunningScene()->getLights();
    bool usingLight = false;
    for (const auto light : lights) {
        usingLight = ((unsigned int)light->getLightFlag() & _lightMask) > 0;
        if (usingLight)
            break;
    }
    if (usingLight != _shaderUsingLight)
        genGLProgramState(usingLight);
    
    int i = 0;
    for (auto& mesh : _meshes) {
        if (!mesh->isVisible())
        {
            i++;
            continue;
        }
        auto programstate = mesh->getGLProgramState();
        auto& meshCommand = mesh->getMeshCommand();

#if (!defined NDEBUG) || (defined CC_MODEL_VIEWER) 
        GLuint textureID = 0;
        if(mesh->getTexture())
        {
            textureID = mesh->getTexture()->getName();
        }else
        { //let the mesh use a dummy texture instead of the missing or crashing texture file
            auto texture = getDummyTexture();
            mesh->setTexture(texture);
            textureID = texture->getName();
        }

#else
        GLuint textureID = mesh->getTexture() ? mesh->getTexture()->getName() : 0;
#endif

        bool isTransparent = (mesh->_isTransparent || color.a < 1.f);

        /* BPC Patch */
        float globalZ = _globalZOrder;
        if (mesh->getGlobalZ() != std::numeric_limits<float>::max())
            globalZ = mesh->getGlobalZ();

        if (isTransparent) {
            flags |= Node::FLAGS_RENDER_AS_3D;

            // If globalZ is positive, then the caller wants to force frontmost rendering, and we
            // should use their specified global Z order. Otherwise, we force-set it to zero since
            // we're rendering a transparent mesh.
            // FIXME: Is this really necessary? It seems we do ordering of the triangles before they
            // hit the GL command queue.
            if (globalZ <= 0.0f) globalZ = 0.0f;
        }

        meshCommand.init(globalZ, textureID, programstate, mesh->getBlendFunc(), mesh->getVertexBuffer(), mesh->getIndexBuffer(), mesh->getPrimitiveType(), mesh->getIndexFormat(), mesh->getIndexCount(), transform, flags);
        /* BPC Patch */
        
        meshCommand.setLightMask(_lightMask);
        
// BPC PATCH BEGIN
        // BRING ME THE EPIDERMAL TISSUE DISRUPTER
        meshCommand.setShouldClip(_shouldClip);
        if (_shouldClip) {
            meshCommand.setGlBounds(_clippingRect);
        }
        
        meshCommand.setTransparent(isTransparent);
        bool shouldWriteDepth = mesh->boolFromWriteMode(mesh->getDepthWriteMode());
        bool cullFaceEnabled = mesh->boolFromWriteMode(mesh->getCullFaceMode());
        meshCommand.setDepthWriteEnabled(shouldWriteDepth);
        meshCommand.setCullFaceEnabled(cullFaceEnabled);
// BPC PATCH END

        auto skin = mesh->getSkin();
        if (skin)
        {
            meshCommand.setMatrixPaletteSize((int)skin->getMatrixPaletteSize());
            meshCommand.setMatrixPalette(skin->getMatrixPalette());
        }
        //support tint and fade
        meshCommand.setDisplayColor(Vec4(color.r, color.g, color.b, color.a));
        
        /*
        if (_forceDepthWrite)
        {
            meshCommand.setDepthWriteEnabled(true);
        }
        meshCommand.setDepthWriteEnabled(!isTransparent || _forceDepthWrite);
        meshCommand.setTransparent(isTransparent);
        meshCommand.setCullFaceEnabled(!isTransparent || _forceCullFace);
         */
        
#ifndef NDEBUG
        const std::string& name = _name + "." + mesh->getName();
        meshCommand.setName(name);
#endif
        renderer->addCommand(&meshCommand);
    }
}

void Sprite3D::setGLProgramState(GLProgramState *glProgramState)
{
    Node::setGLProgramState(glProgramState);
    for (auto& state : _meshes) {
        state->setGLProgramState(glProgramState);
    }
}
void Sprite3D::setGLProgram(GLProgram *glprogram)
{
    Node::setGLProgram(glprogram);
    setGLProgramState(getGLProgramState());
}

void Sprite3D::setBlendFunc(const BlendFunc &blendFunc)
{
    if(_blend.src != blendFunc.src || _blend.dst != blendFunc.dst)
    {
        _blend = blendFunc;
        for(auto& state : _meshes)
        {
            state->setBlendFunc(blendFunc);
        }
    }
}

const BlendFunc& Sprite3D::getBlendFunc() const
{
    return _blend;
}

const AABB& Sprite3D::getAABB() const
{
    Mat4 nodeToWorldTransform(getNodeToWorldTransform());
    
    // If nodeToWorldTransform matrix isn't changed, we don't need to transform aabb.
    if (!_aabbDirty && memcmp(_nodeToWorldTransform.m, nodeToWorldTransform.m, sizeof(Mat4)) == 0)
    {
        return _aabb;
    }
    else
    {
        _aabb.reset();
        
        // Merge mesh and child aabb's in parent space
        for (const auto& mesh : _meshes) {
            if (mesh->isVisible())
                _aabb.merge(mesh->getAABB());
        }
        
        for (auto const & child : _children) {
            _aabb.merge(child->getNodeToParentAABB());
        }
        
        /** BPC PATCH BEGIN **/
        // Don't want to keep an invalid boudning box just because we have no visible mesh!
        if(_aabb.isEmpty()) {
            _aabb.set({0,0,0}, {0,0,0});
        }
        /** BPC PATCH END **/
        
        // convert to world space
        _aabb.transform(nodeToWorldTransform);
        _nodeToWorldTransform = nodeToWorldTransform;
        _aabbDirty = false;
    }
    
    return _aabb;
}

/** BPC PATCH BEGIN **/
AABB Sprite3D::skinAABB(Mesh const * mesh) const{
    AABB aabb = mesh->getAABB();
    if(mesh->getSkin() && mesh->getSkin()->getBoneCount() > 0){
        // bone > 0 implies getMatrixPaletteSize() >= 3 ^
        Vec4 * mp = mesh->getSkin()->getMatrixPalette();
        Mat4 rootBoneTransform(
                               1, 0, 0, mp[0].w,
                               0, 1, 0, mp[0+1].w,
                               0, 0, 1, mp[0+2].w,
                               0, 0, 0, 1
                               );
        aabb.transform(rootBoneTransform);
    }
    return aabb;
}

const AABB& Sprite3D::getNodeToParentAABB(const std::vector<std::string>& excludeMeshes, bool force) const {
    
    // If nodeToWorldTransform matrix isn't changed and we are querying the same set of meshes as before, we don't need to transform aabb.
    _nodeToParentAABBDirty = m_skinAABB ? true : _nodeToParentAABBDirty;
    if (!force && !_nodeToParentAABBDirty
        && _nodeToParentExcludeMeshes.size() == excludeMeshes.size()
        && std::is_permutation(_nodeToParentExcludeMeshes.begin(), _nodeToParentExcludeMeshes.end(), excludeMeshes.begin()))
    {
        return _nodeToParentAABB;
    }
    else
    {
        _nodeToParentAABB.reset();
        
        // Merge mesh and child aabb's in parent space
        for (const auto& mesh : _meshes) {
            if (mesh->isVisible()
                && std::none_of(excludeMeshes.begin(), excludeMeshes.end(), [&mesh](const std::string& meshName){return meshName == mesh->getName();})){
                auto mb = m_skinAABB ? skinAABB(mesh) : mesh->getAABB();
                _nodeToParentAABB.merge(mb);
            }
        }
        
        for(auto const & child : _children) {
            if(child->isVisible()) {
                _nodeToParentAABB.merge(child->getNodeToParentAABB(excludeMeshes, force));
            }
        }
        
        // convert to parent space
        if (!_nodeToParentAABB.isEmpty()) {
            _nodeToParentAABB.transform(getNodeToParentTransform());
            _nodeToParentAABBDirty = false;
            _nodeToParentExcludeMeshes = excludeMeshes;
        }
    }
    
    return _nodeToParentAABB;
}
/** BPC PATCH END **/

Action* Sprite3D::runAction(Action *action)
{
    setForceDepthWrite(true);
    return Node::runAction(action);
}

Rect Sprite3D::getBoundingBox() const
{
    AABB aabb = getAABB();
    Rect ret(aabb._min.x, aabb._min.y, (aabb._max.x - aabb._min.x), (aabb._max.y - aabb._min.y));
    return ret;
}

void Sprite3D::setCullFace(GLenum cullFace)
{
    for (auto& it : _meshes) {
        it->getMeshCommand().setCullFace(cullFace);
    }
}


void Sprite3D::setCullFaceEnabled(bool enabled){
    //no op use the other one
}
void Sprite3D::setCullFaceEnabled(GLWriteMode mode)
{
    for (auto& it : _meshes) {
        it->setCullFaceMode(mode);
    }
}

Mesh* Sprite3D::getMeshByIndex(int index) const
{
    CCASSERT(index < _meshes.size(), "invald index");
    return _meshes.at(index);
}

/**get Mesh by Name */
Mesh* Sprite3D::getMeshByName(const std::string& name) const
{
    for (const auto& it : _meshes) {
        if (it->getName() == name)
            return it;
    }
    return nullptr;
}

std::vector<Mesh*> Sprite3D::getMeshArrayByName(const std::string& name) const
{
    std::vector<Mesh*> meshes;
    for (const auto& it : _meshes) {
        if (it->getName() == name)
            meshes.push_back(it);
    }
    return meshes;
}

MeshSkin* Sprite3D::getSkin() const
{
    for (const auto& it : _meshes) {
        if (it->getSkin())
            return it->getSkin();
    }
    return nullptr;
}

/* BPC PATCH BEGIN */


void Sprite3D::getSprite3DRecursive(Node* parent, std::set<Sprite3D*>& sprites){
    auto s3d = dynamic_cast<Sprite3D*>(parent);
    if(s3d){
        sprites.insert(s3d);
    }
    
    for(auto c : parent->getChildren()){
        getSprite3DRecursive(c, sprites);
    }
}

void Sprite3D::setDepthTestEnabled(bool enabled, bool recursive) {
    for(auto mesh : _meshes) {
        mesh->getMeshCommand().setDepthTestEnabled(enabled);
    }
    if(recursive == false)
        return;

    for(auto child : _children) {
        Sprite3D* sprite3D = dynamic_cast<Sprite3D*>(child);
        if(sprite3D) {
            sprite3D->setDepthTestEnabled(enabled);
        }
    }
}

void Sprite3D::setForceDepthWrite(bool enabled, bool recursive) {
    if(!enabled){
        return; //no-op
    }
    setDepthWriteEnabled(GLWriteMode::AlwaysOn, recursive);
}

void Sprite3D::setDepthWriteEnabled(GLWriteMode mode, bool recursive) {
    for(auto mesh : _meshes) {
        mesh->setDepthWriteMode(mode);
    }
    if(recursive == false)
        return;

    for(auto child : _children) {
        Sprite3D* sprite3D = dynamic_cast<Sprite3D*>(child);
        if(sprite3D) {
            sprite3D->setDepthWriteEnabled(mode);
        }
    }
}

void Sprite3D::setForceCullFace(bool enabled, bool recursive) {
    _forceCullFace = enabled;
    
    if(!enabled){
        return; //no-op
    }
    
    for(auto mesh : _meshes){
        mesh->setCullFaceMode(GLWriteMode::AlwaysOn);
    }
    
    if(recursive) {
        std::set<Sprite3D*> sprites;
        getSprite3DRecursive(this, sprites);
        
        for(auto sprite : sprites) {
            sprite->setForceCullFace(enabled, false);
        }
    }
}
/* BPC PATCH END */

///////////////////////////////////////////////////////////////////////////////////
Sprite3DCache* Sprite3DCache::_cacheInstance = nullptr;
Sprite3DCache* Sprite3DCache::getInstance()
{
    if (_cacheInstance == nullptr)
        _cacheInstance = new (std::nothrow) Sprite3DCache();
    return _cacheInstance;
}
void Sprite3DCache::destroyInstance()
{
    if (_cacheInstance)
    {
        delete _cacheInstance;
        _cacheInstance = nullptr;
    }
}

Sprite3DCache::Sprite3DData* Sprite3DCache::getSpriteData(const std::string& key) const
{
    auto it = _spriteDatas.find(key);
    if (it != _spriteDatas.end())
        return it->second;
    return nullptr;
}

bool Sprite3DCache::addSprite3DData(const std::string& key, Sprite3DCache::Sprite3DData* spritedata)
{
    auto it = _spriteDatas.find(key);
    if (it == _spriteDatas.end())
    {
        _spriteDatas[key] = spritedata;
        return true;
    }
    return false;
}

void Sprite3DCache::removeSprite3DData(const std::string& key)
{
    auto it = _spriteDatas.find(key);
    if (it != _spriteDatas.end())
    {
        delete it->second;
        _spriteDatas.erase(it);
    }
}

void Sprite3DCache::removeAllSprite3DData()
{
    for (auto& it : _spriteDatas) {
        delete it.second;
    }
    _spriteDatas.clear();
}

// BPC PATCH BEGIN
void Sprite3DCache::removeUnusedSprite3DData()
{
    //When things are loaded from cache, they retain all meshVertexData objects, so we can just check the first element.
    for (auto it = _spriteDatas.cbegin(); it != _spriteDatas.cend(); ) {
        if (it->second->meshVertexDatas.empty()) {
            ++it;
            continue;
        }
        
        MeshVertexData* data= it->second->meshVertexDatas.at(0);
        if (data->getReferenceCount() == 1) {
            CCLOG("cocos2d: Sprite3DCache: removing unused Sprite3DData: %s", it->first.c_str());
            
            delete it->second;
            _spriteDatas.erase(it++);
        } else {
            ++it;
        }
    }
}
// BPC PATCH END

Sprite3DCache::Sprite3DCache()
{
    
}
Sprite3DCache::~Sprite3DCache()
{
    removeAllSprite3DData();
}

NS_CC_END
