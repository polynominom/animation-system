#include <BlendWeight.hpp>

namespace AnimationSystem
{
    float BlendWeight::getBlendWeight(const float weight, const JointMask *pJointMask, const int jointId)
    {
        return weight * pJointMask->at(jointId);
    }
} // namespace AnimationSystem
