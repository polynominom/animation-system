#include <iostream>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <cassert>

namespace AnimationSystem
{
    class Renderer
    {
    public:
        Renderer(MTL::Device *pDevice);
        ~Renderer();
        void draw(MTK::View *pView);

    private:
        MTL::Device *_pDevice;
        MTL::CommandQueue *_pCommandQueue;
    };
}
