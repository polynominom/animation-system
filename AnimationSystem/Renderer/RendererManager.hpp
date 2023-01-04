#ifndef __RENDERER_MANAGER_HPP
#define __RENDERER_MANAGER_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>

namespace AnimationSystem
{
    class RendererManager
    {
    public:
        static void init(MTL::Device *_pDevice);
        static MTL::Device *getDevice();

    private:
        static MTL::Device *_pDevice;
    };

} // namespace AnimationSystem
#endif
