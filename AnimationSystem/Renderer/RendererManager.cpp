#include "RendererManager.hpp"

namespace AnimationSystem
{
    MTL::Device *RendererManager::_pDevice = nullptr;
    void RendererManager::init(MTL::Device *device)
    {
        if (!_pDevice)
            _pDevice = device->retain();
    }

    MTL::Device *RendererManager::getDevice()
    {
        return _pDevice;
    }

} // namespace AnimationSystem
