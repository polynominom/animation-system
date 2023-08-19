#include "AnimationClip.hpp"
#include <memory>

namespace AnimationSystem
{
    void AnimationClip::init(size_t animationSampleSize, size_t jointCount)
    {
        _animationSamples.clear();
        _animationSamples.reserve(animationSampleSize);
        
        for(size_t i = 0; i < animationSampleSize; ++i )
        {
            _animationSamples.emplace_back(std::make_unique<AnimationSample>(jointCount));
        }
        
    }
} // namespace AnimationSyste
