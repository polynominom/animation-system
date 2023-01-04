/*  Shadowpass VS and PS
    PS is reserved for transparency rendering feature that will be
    added later in the future.
*/

#include "common_buffers.hlsl"
#include "common_layouts.hlsl"

float4 main_VS(VertexIn vin) : SV_POSITION
{
    // World position
    float4 PosL = mul(float4(vin.PosL, 1.0f), g_world);

    // Homogenous device position
    return mul(PosL, g_viewProj);
}
