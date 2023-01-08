#ifndef SKINNEDVERTEX_HPP
#define SKINNEDVERTEX_HPP

#include <vector>
#include <iostream>
#include <simd/simd.h>

namespace AnimationSystem
{
    class SkinnedVertex
    {
    public:
        // Vertex variables
        simd::float3 _position;
        simd::float3 _normal;
        simd::float2 _texCoord;
        // Weights of each bounded joint (last one is omitted due to they all sum up to 1).
        simd::float3 _jointWeights{0.f,0.f,0.f};
        // One vertex can bounded up to 4 joint.
        simd::int4 _jointIndex{-1,-1,-1,-1};
    };
} // namespace AnimationSystem

#endif
