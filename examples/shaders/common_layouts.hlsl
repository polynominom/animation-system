/*  Common structs for VS and PS inputs
*/

#ifndef PPGE_COMMON_LAYOUTS
#define PPGE_COMMON_LAYOUTS

struct VertexIn
{
	float3 PosL       : POSITION;
    float3 Normal     : NORMAL;
    float3 Tangent    : TANGENT;
    float3 Bitangent  : BITANGENT;
    uint   Color      : COLOR;
    float2 TexCoord0  : TEXTURE0;
    float2 TexCoord1  : TEXTURE1;
    float2 TexCoord2  : TEXTURE2;
};

struct PixelIn
{
	float4 PosH      : SV_POSITION;
	float3 PosL      : POSITION;
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Bitangent : BITANGENT;
    float4 Color     : COLOR;
    float2 TexCoord0 : TEXTURE0;
    float2 TexCoord1 : TEXTURE1;
    float2 TexCoord2 : TEXTURE2;
};

struct VertexColorIn
{
    float3 PosL  : POSITION;
    uint   Color : COLOR;
};

struct PixelColorIn
{
    float4 PosH  : SV_POSITION;
    float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexNormalColorIn
{
    float3 PosL   : POSITION;
    float3 Normal : NORMAL;
    uint   Color : COLOR;
};

struct PixelNormalColorIn
{
    float4 PosH   : SV_POSITION;
    float3 PosL   : POSITION;
    float3 Normal : NORMAL;
    float4 Color  : COLOR;
};

struct VertexTexturedIn
{
    float3 PosL      : POSITION;
    float3 Normal    : NORMAL;
    float2 TexCoord0 : TEXTURE0;
};

struct PixelTexturedIn
{
    float4 PosH      : SV_POSITION;
    float3 PosL      : POSITION;
    float3 Normal    : NORMAL;
    float2 TexCoord0 : TEXTURE0;
};

#endif // PPGE_COMMON_LAYOUTS
