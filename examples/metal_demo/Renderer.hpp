#pragma once

#include <iostream>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>
#include "ShaderTypes.hpp"

namespace AnimationSystem
{
    static constexpr size_t kMaxFrames = 3;
    static constexpr size_t kInstanceDepth = 10;
    static constexpr size_t kInstanceColumns = 10;
    static constexpr size_t kInstanceRows = 10;
    static constexpr size_t kNumInstances = (kInstanceRows * kInstanceColumns * kInstanceDepth);

    class Renderer
    {
    public:
        Renderer(MTL::Device *pDevice);
        ~Renderer();
        void buildShaders();
        void buildBuffers();
        void buildTextures();
        void buildDepthStencilStates();
        void draw(MTK::View *pView);

    private:
        MTL::Device *_pDevice;
        MTL::CommandQueue *_pCommandQueue;
        MTL::Library *_pShaderLibrary;
        MTL::RenderPipelineState *_pPSO;
        MTL::DepthStencilState *_pDepthStencilState;
        MTL::Texture *_pTexture;
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
