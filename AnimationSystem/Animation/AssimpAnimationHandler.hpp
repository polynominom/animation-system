#ifndef __ASSIMP_ANIMATION_HANDLER_HPP__
#define __ASSIMP_ANIMATION_HANDLER_HPP__
#include <Animation/Animation.h>
#include <Animation/SkeletonPose.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <simd/simd.h>
#include <vector>
#include <memory>

namespace AnimationSystem
{
    class AssimpAnimationHandler
    {
    public:
        AssimpAnimationHandler(SkeletonPose *pose): _skeletonPose(pose){}
        simd::float3 lerpScale(float timeTick, const aiNodeAnim *pNodeAnim);
        simd::float3 lerpTranslate(float timeTick, const aiNodeAnim *pNodeAnim);
        simd::quatf lerpRotation(float timeTick, const aiNodeAnim *pNodeAnim);
        void computeGlobalPoses(float timeInSec, const aiScene* pScene);
        void readNodeHierarchy(float timeTick, const aiScene* pScene, const aiNode* pNode, const simd::float4x4& parentTransform);
        
    private:
        // Skeleton pose to handle.
        // The handler is not the owner of this pointer.
        SkeletonPose *_skeletonPose;
    };

} // namespace AnimationSystem

#endif
