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
        static void load(const std::filesystem::path shaderPath, ShaderLibrary &lib);
    };

} // namespace AnimationSystem

#endif
