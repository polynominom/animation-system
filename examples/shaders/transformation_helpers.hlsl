/*  Helpers for transformation options
*/

#ifndef PPGE_TRANSFORMATION_HELPERS
#define PPGE_TRANSFORMATION_HELPERS

#include "math_helpers.hlsl"

#ifdef PPGE_COMMON_BUFFERS
inline float3 ModelToWorld(float3 vec, bool is_position = true)
{
    return  mul(float4(vec, (float)is_position), g_world).xyz;
}

inline float3 WorldToView(float3 vec, bool is_position = true)
{
    return mul(float4(vec, (float)is_position), g_view).xyz;
}

float3 ViewToNDC(float3 vec, bool is_position = true)
{
    float4 ndc = mul(float4(vec, (float)is_position), g_proj);
    return ndc.xyz / ndc.w;
}

float3 WorldToNDC(float3 vec, bool is_position = true)
{
    float4 ndc = mul(float4(vec, (float)is_position), g_viewProj);
    return ndc.xyz / ndc.w;
}
#endif

float3 WorldToNDC(float3 vec, float4x4 transform)
{
    float4 ndc = mul(float4(vec, 1.0f), transform);
    return ndc.xyz / ndc.w;
}

inline float2 NDCToUV(float3 vec)
{
    return vec.xy * float2(0.5f, -0.5f) + 0.5f;
}

inline uint CubemapDirectionToFaceIndex(float3 direction)
{
    const float3 abs_direction = abs(direction);
    const float max_coord = VectorMax(abs_direction);

    if (max_coord == abs_direction.x)
    {
        return direction.x == abs_direction.x ? 1 : 0;
    }
    else if (max_coord == abs_direction.y)
    {
        return direction.y == abs_direction.y ? 2 : 3;
    }
    else
    {
        return direction.z == abs_direction.z ? 4 : 5;
    }
}

#endif // PPGE_TRANSFORMATION_HELPERS
