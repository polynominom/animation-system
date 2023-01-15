#ifndef ANIMATIONCLIP_HPP
#define ANIMATIONCLIP_HPP

#include "AnimationSample.hpp"
#include "Skeleton.hpp"

#include <vector>
#include <memory>

namespace AnimationSystem
{
    class AnimationClip
    {
    public:
        bool _isLooping;
        float32_t _fps;
        uint32_t _frameCount; // _animationSamples size
        std::shared_ptr<Skeleton> _pSkeleton;
        void init(size_t animationSampleSize, size_t jointCount);
    private:
        /**
         * List of animation samples provided from an animator
         * Each sample represent different frame that is sampled from a time t.
         */
        std::vector<std::shared_ptr<AnimationSample>> _animationSamples;
    };
} // namespace AnimationSystem
#endif
