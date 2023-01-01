#ifndef EXAMPLEVIEWDELEGATE_HPP
#define EXAMPLEVIEWDELEGATE_HPP

#include <Renderer.hpp>

namespace AnimationSystem
{
    class ExampleViewDelegate : public MTK::ViewDelegate
    {
    public:
        ExampleViewDelegate(MTL::Device *pDevice);
        virtual ~ExampleViewDelegate() override;
        virtual void drawInMTKView(MTK::View *pView) override;

    private:
        Renderer *_pRenderer;
    };
} // namespace AnimationSystem

#endif
