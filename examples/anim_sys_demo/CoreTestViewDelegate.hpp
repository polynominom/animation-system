#ifndef CORETEST_HPP
#define CORETEST_HPP

#include <AnimationSystem.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Metal/Metal.hpp>

namespace Example
{

    class CoreTestViewDelegate : public MTK::ViewDelegate
    {
    public:
        CoreTestViewDelegate(MTL::Device *pDevice);
        
        virtual ~CoreTestViewDelegate() override;
        virtual void drawInMTKView(MTK::View *pView) override;

    private:
        AnimationSystem::AnimationSystem *_pAnimSystem;
    };

} // namespace AnimationSystem

#endif
