#pragma once
#include <simd/simd.h>

namespace AnimationSystem
{
    namespace ShaderTypes
    {
        struct InstanceData
        {
            simd::float4x4 instanceTransform;
            simd::float4 instanceColor;
        };

        struct CameraData
        {
            simd::float4x4 perspectiveTransform;
            simd::float4x4 worldTransform;
        };
    }

} // namespace AnimationSystem
