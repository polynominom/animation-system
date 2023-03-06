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
        SkeletonPose() { _pSkeleton = std::make_shared<Skeleton>(); }
        SkeletonPose(const aiScene *pScene) : _scene(*pScene), _pSkeleton(std::make_shared<Skeleton>()) {}
        SkeletonPose(std::shared_ptr<Skeleton> pSkeleton);
        void updateGlobalPose(std::string name, simd::float4x4 pose);
        void updateGlobalPose(int id, simd::float4x4 pose);
        void addJointName(std::string name, size_t id) noexcept { _jointNameToIdMap[name] = id; }
        [[nodiscard]] int getJointId(const std::string &name)
        {
            auto x = _jointNameToIdMap.find(name);
            if (x == _jointNameToIdMap.end())
                return -1;

            return (int)x->second;
        }

        void computeGlobalPosesFromAssimp(float timeInSec, const aiScene *pScene);
        void initGlobalPoses();
        std::shared_ptr<Skeleton> getSkeleton() { return _pSkeleton; }
        void compFinalTransformations();
        std::vector<simd::float4x4> getFinalTransformations() const noexcept { return _finalTransformations; }
        const simd::float4x4 getGlobalPose(int id) const { return _globalPoses[id]; };
        simd::float3 getTranslation(int id);
        float getScale(int id);
        simd::quatf getRotation(int id);
        void setTranslation(int id, simd::float3 t);
        void setScale(int id, float s);
        void setRotation(int id, simd::quatf r);
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
