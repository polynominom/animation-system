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

#include <vector>
#include <iostream>
#include <memory>

namespace AnimationSystem
{
    class MetalRenderer
    {
    public:
        MetalRenderer(MTL::Device *device);
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

        int _frame;
        dispatch_semaphore_t _semaphore;
        uint _animationIndex;
        float _zoom;
        float _angle;

        size_t _drawIndex;
    };
} // namespace AnimationSystem

#endif
