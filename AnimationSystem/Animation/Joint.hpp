#ifndef JOINT_HPP
#define JOINT_HPP

#include <algorithm>
#include <Base/Math.hpp>
#include <string>

namespace AnimationSystem
{
    class Joint
    {
    public:
        Joint(){}
        Joint(const char* name): _name(name){}
        void setParentId(uint32_t parentId){_parentId = parentId;}
        void setInvTPose(simd::float4x4 invTPose){_invTPose = invTPose;}
        void setName(std::string name){_name = name;}
        [[nodiscard]] const uint32_t getParentId() const noexcept { return _parentId; }
        [[nodiscard]] const simd::float4x4 getInverseTPose() const noexcept { return _invTPose; }
        [[nodiscard]] const std::string getName() const noexcept {return _name;}

    private:
        simd::float4x4 _invTPose{}; // inverse of the T pose(i.e., bind pose)
        uint32_t _parentId{0};                         // parent
        std::string _name{};
    };
} // namespace AnimationSystem::Base

#endif
