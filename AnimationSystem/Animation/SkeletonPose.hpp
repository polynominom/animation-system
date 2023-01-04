#ifndef SKELETONPOSE_HPP
#define SKELETONPOSE_HPP

#include "Skeleton.hpp"
#include "JointPose.hpp"
#include <memory>
#include <simd/simd.h>

namespace AnimationSystem
{
    class SkeletonPose
    {
    public:
        SkeletonPose() {}

    private:
        std::unique_ptr<Skeleton> _pSkeleton;
        std::vector<JointPose> _localPoses;
        simd::float4x4 _globalPose;
    };

} // namespace AnimationSystem

#endif
