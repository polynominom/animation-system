#ifndef CUBE_HPP
#define CUBE_HPP

#include <Renderer/Shader/ShaderTypes.hpp>

namespace AnimationSystem
{
    namespace Shapes
    {
        using f3 = simd_float3;
        using simd::float2;
        struct Cube
        {
            Cube(float size);
            ShaderTypes::VertexData verts[24];
            uint16_t indices[36];

        private:
            float _size;
        };

    }

}
#endif