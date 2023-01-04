#ifndef EXAMPLEAPPDELEGATE_HPP
#define EXAMPLEAPPDELEGATE_HPP

#if defined(COREDEMO)
#include <CoreTestViewDelegate.hpp>
#define DEMO_VIEW_DELEGATE AnimationSystem::CoreTestViewDelegate
#elif defined(METALDEMO)
#include <ExampleViewDelegate.hpp>
#define DEMO_VIEW_DELEGATE AnimationSystem::ExampleViewDelegate
#else
#define DEMO_VIEW_DELEGATE
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
