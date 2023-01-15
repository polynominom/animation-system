#ifndef EXAMPLEAPPDELEGATE_HPP
#define EXAMPLEAPPDELEGATE_HPP

#if defined(COREDEMO)
#include <CoreTestViewDelegate.hpp>
#include <Renderer/MetalFrameDebugger.hpp>
#define DEMO_VIEW_DELEGATE Example::CoreTestViewDelegate
#define __METAL_FRAME_DEBUGGER 1
#elif defined(METALDEMO)
#include <ExampleViewDelegate.hpp>
#define DEMO_VIEW_DELEGATE AnimationSystem::ExampleViewDelegate
#define __METAL_FRAME_DEBUGGER 0
#else
#define DEMO_VIEW_DELEGATE
#define __METAL_FRAME_DEBUGGER 0
#endif

namespace AnimationSystem
{
    class ExampleAppDelegate : public NS::ApplicationDelegate
    {
    public:
        ~ExampleAppDelegate();
        NS::Menu *createMenuBar();

        virtual void applicationWillFinishLaunching(NS::Notification *p_notification) override;
        virtual void applicationDidFinishLaunching(NS::Notification *p_notification) override;
        virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application *p_sender) override;

    private:
        NS::Window *_pWindow;
        MTK::View *_pMtkView;
        MTL::Device *_pDevice;
        DEMO_VIEW_DELEGATE *_pViewDelegate{nullptr};
    };
}

#endif
