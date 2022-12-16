#include <iostream>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>
#include <simd/simd.h>

namespace AnimationSystem
{
    class Renderer
    {
    public:
        Renderer(MTL::Device *pDevice);
        ~Renderer();
        void buildShaders();
        void buildBuffers();
        void draw(MTK::View *pView);

    private:
        MTL::Device *_pDevice;
        MTL::CommandQueue *_pCommandQueue;

        MTL::RenderPipelineState *_pPSO;
        MTL::Buffer *_pVertexPositionBuffer;
        MTL::Buffer *_pVertexColorBuffer;
    };
}
