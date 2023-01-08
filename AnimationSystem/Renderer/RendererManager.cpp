#include "RendererManager.hpp"
#include <sys/time.h>

namespace AnimationSystem
{
    namespace{
        //return the current time in milliseconds
        long long getTimeMs()
        {
            timeval t;
            gettimeofday(&t, NULL);
            long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
            return ret;
        }
    }

    MTL::Device *RendererManager::_pDevice = nullptr;
    long long RendererManager::_startTimeMs = 0;

    void RendererManager::init(MTL::Device *device)
    {
        if (!_pDevice)
            _pDevice = device->retain();
        
        _startTimeMs = getTimeMs();
    }

    long long RendererManager::getStartTime()
    {
        return _startTimeMs;
    }

    long long RendererManager::getCurrentTime()
    {
        return getTimeMs();
    }

    MTL::Device *RendererManager::getDevice()
    {
        return _pDevice;
    }

} // namespace AnimationSystem
