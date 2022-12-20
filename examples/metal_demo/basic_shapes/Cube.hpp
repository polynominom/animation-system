#pragma once
#include <ShaderTypes.hpp>
#include <array>

namespace AnimationSystem
{
    namespace Shapes
    {
        using f3 = simd_float3;
        using simd::float2;
        template <int r, int d>
        struct Cube
        {
            constexpr static float s = (float)r / d;
            ShaderTypes::VertexData verts[24] = {
                //   Positions          Normals
                {{-s, -s, +s}, {0.f, 0.f, 1.f}, (float2){0.f, 1.f}},
                {{+s, -s, +s}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
                {{+s, +s, +s}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
                {{-s, +s, +s}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
                {{+s, -s, +s}, {1.f, 0.f, 0.f}, {0.f, 1.f}},
                {{+s, -s, -s}, {1.f, 0.f, 0.f}, {1.f, 1.f}},
                {{+s, +s, -s}, {1.f, 0.f, 0.f}, {1.f, 0.f}},
                {{+s, +s, +s}, {1.f, 0.f, 0.f}, {0.f, 0.f}},
                {{+s, -s, -s}, {0.f, 0.f, -1.f}, {0.f, 1.f}},
                {{-s, -s, -s}, {0.f, 0.f, -1.f}, {1.f, 1.f}},
                {{-s, +s, -s}, {0.f, 0.f, -1.f}, {1.f, 0.f}},
                {{+s, +s, -s}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
                {{-s, -s, -s}, {-1.f, 0.f, 0.f}, {0.f, 1.f}},
                {{-s, -s, +s}, {-1.f, 0.f, 0.f}, {1.f, 1.f}},
                {{-s, +s, +s}, {-1.f, 0.f, 0.f}, {1.f, 0.f}},
                {{-s, +s, -s}, {-1.f, 0.f, 0.f}, {0.f, 0.f}},
                {{-s, +s, +s}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
                {{+s, +s, +s}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
                {{+s, +s, -s}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
                {{-s, +s, -s}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
                {{-s, -s, -s}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
                {{+s, -s, -s}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
                {{+s, -s, +s}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
                {{-s, -s, +s}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
            };

            uint16_t indices[36] = {
                0, 1, 2, 2, 3, 0,       /* front */
                4, 5, 6, 6, 7, 4,       /* right */
                8, 9, 10, 10, 11, 8,    /* back */
                12, 13, 14, 14, 15, 12, /* left */
                16, 17, 18, 18, 19, 16, /* top */
                20, 21, 22, 22, 23, 20, /* bottom */
            };
        };

    }

}