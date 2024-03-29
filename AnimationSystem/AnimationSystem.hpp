#ifndef __ANIMATION_SYSTEM_HPP
#define __ANIMATION_SYSTEM_HPP

#include <memory>

#include <Renderer/MetalRenderer.hpp>
#include <Animation/Manager.hpp>
#include <Renderer/RendererManager.hpp>
#include <Core/ResourceManager.hpp>

namespace AnimationSystem
{
    class AnimationSystem
    {
    public:
        AnimationSystem();
        void setCommonShaderPath(const std::string &shaderPath);
        void setCommonCharacterPath(const std::string &shaderPath);
        void setLocomotionBlendSpecificationPath(const std::string &locomotionBlendSpecPath);
        void setRenderer(MTL::Device *device);

        MetalRenderer *getRenderer() { return _renderer.get(); }

    private:
        std::unique_ptr<MetalRenderer> _renderer{};
        std::unique_ptr<ResourceManager> _resourceManager{};
    };
} // namespace AnimationSystem

#endif
