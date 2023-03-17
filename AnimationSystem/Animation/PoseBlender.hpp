//
//  PoseBlender.hpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 17.03.23.
//

#ifndef PoseBlender_hpp
#define PoseBlender_hpp

#include <vector>
#include <SkeletonPose.hpp>
#include <simd/simd.h>

namespace AnimationSystem
{
    /**
     Basic pose blender, that is capable of blending two translation, rotation and scale.
     */
    class PoseBlender
    {
    public:
        static simd::float3 blendTranslation(simd::float3 translationSource, simd::float3 translationDestination, float weight);
        static float blendScale(float scaleSource, float scaleTarget, float weight);
        static simd::quatf blendRotation(simd::quatf rotationSource, simd::quatf rotationTarget, float weight);
    };
}

#endif /* PoseBlender_hpp */
