#ifndef CORETEST_HPP
#define CORETEST_HPP

#include <Renderer/Renderer.h>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>

namespace AnimationSystem
{
    class CoreTestViewDelegate : public MTK::ViewDelegate
    {
    public:
        CoreTestViewDelegate(MTL::Device *pDevice);
        virtual ~CoreTestViewDelegate() override;
        virtual void drawInMTKView(MTK::View *pView) override;

    private:
        MetalRenderer *_pRenderer;
    };

} // namespace AnimationSystem

#endif
