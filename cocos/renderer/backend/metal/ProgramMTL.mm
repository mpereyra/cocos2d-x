/****************************************************************************
 Copyright (c) 2018-2019 Xiamen Yaji Software Co., Ltd.

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
 
#include "ProgramMTL.h"
#include "ShaderModuleMTL.h"
#include "base/ccMacros.h"

CC_BACKEND_BEGIN
namespace {
    const std::string metalSpecificDefine = "#define METAL\n";
}

ProgramMTL::ProgramMTL(const std::string& vertexShader, const std::string& fragmentShader)
: Program(vertexShader, fragmentShader)
{
    _vertexShaderModule = static_cast<ShaderModuleMTL*>(ShaderCache::newVertexShaderModule(vertexShader));
    _fragmentShaderModule = static_cast<ShaderModuleMTL*>(ShaderCache::newFragmentShaderModule(fragmentShader));//std::move(metalSpecificDefine + fragmentShader)));

    CC_SAFE_RETAIN(_vertexShaderModule);
    CC_SAFE_RETAIN(_fragmentShaderModule);
}

ProgramMTL::~ProgramMTL()
{
    CC_SAFE_RELEASE(_vertexShaderModule);
    CC_SAFE_RELEASE(_fragmentShaderModule);
}

int ProgramMTL::getAttributeLocation(Attribute name) const
{
    return _vertexShaderModule->getAttributeLocation(name);
}

int ProgramMTL::getAttributeLocation(const std::string &name) const
{
    return _vertexShaderModule->getAttributeLocation(name);
}

UniformLocation ProgramMTL::getUniformLocation(backend::Uniform name) const
{
    UniformLocation uniformLocation;
    auto vsLocation = _vertexShaderModule->getUniformLocation(name);
    auto fsLocation = _fragmentShaderModule->getUniformLocation(name);
    if(vsLocation != -1 && fsLocation != -1)
    {
        uniformLocation.shaderStage = ShaderStage::VERTEX_AND_FRAGMENT;
        uniformLocation.location[0] = vsLocation;
        uniformLocation.location[1] = fsLocation;
    }
    else if( vsLocation != -1)
    {
        uniformLocation.shaderStage = ShaderStage::VERTEX;
        uniformLocation.location[0] = vsLocation;
    }
    else
    {
        uniformLocation.shaderStage = ShaderStage::FRAGMENT;
        uniformLocation.location[1] = fsLocation;
    }
    return uniformLocation;
}

UniformLocation ProgramMTL::getUniformLocation(const std::string& uniform) const
{
    UniformLocation uniformLocation;
    auto vsLocation = _vertexShaderModule->getUniformLocation(uniform);
    auto fsLocation = _fragmentShaderModule->getUniformLocation(uniform);
    if(vsLocation != -1 && fsLocation != -1)
    {
        uniformLocation.shaderStage = ShaderStage::VERTEX_AND_FRAGMENT;
        uniformLocation.location[0] = vsLocation;
        uniformLocation.location[1] = fsLocation;
    }
    else if( vsLocation != -1)
    {
        uniformLocation.shaderStage = ShaderStage::VERTEX;
        uniformLocation.location[0] = vsLocation;
    }
    else
    {
        uniformLocation.shaderStage = ShaderStage::FRAGMENT;
        uniformLocation.location[1] = fsLocation;
    }
    return uniformLocation;
}

int ProgramMTL::getMaxVertexLocation() const
{
    return _vertexShaderModule->getMaxLocation();
}

int ProgramMTL::getMaxFragmentLocation() const
{
    return _fragmentShaderModule->getMaxLocation();
}

const std::unordered_map<std::string, AttributeBindInfo> ProgramMTL::getActiveAttributes() const
{
    return _vertexShaderModule->getAttributeInfo();
}

//const std::vector<char>& ProgramMTL::cloneUniformBuffer(ShaderStage stage) const
//{
//    switch (stage) {
//        case ShaderStage::VERTEX:
//            return _vertexShader->cloneUniformBuffer();
//            break;
//        case ShaderStage::FRAGMENT:
//            return _fragmentShader->cloneUniformBuffer();
//        default:
//            CCASSERT(false, "Invalid shader stage.");
//            break;
//    }
//}

const UniformInfo& ProgramMTL::getActiveUniformInfo(ShaderStage stage, int location) const
{
    switch (stage) {
        case ShaderStage::VERTEX:
            return _vertexShaderModule->getActiveUniform(location);
        case ShaderStage::FRAGMENT:
            return _fragmentShaderModule->getActiveUniform(location);
        default:
            CCASSERT(false, "Invalid shader stage.");
        break;
    }
}


std::size_t ProgramMTL::getUniformBufferSize(ShaderStage stage) const
{
    switch (stage) {
        case ShaderStage::VERTEX:
            return _vertexShaderModule->getUniformBufferSize();
        case ShaderStage::FRAGMENT:
            return _fragmentShaderModule->getUniformBufferSize();
        default:
            CCASSERT(false, "Invalid shader stage.");
            break;
    }
    return 0;
}

const std::unordered_map<std::string, UniformInfo>& ProgramMTL::getAllActiveUniformInfo(ShaderStage stage) const
{
    switch (stage) {
        case ShaderStage::VERTEX:
            return _vertexShaderModule->getAllActiveUniformInfo();
        case ShaderStage::FRAGMENT:
            return _fragmentShaderModule->getAllActiveUniformInfo();
        default:
            CCASSERT(false, "Invalid shader stage.");
            break;
    }
}

CC_BACKEND_END
