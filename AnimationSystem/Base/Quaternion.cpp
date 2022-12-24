#include "Queternion.hpp"

namespace AnimationSystem::Base
{
    // TODO: finish implementation
    Quaternion::Quaternion(float x, float y, float z, float w) : _values(simd::float4{x, y, z, w}), _normalize(simd::normalize(_values))
    {
    }

    Quaternion::Quaternion(float x, float y, float z) : Quaternion::Quaternion(x, y, z, 1)
    {
    }

} // namespace AnimationSystem
