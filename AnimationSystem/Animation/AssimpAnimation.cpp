#include "AssimpAnimation.hpp"
#include <type_traits>
#include <stack>
#include <iostream>

namespace AnimationSystem
{
    namespace
    {
        template<typename T>
        uint findIndex(float timeTick, const std::vector<T>& arr)
        {
//            auto it = std::find_if(std::begin(arr), std::end(arr), [&timeTick](const T& t1){
//                return timeTick < (float)t1.first;
//            });
            for(uint i = 0; i < arr.size() - 1; ++i)
            {
                float t = (float)arr[i+1].first;
                if(timeTick < t){
                    return i;
                }
            }
            return 0;
        }
        
        template<typename T>
        float compLerpFactor(float timeTick, uint currentId, const std::vector<T>& arr)
        {
            float t1 = (float) arr[currentId].first;
            float t2 = (float) arr[currentId + 1].first;
            float delta = t2 - t1;
            float factor = (timeTick - float(t1))/delta;
            if(factor >= 0.0f && factor <= 1.0f)
                return factor;
            return 0;
        }
    
        simd::float3 lerp(float timeTick, const std::vector<TimeVec3Pair> &keys)
        {
            // no need to compute or lerp if only one channel exist.
            if(keys.size() == 1)
            {
                return keys[0].second;
            }
            
            uint currentId = findIndex<TimeVec3Pair>(timeTick, keys);
            float lerpFactor = compLerpFactor<TimeVec3Pair>(timeTick, currentId, keys);
            return simd::mix<simd::float3>(keys[currentId].second, keys[currentId+1].second, lerpFactor);
        }

        /**
         Assigns the keys from assimp animation to channel's vector
        Args:
        length: length of the given array
        aKeys: key array of the assimp animation
         */

        TimeVec3Pair make_channel_pair(const aiVectorKey& key)
        {
            return std::make_pair(key.mTime, simd_make_float3(key.mValue.x,key.mValue.y,key.mValue.z));
        }
    
        TimeQuatPair make_channel_pair(const aiQuatKey& key)
        {
            return std::make_pair(key.mTime, (simd::quatf){key.mValue.x,key.mValue.y,key.mValue.z,key.mValue.w});
        }
    
        template<typename T, typename R>
        void assignList(unsigned int length, const T* aKeys, std::vector<R> &assignableList)
        {
            assignableList.resize(length);
            for (int i = 0; i < length; ++i)
            {
                auto &key = aKeys[i];
                assignableList[i] = make_channel_pair(key);
            }
        }
        
    }

    AssimpAnimation::AssimpAnimation(const aiAnimation *pAnim): _duration(pAnim->mDuration), _tickPerSecond(pAnim->mTicksPerSecond)
    {
        for (int a = 0; a < pAnim->mNumChannels; ++a)
        {
            auto &assimpChannel = pAnim->mChannels[a];
            
            // get joint name of animation
            std::string jName(assimpChannel->mNodeName.C_Str());
            
            AssimpAnimationChannel channel;
            assignList<aiQuatKey, TimeQuatPair>(assimpChannel->mNumRotationKeys, assimpChannel->mRotationKeys, channel.rotations);
            assignList<aiVectorKey, TimeVec3Pair>(assimpChannel->mNumPositionKeys, assimpChannel->mPositionKeys, channel.positions);
            assignList<aiVectorKey, TimeVec3Pair>(assimpChannel->mNumScalingKeys, assimpChannel->mScalingKeys, channel.scales);
            
            // TODO: check if this fails
            _channelMap[jName] = std::move(channel);
        }
    }

    // Iterates connected nodes recursively to fill the children nodes.
    void AssimpAnimation::_fill(const aiNode *node)
    {
        std::stack<const aiNode *> nodeStack;
        nodeStack.push(node);
        
        while(!nodeStack.empty())
        {
            
            auto currentNode = nodeStack.top();
            nodeStack.pop();
            
            // add every initial transformation ignoring children count.
            _nodeToInitialTransformationMap[currentNode->mName.C_Str()] = Math::convertAssimpM(currentNode->mTransformation);
            if(currentNode->mNumChildren == 0)
                continue;
            
            auto it = _jointParentMap.find(currentNode->mName.C_Str());
            if(it == _jointParentMap.end())
                _jointParentMap.insert(std::make_pair(currentNode->mName.C_Str(), std::vector<std::string>(currentNode->mNumChildren)));
                                       
            for(int i = 0; i < currentNode->mNumChildren; ++i)
            {
                auto child = currentNode->mChildren[i];
                _jointParentMap[currentNode->mName.C_Str()][i] = child->mName.C_Str();
                nodeStack.push(child);
            }
        }        
    }

    void AssimpAnimation::initJointParentMap(const aiNode* root)
    {
        _rootName = std::string(root->mName.C_Str());
        auto rootTrans = root->mTransformation;
        //_globalInverseTransformation = Math::convertAssimpM(rootTrans.Inverse());
        
        _fill(root);
    }

    /***
     *Linearly interpolates rotation values for given time
     */
    simd::quatf AssimpAnimation::compRotation(float timeTick, const std::string &name)
    {
        auto it = _channelMap.find(name);
        if(it == _channelMap.end())
        {
            return simd::quatf{0.0f, .0f, .0f, 1.f};
        }
        
        // no need to compute or lerp if only one rotation channel exist.
        if(it->second.rotations.size() == 1)
        {
            return it->second.rotations[0].second;
        }
        
        uint currentId = findIndex<TimeQuatPair>(timeTick, it->second.rotations);
        float lerpFactor = compLerpFactor<TimeQuatPair>(timeTick, currentId, it->second.rotations);
        return simd_slerp(it->second.rotations[currentId].second, it->second.rotations[currentId+1].second, lerpFactor);
    }
    
    /***
     *Linearly interpolates scale values for given time
     */
    simd::float3 AssimpAnimation::compScale(float timeTick, const std::string& name)
    {
        auto it = _channelMap.find(name);
        if(it == _channelMap.end())
        {
            return simd_make_float3(1.f, 1.f, 1.f);
        }
        
        return lerp(timeTick, it->second.scales);
    }

    /***
     *Linearly interpolates position values for given time
     */
    simd::float3 AssimpAnimation::compPosition(float timeTick, const std::string& name)
    {
        auto it = _channelMap.find(name);
        if(it == _channelMap.end())
        {
            return simd_make_float3(0.f, 0.f, 0.f);
        }
        
        return lerp(timeTick, it->second.positions);
    }
} // namespace AnimationSystem
