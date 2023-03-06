#include "Math.hpp"


namespace AnimationSystem
{
    using simd::float4;

    simd::float3 Math::convertAssimpV(const aiVector3D& from)
    {
        return simd_make_float3(from.x, from.y, from.z);
    }
    
    simd::float4x4 Math::convertAssimpMt(const aiMatrix4x4t<float>& from)
    {
        return simd_matrix_from_rows(simd_make_float4(from.a1, from.a2, from.a3, from.a4),
                              simd_make_float4(from.b1, from.b2, from.b3, from.b4),
                              simd_make_float4(from.c1, from.c2, from.c3, from.c4),
                              simd_make_float4(from.d1, from.d2, from.d3, from.d4));
    }

    simd::float4x4 Math::convertAssimpM(const aiMatrix4x4 from)
    {
        return simd_matrix_from_rows(simd_make_float4(from.a1, from.a2, from.a3, from.a4),
                              simd_make_float4(from.b1, from.b2, from.b3, from.b4),
                              simd_make_float4(from.c1, from.c2, from.c3, from.c4),
                              simd_make_float4(from.d1, from.d2, from.d3, from.d4));
    }

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

simd::float4x4 Math::scaleM(const simd::float3 &v)
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
    
    simd::float3 Math::scale(const simd::float4x4 &_m)
    {
        return simd::float3{_m.columns[0].x,_m.columns[1].y,_m.columns[2].z};
    }

    simd::float3 Math::translation(const simd::float4x4 &_m)
    {
        return simd::float3{_m.columns[3].x,_m.columns[3].y,_m.columns[3].z};
    }

    simd::quatf Math::rotation(const simd::float4x4 &_m)
    {
        auto rot = simd::float3x3(_m.columns[0].xyz, _m.columns[1].xyz, _m.columns[2].xyz);
        return simd::quatf(rot);
    }

} // namespace AnimationSystem
