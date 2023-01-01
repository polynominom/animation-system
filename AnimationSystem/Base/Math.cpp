#include "Math.hpp"

namespace AnimationSystem
{
    using simd::float4;

    simd::float3 Math::add(const simd::float3 &a, const simd::float3 &b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }

    simd_float4x4 Math::makeIdentity()
    {
        return (simd_float4x4){(float4){1.f, 0.f, 0.f, 0.f},
                               (float4){0.f, 1.f, 0.f, 0.f},
                               (float4){0.f, 0.f, 1.f, 0.f},
                               (float4){0.f, 0.f, 0.f, 1.f}};
    }

    simd::float4x4 Math::makePerspective(float fov, float aspect, float znear, float zfar)
    {
        float ys = 1.0f / tanf(fov * 0.5f);
        float xs = ys / aspect;
        float zs = zfar / (znear - zfar);
        return simd_matrix_from_rows(
            (float4){xs, 0.0f, 0.0f, 0.0f},
            (float4){0.0f, ys, 0.0f, 0.0f},
            (float4){0.0f, 0.0f, zs, znear * zs},
            (float4){0.0f, 0.0f, -1.0f, 0.0f});
    }

    simd::float4x4 Math::rotateX(float angleRadians)
    {
        const float a = angleRadians;
        return simd_matrix_from_rows((float4){1.0f, 0.0f, 0.0f, 0.0f},
                                     (float4){0.0f, cosf(a), sinf(a), 0.0f},
                                     (float4){0.0f, -sinf(a), cosf(a), 0.0f},
                                     (float4){0.0f, 0.0f, 0.0f, 1.0f});
    }

    simd::float4x4 Math::rotateY(float angleRadians)
    {
        const float a = angleRadians;
        return simd_matrix_from_rows((float4){cosf(a), 0.0f, sinf(a), 0.0f},
                                     (float4){0.0f, 1.0f, 0.0f, 0.0f},
                                     (float4){-sinf(a), 0.0f, cosf(a), 0.0f},
                                     (float4){0.0f, 0.0f, 0.0f, 1.0f});
    }

    simd::float4x4 Math::rotateZ(float angleRadians)
    {

        const float a = angleRadians;
        return simd_matrix_from_rows((float4){cosf(a), sinf(a), 0.0f, 0.0f},
                                     (float4){-sinf(a), cosf(a), 0.0f, 0.0f},
                                     (float4){0.0f, 0.0f, 1.0f, 0.0f},
                                     (float4){0.0f, 0.0f, 0.0f, 1.0f});
    }

    simd::float4x4 Math::translate(const simd::float3 &v)
    {
        const float4 col0 = {1.0f, 0.0f, 0.0f, 0.0f};
        const float4 col1 = {0.0f, 1.0f, 0.0f, 0.0f};
        const float4 col2 = {0.0f, 0.0f, 1.0f, 0.0f};
        const float4 col3 = {v.x, v.y, v.z, 1.0f};
        return simd_matrix(col0, col1, col2, col3);
    }

    simd::float4x4 Math::scale(const simd::float3 &v)
    {
        return simd_matrix(
            (float4){v.x, 0, 0, 0},
            (float4){0, v.y, 0, 0},
            (float4){0, 0, v.z, 0},
            (float4){0, 0, 0, 1});
    }

    simd::float3x3 Math::discardTranslation(const simd::float4x4 &m)
    {
        return simd_matrix(m.columns[0].xyz, m.columns[1].xyz, m.columns[2].xyz);
    }
} // namespace AnimationSystem