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
        Skeleton() {}
        [[nodiscard]] const unsigned long jointCount() const { return _joints.size(); }
        void addJoint(std::unique_ptr<Joint> &joint) { _joints.emplace_back(std::move(joint)); }
        void setJoints(std::vector<std::unique_ptr<Joint>> &&joints) { _joints = std::move(joints); }
        void initJoints(size_t size) { _joints.resize(size); }
        [[nodiscard]] const Joint* getJoint(uint i) { return _joints[i].get(); }

    private:
        std::vector<std::unique_ptr<Joint>> _joints{};
    };
}
#endif
