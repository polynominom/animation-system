#include "SkeletonPose.hpp"
#include <Animation/AssimpAnimationHandler.hpp>
#include <iostream>

namespace AnimationSystem
{
    SkeletonPose::SkeletonPose(std::shared_ptr<Skeleton> pSkeleton): _pSkeleton(std::move(pSkeleton))
    {
        
    }

    void SkeletonPose::compFinalTransformations()
    {
        _finalTransformations.resize(_pSkeleton->jointCount(), Math::makeIdentity());
        for(size_t i = 0; i < _pSkeleton->jointCount(); ++i)
        {
            _finalTransformations[i] = _globalPoses[i]*_pSkeleton->getJoint(i)->getInverseTPose();
        }
    }
    
    void SkeletonPose::initGlobalPoses()
    {
        _globalPoses.resize(_pSkeleton->jointCount(), Math::makeIdentity());
    }

    void SkeletonPose::updateGlobalPose(std::string name, simd::float4x4 pose)
    {
        auto it = _jointNameToIdMap.find(name);
        if(it != _jointNameToIdMap.end())
            _globalPoses[it->second] = pose;
    }

    void SkeletonPose::computeGlobalPosesFromAssimp(float timeInSec, const aiScene* pScene)
    {
        AssimpAnimationHandler handler(this);
        handler.computeGlobalPoses(timeInSec, pScene);

    }
} // namespace AnimationSystem
