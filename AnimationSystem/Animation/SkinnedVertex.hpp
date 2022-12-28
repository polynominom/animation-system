#pragma once
#include <vector>
#include <iostream>
#include <simd/simd.h>

namespace AnimationSystem
{
    class SkinnedVertex
    {
    private:
        // Vertex variables
        [[no_unique_address]] simd::float3 _position;
        [[no_unique_address]] simd::float3 _normal;
        [[no_unique_address]] simd::float2 _texCoord;
        // Weights of each bounded joint (last one is omitted due to they all sum up to 1).
        [[no_unique_address]] simd::float3 _jointWeights;
        // One vertex can bounded up to 4 joint.
        [[no_unique_address]] simd::packed::uint4 _jointIndex;
    };
} // namespace AnimationSystem
