#ifndef BLEND_OPERATIONS_HPP
#define BLEND_OPERATIONS_HPP

#include "BlendTree.hpp"
#include "SkeletonPose.hpp"
#include <memory>
#include <vector>

namespace AnimationSystem
{
    class BlendOperationHandler
    {
    public:
        static std::unique_ptr<SkeletonPose> handle(BlendNode &node1, BlendNode &node2, float time);
    };
}

#endif
