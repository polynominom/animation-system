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
        for(size_t i = 0; i < _pSkeleton->jointCount(); ++i)
        {
            //TODO: Matrix multiplication order could be vice versa
            _finalTransformations[i] =_globalPoses[i];
        }
    }
    
    void SkeletonPose::initGlobalPoses()
    {
        _globalPoses.resize(_pSkeleton->jointCount(), Math::makeIdentity());
        _finalTransformations.resize(_pSkeleton->jointCount(), Math::makeIdentity());
    }

    void SkeletonPose::updateGlobalPose(std::string name, simd::float4x4 pose)
    {
        auto it = _jointNameToIdMap.find(name);
        if(it != _jointNameToIdMap.end())
            _globalPoses[it->second] = pose;
    }

    void SkeletonPose::computeGlobalPosesFromAssimp(float timeInSec, const aiScene* pScene)
    {
        //AssimpAnimationHandler handler(this);
        //handler.computeGlobalPoses(timeInSec, pScene);

    }
} // namespace AnimationSystem
