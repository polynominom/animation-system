#include <ExampleViewDelegate.hpp>

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
        ExampleViewDelegate *_pViewDelegate{nullptr};
    };
}
