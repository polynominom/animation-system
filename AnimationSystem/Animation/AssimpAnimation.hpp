#ifndef __ASSIMP_ANIMATION__HPP
#define __ASSIMP_ANIMATION__HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include <utility>
#include <simd/simd.h>
#include <assimp/anim.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <Base/Math.hpp>

namespace AnimationSystem
{

    using TimeQuatPair = std::pair<double, simd::quatf>;
    using TimeVec3Pair = std::pair<double, simd::float3>;

    /**
     * Represents one joint's animation in assimp structure
     */
    class AssimpAnimationChannel
    {
    public:
        std::vector<TimeQuatPair> rotations{};
        std::vector<TimeVec3Pair> positions{};
        std::vector<TimeVec3Pair> scales{};
    };

    /**
     * Structure that represent whole animation that effect certain joints
     */
    class AssimpAnimation
    {
    public:
        AssimpAnimation(const aiAnimation *pAnim);
        void initJointParentMap(const aiNode *root);
        std::vector<std::string> getChildList(std::string name) const
        {
            auto it = _jointParentMap.find(name);
            if(it != _jointParentMap.end())
                return it->second;
            return {};
        }
        
        std::string getRootNodeName(){return _rootName;}
        const float getTickPerSecond() const noexcept { return _tickPerSecond; }
        const float getDuration() const noexcept { return _duration; }
        simd::quatf compRotation(float timeTick, const std::string &name);
        simd::float3 compPosition(float timeTick, const std::string &name);
        simd::float3 compScale(float timeTick, const std::string &name);
        const float getTimeTick(float timeInSec) const noexcept
        {
            float tps = (float) _tickPerSecond != 0 ? _tickPerSecond:25.0f;
            return fmod(timeInSec * tps, (float)_duration);
        }
        
        const bool nameExist(const std::string &name) const noexcept {return _channelMap.find(name) != _channelMap.end();}
        
        const simd::float4x4 getInitialTransformation(const std::string &name) const noexcept
        {
            auto it = _nodeToInitialTransformationMap.find(name);
            assert(it != _nodeToInitialTransformationMap.end());
            return it->second;
            
        }

    private:
        void _fill(const aiNode *node);
        float _tickPerSecond{0.0f};
        float _duration{0.0f};
        std::string _rootName{""};
        std::unordered_map<std::string, AssimpAnimationChannel> _channelMap;
        
        // child joint name to parent name map
        std::unordered_map<std::string, std::vector<std::string>> _jointParentMap;
        std::unordered_map<std::string, simd::float4x4> _nodeToInitialTransformationMap;
    };
} // namespace AnimationSystem

#endif
