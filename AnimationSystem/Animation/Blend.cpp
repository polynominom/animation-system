#include "Blend.hpp"

namespace AnimationSystem
{
    template <typename Blender, typename BlendWeighter>
    void BlenderLocal(const SkeletonPose *pSourcePose, const SkeletonPose *pTargetPose, const float blendWeight, const JointMask *pJointMask, SkeletonPose *pResultPose)
    {
        int n = pResultPose->getSkeleton()->jointCount();
        for (int jointId = 0; jointId < n; ++jointId)
        {
            const float jointBlendWeight = BlendWeighter::getBlendWeight(blendWeight, pJointMask, jointId);
            if (jointBlendWeight == 0.0f)
            {
                pResultPose->updateGlobalPose(jointId, pSourcePose->getGlobalPose(jointId));
            }
            else
            {
                const simd::float4x4 sourceTransform = pSourcePose->getGlobalPose(jointId);
                const simd::float4x4 targetTransform = pTargetPose->getGlobalPose(jointId);

                simd::float3 translation = Blender::blendTranslation(Math::translation(sourceTransform), Math::translation(targetTransform), jointBlendWeight);
                pResultPose->setTranslation(jointId, translation);

                float scale = Blender::blendScale(Math::scale(sourceTransform), Math::scale(targetTransform), jointBlendWeight);
                pResultPose->setScale(jointId, scale);

                simd::quatf rotation = Blender::blendRotation(Math::rotation(sourceTransform), Math::rotation(targetTransform), jointBlendWeight);
                pResultPose->setRotation(jointId, rotation);
            }
        }
    }
}
