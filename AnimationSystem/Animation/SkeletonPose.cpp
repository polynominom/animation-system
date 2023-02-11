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

    void SkeletonPose::updateGlobalPose(int id, simd::float4x4 pose)
    {
        if(id < _globalPoses.size())
            _globalPoses[id] = pose;
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

    simd::float3 SkeletonPose::getTranslation(int id)
    {
        return _localPoses[id]->_translation;
    }
    
    float SkeletonPose::getScale(int id)
    {
        return _localPoses[id]->_scale;
    }

    simd::quatf SkeletonPose::getRotation(int id)
    {
        return _localPoses[id]->_rotation;
    }

    void SkeletonPose::setTranslation(int id, simd::float3 t)
    {
        _localPoses[id]->_translation = t;
    }

    void SkeletonPose::setScale(int id, float s)
    {
        _localPoses[id]->_scale = s;
    }

    void SkeletonPose::setRotation(int id, simd::quatf r)
    {
        _localPoses[id]->_rotation = r;
    }

} // namespace AnimationSystem
