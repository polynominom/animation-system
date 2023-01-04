/*  Forwardpass Renderer VS
*/

#include "color_helpers.hlsl"
#include "common_buffers.hlsl"
#include "common_layouts.hlsl"
#include "transformation_helpers.hlsl"

PixelIn main_VS(VertexIn vin)
{
	PixelIn vout;

    // World position
    vout.PosL = ModelToWorld(vin.PosL);
 
     // Homogenous device position
    vout.PosH = mul(float4(vout.PosL, 1.0f), g_viewProj);

    // World normal
    vout.Normal = normalize(mul(float4(vin.Normal, 0.0f), g_worldInvTran).xyz);

    // World tangent
    vout.Tangent = normalize(mul(float4(vin.Tangent, 0.0f), g_world).xyz);

    // TexCoord
    vout.TexCoord0 = vin.TexCoord0;
    
    // Vertex color
	vout.Color = Decode32bitRGBA(vin.Color); 
    
    return vout;
}
