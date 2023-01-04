/*  Forwardpass Renderer PS
*/

#include "common_buffers.hlsl"
#include "common_layouts.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"
#include "lighting_helpers.hlsl"
#include "material_helpers.hlsl"
#include "shadow_helpers.hlsl"

float4 ambient_pass_PS(PixelIn pin) : SV_Target
{
    Fragment frag = (Fragment)0;
    frag.Initialize(pin.PosL, pin.Normal, pin.Tangent, pin.TexCoord0, pin.PosH.z / pin.PosH.w);

    float3 ambient = lerp(float3(0.0f, 0.0f, 0.0f), frag.albedo, (1.0f - frag.metalness));
    return float4(0.05f * ambient * frag.occlusion, frag.alpha);
}


float4 light_pass_PS(PixelIn pin) : SV_Target
{
    Fragment frag = (Fragment)0;
    frag.Initialize(pin.PosL, pin.Normal, pin.Tangent, pin.TexCoord0, pin.PosH.z / pin.PosH.w);

    Light light = (Light)0;
    light.Initialize(frag.position, frag.normal);

    float shadow = GetShadowFactor(light, frag);
    light.radiance *= shadow;

    float3 lit_color = frag.Shade(light);
    return float4(lit_color, 1.0f);
}
