/*  Helpers for shadowing calculations
*/

#ifndef PPGE_SHADOW_HELPERS
#define PPGE_SHADOW_HELPERS

#include "color_helpers.hlsl"
#include "common_buffers.hlsl"
#include "common_constants.hlsl"
#include "common_textures.hlsl"
#include "common_samplers.hlsl"
#include "lighting_helpers.hlsl"
#include "material_helpers.hlsl"
#include "transformation_helpers.hlsl"

static const uint  g_pcf_size = 4;

float SampleAndCompareShadowMap(float3 uv, float compare_val)
{
    [flatten]
    if (light_is_directional())
    {
        return g_shadow_map_dir_light.SampleCmp(g_sampler_comparison, uv, compare_val).r;
    }
    else if (light_is_point())
    {
        return g_shadow_map_point_light.SampleCmp(g_sampler_comparison, uv, compare_val).r;
    }
    else if (light_is_spot())
    {
        return g_shadow_map_spot_light.SampleCmp(g_sampler_comparison, uv.xy, compare_val).r;
    }
    else 
    {
        return 0.0f;
    }
}

float PCF(float3 uv, float compare_val)
{
    float shadow = 0.0f;
    const float texel_size =  renderer_get_shadowmap_texel_size();

    for (uint y = 0; y < g_pcf_size; y++)
    {
        for (uint x = 0; x < g_pcf_size; x++)
        {
            float2 offset = float2(-1.0f * trunc(g_pcf_size / 2.0f) * texel_size + x * texel_size, 
                                   -1.0f * trunc(g_pcf_size / 2.0f) * texel_size + y * texel_size);
            shadow += SampleAndCompareShadowMap(uv + float3(offset.xy, 0.0f), compare_val);
        }
    }

    return shadow / (g_pcf_size * g_pcf_size);
}

inline float get_slope_corrected_bias(float lambertian_reflectance, float bias = -5.0f)
{
    return (1.0f - lambertian_reflectance) * 0.0005f * bias;
}

inline float3 get_normal_offset(float3 normal, float lambertian_reflectance, float normal_bias = 5.0f)
{
    return normal * (1.0f - lambertian_reflectance) * renderer_get_shadowmap_texel_size() * 10 * normal_bias;
}

float GetShadowFactor(Light light, Fragment fragment)
{
    float shadow = 1.0f;

    if (!light_can_cast_shadow())
        return shadow;

    // Add bias in the direction of fragment normal
    float3 world_pos = fragment.position + get_normal_offset(fragment.normal, light.lambertian_reflectance);

    if (light_is_directional())
    {
        float3 frag_in_light_space = WorldToNDC(world_pos, light_get_viewProj(0));
        float2 uv = NDCToUV(frag_in_light_space);

        if (IsSaturated(uv))
        {
            float shadow_comp = frag_in_light_space.z + get_slope_corrected_bias(light.lambertian_reflectance);
            shadow = PCF(float3(uv, 0.0f), shadow_comp);
        }
    }
    else if (light_is_point())
    {
        if (light.IsInRange())
        {
            float3 frag_in_light_space = WorldToNDC(world_pos, light_get_viewProj(CubemapDirectionToFaceIndex(light.light_to_fragment)));
            float shadow_comp = frag_in_light_space.z + get_slope_corrected_bias(light.lambertian_reflectance);
            shadow = PCF(light.light_to_fragment, shadow_comp);
        }
    }
    else if (light_is_spot())
    {
        if (light.IsInRange())
        {
            float3 frag_in_light_space = WorldToNDC(world_pos, light_get_viewProj(0));
            float2 uv = NDCToUV(frag_in_light_space);

            if (IsSaturated(uv))
            {
                float shadow_comp = frag_in_light_space.z + get_slope_corrected_bias(light.lambertian_reflectance);
                shadow = PCF(float3(uv, 0.0f), shadow_comp);
            }
        }
    }

    return shadow;
}

#endif // PPGE_SHADOW_HELPERS
