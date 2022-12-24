#pragma once
#include "Skeleton.hpp"
#include "JointPose.hpp"
#include <memory>

namespace AnimationSystem
{
    class SkeletonPose
    {
    private:
        std::unique_ptr<Skeleton> _pSkeleton;
        std::vector<JointPose> _localPoses;
    };

} // namespace AnimationSystem
