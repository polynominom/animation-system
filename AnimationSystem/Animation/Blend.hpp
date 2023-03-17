
#ifndef __BLEND__HPP
#define __BLEND__HPP

#include <SkeletonPose.hpp>
#include <Math.hpp>
#include <JointMask.hpp>

namespace AnimationSystem
{
    /**
     Blends Local skeleton poses with respect to joint mask and blend weights
     */
    template <typename Blender, typename BlendWeight>
    void BlenderLocal(const SkeletonPose *pSourcePose, const SkeletonPose *pTargetPose, const float blendWeight, const JointMask *pJointMask, SkeletonPose *pResultPose);
}

#endif
