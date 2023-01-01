#ifndef ANIMATIONCLIP_HPP
#define ANIMATIONCLIP_HPP

#include "AnimationSample.hpp"

namespace AnimationSystem
{
    class AnimationClip
    {
    private:
        bool _isLooping;
        float32_t _fps;
        uint32_t _frameCount;
        uint32_t _skeletonId;
        std::vector<AnimationSample> _animationSamples;
    };
} // namespace AnimationSystem
#endif
