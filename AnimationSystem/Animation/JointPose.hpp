#ifndef JOINTPOSE_HPP
#define JOINTPOSE_HPP

#include <Base/Quaternion.hpp>
#include <simd/simd.h>

namespace AnimationSystem
{
    class JointPose
{
public:
    simd::quatf _rotation{.0f, .0f, .0f, 1.f};
    simd::float3 _translation{.0f, .0f, .0f};
    float _scale{1}; // uniform scale only
    };
} // namespace AnimationSystem

#endif
