/*  Forwardpass Renderer PS
*/

#include "common_buffers.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"
#include "lighting_helpers.hlsl"
#include "material_helpers.hlsl"
#include "shadow_helpers.hlsl"


float4 emission_PS(in float4 PosH : SV_POSITION, in float2 TexCoord0 : TEXTURE0) : SV_Target0
{
    float4 sample_emission = g_buffer_emission.Sample(g_sampler_anisotropic, TexCoord0);
    float3 emissive = sample_emission.rgb;

    return float4(emissive, 1.0f);
}

float4 light_pass_PS(in float4 PosH : SV_POSITION, in float2 TexCoord0 : TEXTURE0) : SV_Target0
{
    Fragment frag = (Fragment)0;
    frag.DeferredInitialize(TexCoord0);

    Light light = (Light)0;
    light.Initialize(frag.position, frag.normal);

    float shadow = GetShadowFactor(light, frag);
    light.radiance *= shadow;

    float3 lit_color = frag.Shade(light);
    return float4(lit_color, 1.0f);
}
