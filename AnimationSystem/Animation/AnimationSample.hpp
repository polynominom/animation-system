#ifndef ANIMATIONSAMPLE_HPP
#define ANIMATIONSAMPLE_HPP

#include "JointPose.hpp"
#include <vector>

namespace AnimationSystem
{
    class AnimationSample
    {
    public:
        AnimationSample(std::vector<JointPose> poses) : _jointPoses(std::move(poses)){};
        [[nodiscard]] JointPose getPose(int i);

    private:
        [[no_unique_address]] std::vector<JointPose> _jointPoses;
    };
} // namespace AnimationSystem

#endif
