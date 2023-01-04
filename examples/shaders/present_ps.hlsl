/*  PS that copies pixels from bound color texture
    into the currently bound render target
*/

#include "color_helpers.hlsl"

Texture2D g_color_buffer : register(t0);
SamplerState g_sampler   : register(s0);

float4 main_PS(in float4 PosH : SV_POSITION, in float2 TexCoord0 : TEXTURE0) : SV_Target0
{
    float3 color = g_color_buffer.Sample(g_sampler, TexCoord0);

    // Reinhard tonemapping operator.
    // see: "Photographic Tone Reproduction for Digital Images", eq. 4
    float luminance = dot(color, float3(0.2126f, 0.7152f, 0.0722f));
    float mapped_luminance = (luminance * (1.0f + luminance / (1.0f * 1.0f))) / (1.0 + luminance);
    // Scale color by ratio of average luminances.
    float3 mapped_color = (mapped_luminance / luminance) * color;
    // Gamma correction.
    color = gamma(mapped_color, 1.5f);

    return float4(color, 1.0f);
}
