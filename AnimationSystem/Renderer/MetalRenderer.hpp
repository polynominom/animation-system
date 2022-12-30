#pragma once
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Base/Entity.hpp>
#include <vector>

namespace AnimationSystem
{
    class MetalRenderer
    {
    public:
        MetalRenderer(MTL::Device *pDevice) : _entities{};
        MetalRenderer(MTL::Device *pDevice, std::vector<Entity> entities) : _entities(std::move(entities));
        ~MetalRenderer();
        void buildShaders();
        void buildComputePipeline();
        void buildBuffers();
        void buildTextures();
        void buildDepthStencilStates();
        void draw(MTK::View *pView);

    private:
        std::vector<Entity> _entities;

        // Metal Device
        MTL::Device *_pDevice;

        // Command queue
        MTL::CommandQueue *_pCommandQueue;

        // Shader library to load
        MTL::Library *_pShaderLibrary;

        // Pipeline States
        MTL::RenderPipelineState *_pPSO;
        MTL::ComputePipelineState *_pComputePSO;

        // Depth stencil
        MTL::DepthStencilState *_pDepthStencilState;

        // Texture
        MTL::Texture *_pTexture;

        float _angle;
        int _frame;
        dispatch_semaphore_t _semaphore;
        // hold 2-3 frames in memory
        static const int kMaxFrames{3};
        uint _animationIndex;
    };
} // namespace AnimationSystem
