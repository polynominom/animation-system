/*  Forwardpass Renderer PS
*/

#include "common_buffers.hlsl"
#include "common_layouts.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"
#include "material_helpers.hlsl"

struct PixelOut
{
    float4 albedo   : SV_Target0;
    float3 normal   : SV_Target1;
    float3 position : SV_Target2;
    float4 material : SV_Target3;
    float3 emission : SV_Target4;
};

PixelOut main_PS(PixelIn pin)
{
    PixelOut p_out = (PixelOut)0;

    Fragment frag = (Fragment)0;
    frag.Initialize(pin.PosL, pin.Normal, pin.Tangent, pin.TexCoord0, pin.PosH.z / pin.PosH.w);

    p_out.albedo = float4(frag.albedo, frag.alpha);
    p_out.normal = frag.normal;
    p_out.position = frag.position;
    p_out.material = float4(frag.roughness, frag.metalness, frag.occlusion, 0.0f);
    p_out.emission = frag.emissive;

    return p_out;
}
