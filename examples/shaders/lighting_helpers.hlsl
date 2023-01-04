/*  Helpers for lighting calculations
*/

#ifndef PPGE_LIGHTING_HELPERS
#define PPGE_LIGHTING_HELPERS

#include "common_buffers.hlsl"
#include "common_constants.hlsl"

#define LIGHT_IS_DIRECTIONAL   uint(1U << 0)
#define LIGHT_IS_POINT         uint(1U << 1)
#define LIGHT_IS_SPOT          uint(1U << 2)
#define LIGHT_CAN_CAST_SHADOW  uint(1U << 3)

bool     light_is_directional()       { return g_light_options & LIGHT_IS_DIRECTIONAL;  }
bool     light_is_point()             { return g_light_options & LIGHT_IS_POINT;        }
bool     light_is_spot()              { return g_light_options & LIGHT_IS_SPOT;         }
bool     light_can_cast_shadow()      { return g_light_options & LIGHT_CAN_CAST_SHADOW; }
float4x4 light_get_viewProj(uint idx) { return g_light_viewProj[idx];                   }

struct Light
{
    float3 color;
    float3 light_to_fragment;
    float  light_to_fragment_distance;
    float  attenuation;
    float  lambertian_reflectance;  // n dot l
    float3 radiance;

    bool IsInRange() { return light_to_fragment_distance < g_light_range; }

    float CalculateAttenuation()
    {
        // No attenuation for directional light
        if (light_is_directional())
        {
            return 1.0f;
        }
        // Distance attenuation for point light
        else if (light_is_point())
        {
            return 1.0f / dot(g_light_dist_attenuation, 
                float3(1.0f, light_to_fragment_distance, light_to_fragment_distance * light_to_fragment_distance));
        }
        // Distance and angle attenuation for spot light
        else if (light_is_spot())
        {
            float dist_attenuation = 1.0f / dot(g_light_dist_attenuation, 
                float3(1.0f, light_to_fragment_distance, light_to_fragment_distance * light_to_fragment_distance));

            float angle_attenuation = 0.0f;

            float theta = acos(dot(normalize(g_light_direction), light_to_fragment));
            float outer_cutoff = radians(min(g_light_angle_attenuation.x, 89.5f));
            float inner_cutoff = min(radians(max(g_light_angle_attenuation.y, 0)), outer_cutoff);

            if (theta < inner_cutoff)
            {
                angle_attenuation = 1.0f;
            }
            else if (theta < outer_cutoff)
            {
                float decay_rate = max(g_light_angle_attenuation.z, 0.0f);
                float cos_x = cos(theta - inner_cutoff);
                float cos_cufoff = cos(outer_cutoff - inner_cutoff);

                angle_attenuation = pow((1.0f - (1.0f - cos_x) / (1.0f - cos_cufoff)), decay_rate);
            }

            return angle_attenuation * dist_attenuation;
        }
        else
        {
            return 0.0f;
        }
    }

    void Initialize(float3 frag_pos, float3 frag_normal)
    {
        color = g_light_color * g_light_intensity;

        light_to_fragment_distance = PLUS_INF;
        light_to_fragment = normalize(g_light_direction);
        [flatten] if (!light_is_directional())
        {
            light_to_fragment = frag_pos - g_light_position;
            light_to_fragment_distance = length(light_to_fragment);
            light_to_fragment = normalize(light_to_fragment);
        }

        lambertian_reflectance = saturate(dot(frag_normal, -light_to_fragment));
        attenuation = CalculateAttenuation();
        radiance = color * attenuation;
    }
};

#endif // PPGE_LIGHTING_HELPERS
