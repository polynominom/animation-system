#ifndef SHADERLIBRARY_HPP
#define SHADERLIBRARY_HPP

#include <unordered_map>
#include <string>

#include <Metal/Metal.hpp>

namespace AnimationSystem
{
    class ShaderLibrary
    {
    public:
        ShaderLibrary(){};
        ~ShaderLibrary();
        [[nodiscard]] MTL::Library *get(const std::string &shaderSourceName);
        [[nodiscard]] MTL::RenderPipelineDescriptor *registerPipelineDescriptor(const std::string &shaderSourceName);
        void add(MTL::Library *pLibrary, std::string shaderSourceName, const char *shaderSource);

    private:
        [[no_unique_address]] std::unordered_map<std::string, MTL::Library *> _shaderMap;
    };
} // namespace AnimationSystem

#endif