#ifndef BLEND_WEIGHT_HPP
#define BLEND_WEIGHT_HPP

#include <JointMask.hpp>

namespace AnimationSystem
{
class BlendThreshold
{
public:
    static float getBlendThreshold(const float weight, const JointMask *pJointMask, const int jointId);
    virtual ~BlendThreshold();
};

class BlendThreshold1D : BlendThreshold
{
public:
    BlendThreshold1D(int w) : _position(w) {}
    ~BlendThreshold1D();
    int getPosition() const { return _position; }

private:
    int _position;
};

class BlendThreshold2D : BlendThreshold
{
public:
    BlendThreshold2D(int w1, int w2) : _blendPosition1(w1), _blendPosition2(w2) {}
    ~BlendThreshold2D();
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
