#ifndef MATH_HPP
#define MATH_HPP

#include <simd/simd.h>
#include <math.h>
#include <assimp/postprocess.h>

namespace AnimationSystem
{
    class Math
    {
    public:
        static simd::float3 add(const simd::float3 &a, const simd::float3 &b);
        static simd_float4x4 makeIdentity();
        static simd::float4x4 makePerspective(float fov, float aspect, float znear, float zfar);
        static simd::float4x4 rotateX(float angleRadians);
        static simd::float4x4 rotateY(float angleRadians);
        static simd::float4x4 rotateZ(float angleRadians);
        static simd::float4x4 translate(const simd::float3 &v);
        static simd::float4x4 scale(const simd::float3 &v);
        static simd::float3x3 discardTranslation(const simd::float4x4 &m);
        // convertions
        static simd::float4x4 convertAssimpM(const aiMatrix4x4 from);
        static simd::float4x4 convertAssimpMt(const aiMatrix4x4t<float>& from);
        static simd::float3 convertAssimpV(const aiVector3D& from);
        };
} // namespace AnimationSystem
#endif
