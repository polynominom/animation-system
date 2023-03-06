#include <SkeletonPose.hpp>
#include <Math.hpp>

namespace AnimationSystem
{
using JointMask = std::vector<float>;
    template<typename Blender, typename BlendWeight>
    void BlenderLocal(const SkeletonPose* pSourcePose, const SkeletonPose* pTargetPose, const float blendWeight, const JointMask* pJointMask, SkeletonPose* pResultPose)
    {
        int n = pResultPose->getSkeleton()->jointCount();
        for( int jointId = 0; jointId < n; ++jointId)
        {
            const float jointBlendWeight = BlendWeight::GetBlendWeight( blendWeight, pJointMask, jointId );
            if(jointBlendWeight == 0.0f)
            {
                pResultPose->updateGlobalPose(jointId, pSourcePose->getGlobalPose(jointId));
            }
            else 
            {
                const simd::float4x4 sourceTransform = pSourcePose->getGlobalPose(jointId);
                const simd::float4x4 targetTransform = pTargetPose->getGlobalPose(jointId);

                simd::float3 translation = Blender::BlendTranslation(Math::translation(sourceTransform), Math::translation(targetTransform), jointBlendWeight);
                pResultPose->setTranslation(jointId, translation);

                float scale = Blender::BlendTranslation(Math::scale(sourceTransform), Math::scale(targetTransform), jointBlendWeight);
                pResultPose->setScale(jointId, scale);

                simd::quatf rotation = Blender::BlendTranslation(Math::rotation(sourceTransform), Math::rotation(targetTransform), jointBlendWeight);
                pResultPose->setRotation(jointId, rotation);
            }
        }
    }
}
