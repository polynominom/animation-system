#ifndef SHADERRESOURCE_HPP
#define SHADERRESOURCE_HPP

#include "ShaderLibrary.hpp"
#include <Core/Core.h>

#include <Metal/Metal.hpp>

#include <iostream>

namespace AnimationSystem
{
    class ShaderResource
    {
    public:
        static void load(MTL::Device *pDevice, const std::filesystem::path shaderPath, ShaderLibrary &lib)
        {
            auto shaderSrc = Reader<std::string>(shaderPath).read();
            const char *c_shaderSrc = shaderSrc.c_str();
            if (shaderSrc.empty())
            {
                std::cout << "[ERROR] When reading File " << shaderPath << ". It does not exist or something worse...\n";
                return;
            }

            NS::Error *pError = nullptr;
            MTL::Library *pLibrary = pDevice->newLibrary(NS::String::string(c_shaderSrc, NS::StringEncoding::UTF8StringEncoding), nullptr, &pError);

            if (!pLibrary)
            {
                std::cout << pError->localizedDescription()->utf8String() << std::endl;
                assert(false);
            }

            lib.add(pLibrary, shaderPath, c_shaderSrc);
        }
    };

} // namespace AnimationSystem

#endif