#ifndef JOINTPOSE_HPP
#define JOINTPOSE_HPP

#include <Base/Quaternion.hpp>
#include <simd/simd.h>

namespace AnimationSystem
{
    class JointPose
    {
    public:
        Base::Quaternion _rotation{0, 0, 0, 0};
        simd::float3 _translation;
        float _scale; // uniform sclae only
    };
} // namespace AnimationSystem

#endif
