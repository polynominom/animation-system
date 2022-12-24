#pragma once
#include <algorithm>
#include <Base/Math.hpp>

namespace AnimationSystem
{
    class Joint
    {
        Joint();
        [[nodiscard]] const uint getParentId() const noexcept {return _parentId;}
        [[nodiscard]] const simd::float4x3 getInverseTPose() const noexcept {return _invTPose;}
        private:
            [[no_unique_address]] simd::float4x3 _invTPose; // inverse of the bind pose(i.e., bind pose)
            uint _parentId; // parent id
            //std::string / const char* name?
    };
} // namespace AnimationSystem::Base