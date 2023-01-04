#ifndef __COMMON_LAYOUTS_METAL
#define __COMMON_LAYOUTS_METAL

#include <metal_stdlib>

struct VertexIn
{
	float3 PosL       : [[attribute(0)]];
    float3 Normal     : [[attribute(1)]];
    float3 Tangent    : [[attribute(2)]];
    float3 Bitangent  : [[attribute(3)]];
    uint   Color      : [[attribute(4)]];
    float2 TexCoord0  : [[texture(0)]];
    float2 TexCoord1  : [[texture(1)]];
    float2 TexCoord2  : [[texture(2)]];
};

struct PixelIn
{
	float4 PosH      : SV_POSITION;
	float3 PosL      : [[position]];
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Bitangent : BITANGENT;
    float4 Color     : COLOR;
    float2 TexCoord0 : [[texture(0)]];
    float2 TexCoord1 : [[texture(1)]];
    float2 TexCoord2 : [[texture(2)]];
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

#endif