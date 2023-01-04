#include "ShaderResource.hpp"
#include <Renderer/RendererManager.hpp>

namespace AnimationSystem
{
    void ShaderResource::load(const std::filesystem::path shaderPath, ShaderLibrary &lib)
    {
        auto shaderSrc = Reader<std::string>(shaderPath).read();
        const char *c_shaderSrc = shaderSrc.c_str();
        if (shaderSrc.empty())
        {
            std::cout << "[ERROR] When reading File " << shaderPath << ". It does not exist or something worse...\n";
            return;
        }

        NS::Error *pError = nullptr;
        MTL::Library *pLibrary = RendererManager::getDevice()->newLibrary(NS::String::string(c_shaderSrc, NS::StringEncoding::UTF8StringEncoding), nullptr, &pError);

        if (!pLibrary)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

        lib.add(pLibrary, shaderPath, c_shaderSrc);
    }
}
