#include <BlendThreshold.hpp>

namespace AnimationSystem
{
    float BlendThreshold::getBlendThreshold(const float weight, const JointMask *pJointMask, const int jointId)
    {
        return weight * pJointMask->at(jointId);
    }
} // namespace AnimationSystem
