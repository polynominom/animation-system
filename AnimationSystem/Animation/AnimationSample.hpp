#ifndef ANIMATIONSAMPLE_HPP
#define ANIMATIONSAMPLE_HPP

#include "JointPose.hpp"

#include <string>
#include <vector>
#include <memory>

namespace AnimationSystem
{
    /**
     * Animation sample for time t that has the joint poses on that time.
     */
    class AnimationSample
    {
    public:
        AnimationSample(size_t jointCount){_jointPoses.resize(jointCount);}
        AnimationSample(std::vector<std::unique_ptr<JointPose>> poses) : _jointPoses(std::move(poses)){};
        void setJointPoses(std::vector<std::unique_ptr<JointPose>>&& jointPoses){_jointPoses = std::move(jointPoses);}
        [[nodiscard]] JointPose getPose(int i);

    private:
        // skeleton joints
        std::vector<std::unique_ptr<JointPose>> _jointPoses;
    };
} // namespace AnimationSystem

#endif
