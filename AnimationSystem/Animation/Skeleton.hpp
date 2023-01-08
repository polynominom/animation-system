#ifndef SKELETON_HPP
#define SKELETON_HPP

#include "Joint.hpp"
#include <vector>
#include <memory>

namespace AnimationSystem
{
    class Skeleton
    {
    public:
        Skeleton(){}
        [[nodiscard]] const unsigned long jointCount() const { return _joints.size(); }
        void addJoint(const std::shared_ptr<Joint>& joint){_joints.push_back(joint);}
        void setJoints(std::vector<std::shared_ptr<Joint>> &&joints){_joints = std::move(joints);}
        void initJoints(size_t size){_joints.resize(size);}
        [[nodiscard]] std::shared_ptr<Joint> getJoint(uint i){return _joints[i];}

    private:
        std::vector<std::shared_ptr<Joint>> _joints{};
    };
}
#endif
