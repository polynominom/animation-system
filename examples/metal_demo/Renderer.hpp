#pragma once

#include <iostream>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>
#include "ShaderTypes.hpp"

namespace AnimationSystem
{
    static constexpr size_t kMaxFrames = 3;
    static constexpr size_t kNumInstances = 32;
    struct FrameData
    {
        float angle;
    };

    class Renderer
    {
    public:
        Renderer(MTL::Device *pDevice);
        ~Renderer();
        void buildShaders();
        void buildBuffers();
        void buildDepthStencilStates();
        void buildFrameData();
        void draw(MTK::View *pView);

    private:
        MTL::Device *_pDevice;
        MTL::CommandQueue *_pCommandQueue;
        MTL::Library *_pShaderLibrary;
        MTL::RenderPipelineState *_pPSO;
        MTL::DepthStencilState *_pDepthStencilState;

        MTL::Buffer *_pVertexDataBuffer;
        MTL::Buffer *_pInstanceDataBuffer[kMaxFrames];
        MTL::Buffer *_pCameraDataBuffer[kMaxFrames];
        MTL::Buffer *_pIndexBuffer;

        float _angle;
        int _frame;
        dispatch_semaphore_t _semaphore;
        static const int kMaxFrames{3};
    };
}
