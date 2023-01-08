#ifndef SKELETONPOSE_HPP
#define SKELETONPOSE_HPP

#include "Skeleton.hpp"
#include "JointPose.hpp"
#include <memory>
#include <simd/simd.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

namespace AnimationSystem
{
    class SkeletonPose
    {
    public:
        SkeletonPose(){_pSkeleton = std::make_shared<Skeleton>();}
        SkeletonPose(const aiScene* pScene): _scene(*pScene), _pSkeleton(std::make_shared<Skeleton>()){}
        SkeletonPose(std::shared_ptr<Skeleton> pSkeleton);
        void updateGlobalPose(std::string name, simd::float4x4 pose);
        void addJointName(std::string name, size_t id) noexcept {_jointNameToIdMap[name] = id;}
        void computeGlobalPosesFromAssimp(float timeInSec, const aiScene *pScene);
        void initGlobalPoses();
        std::shared_ptr<Skeleton> getSkeleton(){return _pSkeleton;}
        void compFinalTransformations();
        std::vector<simd::float4x4> getFinalTransformations() const noexcept {return _finalTransformations;}
        
    private:
        std::shared_ptr<Skeleton> _pSkeleton;
        std::vector<JointPose> _localPoses;
        std::unordered_map<std::string, size_t> _jointNameToIdMap{};
        std::vector<simd::float4x4> _globalPoses;
        std::vector<simd::float4x4> _finalTransformations;
        const aiScene _scene{};
    };

} // namespace AnimationSystem

#endif
