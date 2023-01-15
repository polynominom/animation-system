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
        AnimationSystem(MTL::Device *device, std::string shaderPath, std::string characterPath);
        MetalRenderer* getRenderer() { return _renderer.get(); }
    private:
        std::shared_ptr<MetalRenderer> _renderer;
        std::shared_ptr<ResourceManager> _resourceManager;
    };
} // namespace AnimationSystem

#endif
