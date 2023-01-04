/*  Helper functions to encode/decode RGBA colors and 
    color space conversions
*/

#ifndef PPGE_COLOR_HELPERS
#define PPGE_COLOR_HELPERS

float4 Decode32bitRGBA(uint color)
{
    float r = ((color >> 24) & 0xff) / 255.0f;
    float g = ((color >> 16) & 0xff) / 255.0f;
    float b = ((color >>  8) & 0xff) / 255.0f;
    float a = ((color      ) & 0xff) / 255.0f;

    return float4(r, g, b, a);
}

inline bool IsSaturated(float  val) { return val == saturate(val); }
inline bool IsSaturated(float2 val) { return IsSaturated(val.x)   && IsSaturated(val.y); } 
inline bool IsSaturated(float3 val) { return IsSaturated(val.xy)  && IsSaturated(val.z); } 
inline bool IsSaturated(float4 val) { return IsSaturated(val.xyz) && IsSaturated(val.w); } 

inline float3 degamma(float3 color, float gamma_val) { return pow(abs(color),        gamma_val); }
inline float3 gamma  (float3 color, float gamma_val) { return pow(abs(color), 1.0f / gamma_val); }

#endif // PPGE_COLOR_HELPERS
