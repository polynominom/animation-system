/*  Helper functions for math
*/

#ifndef PPGE_MATH_HELPERS
#define PPGE_MATH_HELPERS

float VectorMin(float2 vec) { return min(vec.x, vec.y); }
float VectorMin(float3 vec) { return min(VectorMin(vec.xy), vec.z); }
float VectorMin(float4 vec) { return min(VectorMin(vec.xyz), vec.w); }

float VectorMax(float2 vec) { return max(vec.x, vec.y); }
float VectorMax(float3 vec) { return max(VectorMax(vec.xy), vec.z); }
float VectorMax(float4 vec) { return max(VectorMax(vec.xyz), vec.w); }

int VectorMin(int2 vec) { return min(vec.x, vec.y); }
int VectorMin(int3 vec) { return min(VectorMin(vec.xy), vec.z); }
int VectorMin(int4 vec) { return min(VectorMin(vec.xyz), vec.w); }

int VectorMax(int2 vec) { return max(vec.x, vec.y); }
int VectorMax(int3 vec) { return max(VectorMax(vec.xy), vec.z); }
int VectorMax(int4 vec) { return max(VectorMax(vec.xyz), vec.w); }

#endif // PPGE_MATH_HELPERS
