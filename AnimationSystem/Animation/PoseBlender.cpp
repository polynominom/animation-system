//
//  PoseBlender.cpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 17.03.23.
//

#include "PoseBlender.hpp"

namespace AnimationSystem
{
    float PoseBlender::blendScale(float scaleSource, float scaleTarget, float weight)
    {
        return scaleSource * (1.0 - weight) + (scaleTarget * weight);
    }

    simd::float3 PoseBlender::blendTranslation(simd::float3 translationSource, simd::float3 translationDestination, float weight)
    {
        return simd::mix<simd::float3>(translationSource, translationDestination, weight);
    }

    simd::quatf PoseBlender::blendRotation(simd::quatf rotationSource, simd::quatf rotationTarget, float weight)
    {
        return simd_slerp(rotationSource, rotationTarget, weight);
    }
}
