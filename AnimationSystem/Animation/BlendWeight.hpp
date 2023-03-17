#ifndef BLEND_WEIGHT_HPP
#define BLEND_WEIGHT_HPP

#include <JointMask.hpp>

namespace AnimationSystem
{
class BlendWeight
{
public:
    static float getBlendWeight(const float weight, const JointMask *pJointMask, const int jointId);
    virtual ~BlendWeight();
};

class BlendWeight1D : BlendWeight
{
public:
    BlendWeight1D(int w) : _position(w) {}
    ~BlendWeight1D();
    int getPosition() const { return _position; }

private:
    int _position;
};

class BlendWeight2D : BlendWeight
{
public:
    BlendWeight2D(int w1, int w2) : _blendPosition1(w1), _blendPosition2(w2) {}
    ~BlendWeight2D();
    int getPosition(int i) const
    {
        assert(i == 0 || i == 1);
        return i == 0 ? _blendPosition1 : _blendPosition2;
    }

private:
    int _blendPosition1;
    int _blendPosition2;
};
} // namespace AnimationSystem

#endif
