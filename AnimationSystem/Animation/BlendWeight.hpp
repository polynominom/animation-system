#ifndef BLEND_WEIGHT_HPP
#define BLEND_WEIGHT_HPP

#include <JointMask.hpp>

namespace AnimationSystem
{
    class BlendWeight
    {
    public:
        static float getBlendWeight(const float weight, const JointMask *pJointMask, const int jointId);
    };
} // namespace AnimationSystem

#endif
