/*  Helpers for material calculations
*/

#ifndef PPGE_MATERIAL_HELPERS
#define PPGE_MATERIAL_HELPERS

#include "color_helpers.hlsl"
#include "common_buffers.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"
#include "lighting_helpers.hlsl"
#include "pbr_helpers.hlsl"
#include "renderer_helpers.hlsl"
#include "transformation_helpers.hlsl"

#define IS_ALBEDO_MAP_BOUND     uint(1U << 0)
#define IS_SPECULAR_MAP_BOUND   uint(1U << 1)
#define IS_ROUGHNESS_MAP_BOUND  uint(1U << 2)
#define IS_METALIC_MAP_BOUND    uint(1U << 3)
#define IS_NORMAL_MAP_BOUND     uint(1U << 4)
#define IS_OCCLUSION_MAP_BOUND  uint(1U << 5)
#define IS_EMISSION_MAP_BOUND   uint(1U << 6)
#define IS_HEIGHT_MAP_BOUND     uint(1U << 7)
#define IS_ALPHA_MASK_MAP_BOUND uint(1U << 8)

bool material_is_albedo_map_bound()     { return g_material_options & IS_ALBEDO_MAP_BOUND;     }
bool material_is_specular_map_bound()   { return g_material_options & IS_SPECULAR_MAP_BOUND;   }
bool material_is_roughness_map_bound()  { return g_material_options & IS_ROUGHNESS_MAP_BOUND;  }
bool material_is_metalic_map_bound()    { return g_material_options & IS_METALIC_MAP_BOUND;    }
bool material_is_normal_map_bound()     { return g_material_options & IS_NORMAL_MAP_BOUND;     }
bool material_is_occlusion_map_bound()  { return g_material_options & IS_OCCLUSION_MAP_BOUND;  }
bool material_is_emission_map_bound()   { return g_material_options & IS_EMISSION_MAP_BOUND;   }
bool material_is_height_map_bound()     { return g_material_options & IS_HEIGHT_MAP_BOUND;     }
bool material_is_alpha_mask_map_bound() { return g_material_options & IS_ALPHA_MASK_MAP_BOUND; }

struct Fragment
{
    float  alpha;
    float3 albedo;
    float  roughness;
    float  metalness;
    float  occlusion;
    float3 emissive;
    float3 position;
    float  depth;
    float3 normal;
    float  fragment_to_eye_distance;
    float3 fragment_to_eye;

    float3 SampleNormalMap(float3 in_normal, float3 in_tangent, float2 in_uv)
    {
        float3 normal_map_value = g_material_normal.Sample(g_sampler_anisotropic, in_uv).rgb;
        normal_map_value = 2.0f * normal_map_value - 1.0f;

        float3 N = normalize(in_normal);
        float3 T = normalize(in_tangent - dot(in_tangent, N) * N);
        float3 B = cross(N, T);

        float3x3 TBN = float3x3(T, B, N);

        return normalize(mul(normal_map_value, TBN));
    }

    void Initialize(float3 in_position, float3 in_normal, float3 in_tangent, float2 in_uv, float in_depth)
    {
        alpha = 1.0f;
        [flatten] if (material_is_alpha_mask_map_bound())
        {
            alpha = g_material_alpha_mask.Sample(g_sampler_anisotropic, in_uv).r;
        }

        albedo = g_albedo_color.rgb;
        [flatten] if (material_is_albedo_map_bound())
        {
            float4 albedo_sample = g_material_albedo.Sample(g_sampler_anisotropic, in_uv);
            alpha  = min(alpha, albedo_sample.a);
            albedo *= degamma(albedo_sample.rgb, 1.5f);
        }

        roughness = g_roughness_factor;
        [flatten] if (material_is_roughness_map_bound())
        {
            roughness *= g_material_roughness.Sample(g_sampler_anisotropic, in_uv).g;
        }

        metalness = g_metalic_factor;
        [flatten] if (material_is_metalic_map_bound())
        {
            metalness *= g_material_metalic.Sample(g_sampler_anisotropic, in_uv).b;
        }

        normal = normalize(in_normal);
        [flatten] if (material_is_normal_map_bound())
        {
            normal = SampleNormalMap(in_normal, in_tangent, in_uv);
        }

        occlusion = 1.0f;
        [flatten] if (material_is_occlusion_map_bound())
        {
            occlusion = g_material_occlusion.Sample(g_sampler_anisotropic, in_uv).r;
        }

        emissive = 0.0f;
        [flatten] if (material_is_emission_map_bound())
        {
            emissive = g_emissive_color.rgb;
            emissive *= g_material_emission.Sample(g_sampler_anisotropic, in_uv).rgb;
        }

        depth                    = in_depth;
        position                 = in_position;
        fragment_to_eye          = g_camera_position - in_position;
        fragment_to_eye_distance = length(fragment_to_eye);
        fragment_to_eye          = normalize(fragment_to_eye);
    }

    void DeferredInitialize(float2 ss_coord)
    {
        float4 sample_albedo = g_buffer_albedo.Sample(g_sampler_anisotropic, ss_coord);
        alpha = sample_albedo.a;
        albedo = sample_albedo.rgb;

        float4 sample_material = g_buffer_material.Sample(g_sampler_anisotropic, ss_coord);
        roughness = sample_material.r;
        metalness = sample_material.g;
        occlusion = sample_material.b;

        float4 sample_normal = g_buffer_normal.Sample(g_sampler_anisotropic, ss_coord);
        normal = sample_normal.rgb;

        float4 sample_emission = g_buffer_emission.Sample(g_sampler_anisotropic, ss_coord);
        emissive = sample_emission.rgb;

        float4 sample_position = g_buffer_position.Sample(g_sampler_anisotropic, ss_coord);
        depth = ViewToNDC(sample_position.rgb).z;
        position = sample_position.rgb;
        fragment_to_eye          = g_camera_position - sample_position.rgb;
        fragment_to_eye_distance = length(fragment_to_eye);
        fragment_to_eye          = normalize(fragment_to_eye);
    }

    float3 Shade(Light light)
    {
        float3 color = float3(0.0f, 0.0f, 0.0f);
        if (!light_is_directional() && !light.IsInRange())
            return color;

        float3 f0      = lerp(0.04f, albedo, metalness);
        float3 l       = -light.light_to_fragment;
        float3 e       = fragment_to_eye;
        float3 h       = normalize(l + e);
        float  n_dot_e = saturate(dot(e, normal));
        float  n_dot_l = saturate(dot(l, normal));
        float  h_dot_e = saturate(dot(e, h));
        float  h_dot_l = saturate(dot(l, h));
        float  n_dot_h = saturate(dot(h, normal));

        float3 diffuse_energy  = float3(0.0f, 0.0f, 0.0f);
        float3 specular_energy = float3(1.0f, 1.0f, 1.0f);
        float3 diffuse_brdf    = float3(0.0f, 0.0f, 0.0f);
        float3 specular_brdf   = float3(0.0f, 0.0f, 0.0f);

        specular_brdf = SpecularBRDF(roughness, metalness, f0, n_dot_e, n_dot_l, 
                                     h_dot_e, h_dot_l, n_dot_h, specular_energy);

        diffuse_brdf   = DiffuseBRDF(albedo, roughness, n_dot_e, n_dot_l, h_dot_e);
        diffuse_energy = lerp(float3(1.0f, 1.0f, 1.0f) - specular_energy, float3(0.0f, 0.0f, 0.0f), metalness);

        color = (diffuse_energy * diffuse_brdf + specular_brdf) * light.radiance * n_dot_l;

        return saturate(color);
    }
};

#endif // PPGE_MATERIAL_HELPERS
