#pragma once
#include "Joint.hpp"
#include <vector>

namespace AnimationSystem
{
    class Skeleton
    {
        public:
            Skeleton(const std::vector<Joint> &joints): _joints(std::move(joints)){}
            [[nodiscard]] const unsigned long  jointCount() const { return _joints.size();}
        private:
            [[no_unique_address]] std::vector<Joint> _joints{};
    };
}