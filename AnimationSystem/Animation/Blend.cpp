#include <SkeletonPose.hpp>
namespace AnimationSystem
{
    template<typename Blender, typename BlendWeight>
    void BlenderLocal(const SkeletonPose* pSourcePose, const SkeletonPose* pTargetPose, const float blendWeight, const JointMask* pJointMask, SkeletonPose* pResultPose)
    {
        int n = pResultPose->getSkeleton()->jointCount;
        for( int jointId = 0; jointId < n; ++jointCount; )
        {
            const float jointBlendWeight = BlendWeight::GetBlendWeight( blendWeight, pJointMask, jointId );
            if(jointBlendWeight == 0.0f)
            {
                pResultPose->updateGlobalPose(jointId, pSourcePose->getGlobalPose(jointId));
            }
            else 
            {
                simd::float4 sourceTransform = pSourcePose->getGlobalPose(jointId);
                simd::float4 targetTransform = pTargetPose->getGlobalPose(jointId);

                simd::float3 translation = Blender::BlendTranslation(sourceTransform.getTranslation(), targetTransform.getTranslation(), jointBlendWeight);
                pResultPose->setTranslation(jointId, translation);

                float scale = Blender::BlendTranslation(sourceTransform.getScale(), targetTransform.getScale(), jointBlendWeight);
                pResultPose->setScale(jointId, scale);

                simd::quatf rotation = Blender::BlendTranslation(sourceTransform.getRotation(), targetTransform.getRotation(), jointBlendWeight);
                pResultPose->setRotation(jointId, rotation);
            }
        }
    }
}
