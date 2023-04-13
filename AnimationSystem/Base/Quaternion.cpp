#include "Quaternion.hpp"

namespace AnimationSystem::Base
{
    // TODO: finish implementation, for now use simd from metal framework
    Quaternion::Quaternion(float x, float y, float z, float w) : _values(simd::float4{x, y, z, w}), _normalized(simd::normalize(_values))
    {
    }

    Quaternion::Quaternion(float x, float y, float z) : Quaternion::Quaternion(x, y, z, 1)
    {
    }

} // namespace AnimationSystem
