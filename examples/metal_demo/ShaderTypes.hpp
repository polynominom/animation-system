#pragma once
#include <simd/simd.h>

namespace AnimationSystem
{
    namespace ShaderTypes
    {
        struct VertexData
        {
            simd::float3 position;
            simd::float3 normal;
        };

        struct InstanceData
        {
            simd::float4x4 instanceTransform;
            simd::float3x3 instanceNormalTransform;
            simd::float4 instanceColor;
        };

        struct CameraData
        {
            simd::float4x4 perspectiveTransform;
            simd::float4x4 worldTransform;
            simd::float3x3 worldNormalTransform;
        };
    }

} // namespace AnimationSystem
