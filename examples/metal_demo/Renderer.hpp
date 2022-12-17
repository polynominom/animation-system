#pragma once

#include <iostream>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>
#include <simd/simd.h>

namespace AnimationSystem
{
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
        void buildFrameData();
        void draw(MTK::View *pView);

    private:
        MTL::Device *_pDevice;
        MTL::CommandQueue *_pCommandQueue;
        MTL::Library *_pShaderLibrary;
        MTL::RenderPipelineState *_pPSO;
        MTL::Buffer *_pArgBuffer;
        MTL::Buffer *_pVertexPositionBuffer;
        MTL::Buffer *_pVertexColorBuffer;

        MTL::Buffer *_pFrameData[3];
        float _angle;
        int _frame;
        dispatch_semaphore_t _semaphore;
        static const int kMaxFrames{3};
    };
}
