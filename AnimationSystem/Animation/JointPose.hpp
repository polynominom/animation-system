#pragma once
#include <Base/Quaternion.hpp>
#include "Joint.hpp"
#include <simd/simd.h>

namespace AnimationSystem
{
    class JointPose
    {
    private:
        Base::Quaternion _rotation{0, 0, 0, 0};
        simd::float4 _translation;
        simd::float4 _scale;
    };
} // namespace AnimationSystem
