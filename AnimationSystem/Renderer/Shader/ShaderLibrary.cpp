#include "ShaderLibrary.hpp"
#include <iostream>

namespace AnimationSystem
{
    using NS::StringEncoding::UTF8StringEncoding;
    ShaderLibrary::~ShaderLibrary()
    {
        for (auto &it : _shaderMap)
        {
            it.second->release();
        }
    }

    MTL::Library *ShaderLibrary::get(const std::string &shaderSourceName)
    {
        auto it = _shaderMap.find(shaderSourceName);
        if (it == _shaderMap.end())
            return nullptr;

        return _shaderMap[shaderSourceName];
    }

    void ShaderLibrary::add(MTL::Library *pLibrary, std::string shaderSourceName, const char *shaderSource)
    {
        MTL::Function *pVertexFn = pLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
        MTL::Function *pFragFn = pLibrary->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));
        _shaderMap.insert(std::make_pair(shaderSourceName, pLibrary));
    }

    MTL::RenderPipelineDescriptor *ShaderLibrary::registerPipelineDescriptor(const std::string &shaderSourceName)
    {
        MTL::Library *pLibrary = get(shaderSourceName);

        MTL::RenderPipelineDescriptor *pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
        MTL::Function *pVertexFn = pLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
        MTL::Function *pFragFn = pLibrary->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

        pDesc->setVertexFunction(pVertexFn);
        pDesc->setFragmentFunction(pFragFn);
        pDesc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
        pDesc->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth16Unorm);

        pVertexFn->release();
        pFragFn->release();
        return pDesc;
    }
} // namespace AnimationSystem
