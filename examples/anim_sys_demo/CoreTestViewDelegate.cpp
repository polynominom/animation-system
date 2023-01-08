#include "CoreTestViewDelegate.hpp"

namespace AnimationSystem
{
    CoreTestViewDelegate::CoreTestViewDelegate(MTL::Device *pDevice) : MTK::ViewDelegate()
    {
        _pRenderer = new MetalRenderer(pDevice);
    }
    CoreTestViewDelegate::~CoreTestViewDelegate()
    {
        delete this->_pRenderer;
    }

    void CoreTestViewDelegate::drawInMTKView(MTK::View *pView)
    {
        _pRenderer->draw(pView);
    }
} // namespace AnimationSystem
