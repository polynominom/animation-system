#ifndef __ANIMATION_SYSTEM_MANAGER__HPP
#define __ANIMATION_SYSTEM_MANAGER__HPP

#include "AnimationClip.hpp"
#include "AssimpAnimation.hpp"
#include <Renderer/Mesh.hpp>

#include <vector>
#include <memory>

namespace AnimationSystem
{
    class Manager
    {
    public:
        Manager() {}
        void setAnimationClips(std::vector<std::shared_ptr<AnimationClip>> &&clips) { _animationClips = std::move(clips); }
        void addAnimationClips(std::shared_ptr<AnimationClip> clip) { _animationClips.push_back(clip); }
        void setAssimpAnimations(std::vector<std::shared_ptr<AssimpAnimation>> &&anims) { _assimpAnimations = std::move(anims); }
        void calculateSkeletonPoses();
        void calculateSkeletonPosesWithTime(float timeInTick, std::shared_ptr<Mesh> &mesh);

    private:
        simd::float4x4 _compGlobalPoseRecursively(float timeInSec, SkeletonPose* pose, std::string name, const simd::float4x4& parentTransform);
        
        std::vector<std::shared_ptr<AnimationClip>> _animationClips;
        std::vector<std::shared_ptr<AssimpAnimation>> _assimpAnimations;
    };
} // namespace AnimationSystem

#endif
