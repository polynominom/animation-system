#include <ExampleViewDelegate.hpp>

namespace AnimationSystem
{
    ExampleViewDelegate::ExampleViewDelegate(MTL::Device *device)
        : MTK::ViewDelegate(), _pRenderer(new Renderer(device))
    {
    }

    ExampleViewDelegate::~ExampleViewDelegate()
    {
        delete this->_pRenderer;
    }

    void ExampleViewDelegate::drawInMTKView(MTK::View *pView)
    {
        _pRenderer->draw(pView);
    }
}