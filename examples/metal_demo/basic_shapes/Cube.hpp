#pragma once
#include <ShaderTypes.hpp>
#include <array>

namespace AnimationSystem
{
    namespace Shapes
    {
        using f3 = simd_float3;
        struct Cube
        {

            ShaderTypes::VertexData verts[24];
            uint16_t indices[36];

            constexpr Cube(const float s) : verts()
            {
                // +,+,+ (plus, plus, plus)
                const auto ppp = (f3){+s, +s, +s};
                // -,-,- (minus, minus, minus)
                const auto mmm = (f3){-s, -s, -s};

                // one minues 2 pluses
                const auto mpp = (f3){-s, +s, +s};
                const auto pmp = (f3){+s, -s, +s};
                const auto ppm = (f3){+s, +s, -s};

                const auto mmp = (f3){-s, -s, +s};
                const auto mpm = (f3){-s, +s, -s};
                const auto pmm = (f3){+s, -s, -s};

                const auto normalX = (f3){1.f, 0.f, 0.f};
                const auto nNormalX = (f3){-1.f, 0.f, 0.f};

                const auto normalY = (f3){0.f, 1.f, 0.f};
                const auto nNormalY = (f3){0.f, -1.f, 0.f};

                const auto normalZ = (f3){0.f, 0.f, 1.f};
                const auto nNormalZ = (f3){0.f, 0.f, -1.f};

                verts[0] = ShaderTypes::VertexData{mmp, normalZ},
                verts[1] = ShaderTypes::VertexData{pmp, normalZ},
                verts[2] = ShaderTypes::VertexData{ppp, normalZ},
                verts[3] = ShaderTypes::VertexData{mpp, normalZ},
                verts[4] = ShaderTypes::VertexData{pmp, normalX},
                verts[5] = ShaderTypes::VertexData{pmm, normalX},
                verts[6] = ShaderTypes::VertexData{ppm, normalX},
                verts[7] = ShaderTypes::VertexData{ppp, normalX},
                verts[8] = ShaderTypes::VertexData{pmm, nNormalZ},
                verts[9] = ShaderTypes::VertexData{mmm, nNormalZ},
                verts[10] = ShaderTypes::VertexData{mpm, nNormalZ},
                verts[11] = ShaderTypes::VertexData{ppm, nNormalZ},
                verts[12] = ShaderTypes::VertexData{mmm, nNormalX},
                verts[13] = ShaderTypes::VertexData{mmp, nNormalX},
                verts[14] = ShaderTypes::VertexData{mpp, nNormalX},
                verts[15] = ShaderTypes::VertexData{mpm, nNormalX},
                verts[16] = ShaderTypes::VertexData{mpp, normalY},
                verts[17] = ShaderTypes::VertexData{ppp, normalY},
                verts[18] = ShaderTypes::VertexData{mpm, normalY},
                verts[19] = ShaderTypes::VertexData{ppm, normalY},
                verts[20] = ShaderTypes::VertexData{mmm, nNormalY},
                verts[21] = ShaderTypes::VertexData{pmm, nNormalY},
                verts[22] = ShaderTypes::VertexData{pmp, nNormalY},
                verts[23] = ShaderTypes::VertexData{mmp, nNormalY};
            }

            constexpr std::array<uint16_t, 36> getIndices()
            {
                return {
                    0, 1, 2, 2, 3, 0,       /* front */
                    4, 5, 6, 6, 7, 4,       /* right */
                    8, 9, 10, 10, 11, 8,    /* back */
                    12, 13, 14, 14, 15, 12, /* left */
                    16, 17, 18, 18, 19, 16, /* top */
                    20, 21, 22, 22, 23, 20, /* bottom */
                };
            }
        };

    }

}