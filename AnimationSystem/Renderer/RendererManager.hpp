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
        [[nodiscard]] static long long getStartTime();
        [[nodiscard]] static long long getCurrentTime();

    private:
        static MTL::Device *_pDevice;
        static long long _startTimeMs;
    };

} // namespace AnimationSystem
#endif
