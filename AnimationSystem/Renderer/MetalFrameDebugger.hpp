#ifndef __METAL_FRAME_DEBUGGER__HPP
#define __METAL_FRAME_DEBUGGER__HPP

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <simd/simd.h>
#include <chrono>
#include <time.h>

namespace AnimationSystem
{
    class MetalFrameDebugger
    {
    public:
        static MetalFrameDebugger &getInstance()
        {
            static MetalFrameDebugger instance; // Guaranteed to be destroyed.
                                                // Instantiated on first use.
            return instance;
        }

        MetalFrameDebugger(MetalFrameDebugger const &) = delete;
        void operator=(MetalFrameDebugger const &) = delete;

        void triggerCapture(MTL::Device *pDevice);
        bool isBeginCapture() { return _beginCapture; }
        void setBeginCapture(bool c) { _beginCapture = c; }
        NS::String *getFileName() { return _pTraceSaveFilePath; }

    private:
        MetalFrameDebugger() {}
        bool _beginCapture{false};
        NS::String *_pTraceSaveFilePath{nullptr};
    };
} // namespace AnimationSystem

#endif
