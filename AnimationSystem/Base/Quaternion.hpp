#pragma once
#include <Base/Math.hpp>

namespace AnimationSystem::Base
{
    /**
     * Queternion class to represent rotations ( x y z w )
     * 
     * Normalized and eulerAngle representation are holded by the class as well.
    */
    class Quaternion
    {
        public:
            Quaternion(float x, float y, float z);
            Quaternion(float x, float y, float z, float w);

            [[nodiscard]] const simd::float4 getValues() const noexcept {return _values;}
            [[nodiscard]] const simd::float4 getNormalized() const noexcept {return _normalized;}
            [[nodiscard]] const simd::float3 getEulerAngles() const noexcept {return _eulerAngles;}

            [[nodiscard]] const float x() const noexcept {return _values.x; };
            [[nodiscard]] const float y() const noexcept {return _values.y; };
            [[nodiscard]] const float z() const noexcept {return _values.z; };
            [[nodiscard]] const float w() const noexcept {return _values.z; };
        private:
            [[no_unique_address]] simd::float4 _values{0.,0.,0.,0.};
            [[no_unique_address]] simd::float4 _normalized{0., 0., 0., 0.};
            [[no_unique_address]] simd::float3 _eulerAngles{0., 0., 0.};
    };
} // namespace AnimationSystem::Base
