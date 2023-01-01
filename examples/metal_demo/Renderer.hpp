#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>
#include <Core/Core.h>
#include <Base/Base.h>

namespace AnimationSystem
{
    static constexpr size_t kMaxFrames = 3;
    static constexpr size_t kInstanceDepth = 10;
    static constexpr size_t kInstanceColumns = 10;
    static constexpr size_t kInstanceRows = 10;
    static constexpr size_t kNumInstances = (kInstanceRows * kInstanceColumns * kInstanceDepth);
    static constexpr size_t kTextureWidth = 128;
    static constexpr size_t kTextureHeight = 128;

    class Renderer
    {
    public:
        Renderer(MTL::Device *pDevice);
        ~Renderer();
        void buildShaders();
        void buildComputePipeline();
        void buildBuffers();
        void buildTextures();
        void buildDepthStencilStates();
        void generateMandelbrotTexture(MTL::CommandBuffer *pCommandBuffer);
        void draw(MTK::View *pView);

    private:
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

        // Buffers
        MTL::Buffer *_pVertexDataBuffer;
        MTL::Buffer *_pInstanceDataBuffer[kMaxFrames];
        MTL::Buffer *_pCameraDataBuffer[kMaxFrames];
        MTL::Buffer *_pIndexBuffer;
        MTL::Buffer *_pTextureAnimationBuffer;

        float _angle;
        int _frame;
        dispatch_semaphore_t _semaphore;
        static const int kMaxFrames{3};
        uint _animationIndex;
    };
}

#endif
