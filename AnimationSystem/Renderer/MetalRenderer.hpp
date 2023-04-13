#ifndef METALRENDERER_HPP
#define METALRENDERER_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Base/Entity.hpp>

#include <Core/Core.h>
#include <Renderer/Shader/ShaderLibrary.hpp>
#include <Renderer/Shader/ShaderResource.hpp>
#include <Renderer/Shapes/Cube.hpp>
#include <Renderer/Scene.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Import.hpp>
#include <Renderer/RendererManager.hpp>
#include <Renderer/MetalFrameDebugger.hpp>
#include <Animation/Manager.hpp>

#include <vector>
#include <iostream>
#include <memory>

/**
    BASIC RENDERER FOR ANIMATION SYSTEM
 */
namespace AnimationSystem
{
    class MetalRenderer
    {
    public:
        MetalRenderer(MTL::Device *device, std::shared_ptr<ResourceManager> resourceManager);
        ~MetalRenderer();
        MTL::Device *getDevice();
        void buildShaders();
        void buildComputePipeline();
        void buildEntities();
        void buildTextures();
        void buildDepthStencilStates();
        void draw(MTK::View *pView);

    private:
        // Metal Device
        MTL::Device *_pDevice;

        // Command queue
        MTL::CommandQueue *_pCommandQueue;

        // Shader library to load
        ShaderLibrary _shaderLibrary;

        // Pipeline States
        MTL::RenderPipelineState *_pPSO;
        MTL::ComputePipelineState *_pComputePSO;

        // Depth stencil
        MTL::DepthStencilState *_pDepthStencilState;

        // Texture
        MTL::Texture *_pTexture;

        // uniform buffer
        MTL::Buffer *_pUniformBuffer;

        // scene
        std::shared_ptr<Scene> _scene;

        // animation system manager
        std::shared_ptr<AnimationSystem::Manager> _pAnimationSystenManager;

        std::shared_ptr<ResourceManager> _resourceManager;

        // semaphore for draw
        dispatch_semaphore_t _semaphore;
    };
} // namespace AnimationSystem

#endif
