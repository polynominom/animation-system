#include "AssimpAnimationHandler.hpp"
#include <Math.hpp>
#include <vector>
#include <Core/CommonConsts.hpp>

#define ASSIMP_LOAD_FLAGS_ANIM_HANDLER (aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded)

#define MAX_NUM_BONES_PER_VERTEX 4

namespace AnimationSystem
{
    namespace
    {
        template<typename T>
        uint findIndex(float timeTick, unsigned int length, T *arr)
        {
            for(uint i = 0; i < length - 1; ++i)
            {
                float t = (float)arr[i+1].mTime;
                if(timeTick < t){
                    return i;
                }
            }
            return 0;
        }
        
        template<typename T>
        float compLerpFactor(float timeTick, uint currentId, T *arr)
        {
            float t1 = (float) arr[currentId].mTime;
            float t2 = (float) arr[currentId + 1].mTime;
            float delta = t2 - t1;
            float factor = (timeTick - float(t1))/delta;
            if(factor >= 0.0f && factor <= 1.0f)
                return factor;
            return 0;
        }
    
        simd::float3 lerp(float lerpFactor, uint id, aiVectorKey* arr)
        {
            auto r = arr[id].mValue + lerpFactor * (arr[id+1].mValue- arr[id].mValue);
            // convert assimp vector to simd vector.
            return {r.x, r.y, r.z};
        }
     
    }

    simd::float3 AssimpAnimationHandler::lerpScale(float timeTick, const aiNodeAnim *pNodeAnim)
    {
        // standard linear interpolation to find scale value.
        
        if(pNodeAnim->mNumScalingKeys == 1)
        {
            return {pNodeAnim->mScalingKeys[0].mValue.x, pNodeAnim->mScalingKeys[0].mValue.y, pNodeAnim->mScalingKeys[0].mValue.z};
        }
        
        uint currentId = findIndex<aiVectorKey>(timeTick, pNodeAnim->mNumScalingKeys, pNodeAnim->mScalingKeys);
        assert(currentId+1 < pNodeAnim->mNumScalingKeys);
        
        float lerpFactor = compLerpFactor(timeTick, currentId, pNodeAnim->mScalingKeys);
        return lerp(lerpFactor, currentId, pNodeAnim->mScalingKeys);
    }

    simd::quatf AssimpAnimationHandler::lerpRotation(float timeTick, const aiNodeAnim *pNodeAnim)
    {
        // standard linear interpolation to find rotation value.
        
        if(pNodeAnim->mNumRotationKeys == 1)
        {
            
            return simd::quatf(pNodeAnim->mRotationKeys[0].mValue.x, pNodeAnim->mRotationKeys[0].mValue.y, pNodeAnim->mRotationKeys[0].mValue.z, pNodeAnim->mRotationKeys[0].mValue.w);
        }
        
        uint currentId = findIndex<aiQuatKey>(timeTick, pNodeAnim->mNumRotationKeys, pNodeAnim->mRotationKeys);
        assert(currentId+1 < pNodeAnim->mNumRotationKeys);
        
        float lerpFactor = compLerpFactor(timeTick, currentId, pNodeAnim->mRotationKeys);
        aiQuaternion result;
        aiQuaternion::Interpolate(result,
                                  pNodeAnim->mRotationKeys[currentId].mValue,
                                  pNodeAnim->mRotationKeys[currentId+1].mValue,
                                  lerpFactor);
        result.Normalize();
        
        // convert assimp quaternion to simd quaternion
        return simd::quatf(result.x, result.y, result.z, result.w);
    }

    simd::float3 AssimpAnimationHandler::lerpTranslate(float timeTick, const aiNodeAnim *pNodeAnim)
    {
        // standard linear interpolation to find position(translate) value.
        
        if(pNodeAnim->mNumPositionKeys == 1)
        {
            return {pNodeAnim->mPositionKeys[0].mValue.x, pNodeAnim->mPositionKeys[0].mValue.y, pNodeAnim->mPositionKeys[0].mValue.z};
        }
        
        uint currentId = findIndex<aiVectorKey>(timeTick, pNodeAnim->mNumPositionKeys, pNodeAnim->mPositionKeys);
        assert(currentId + 1 < pNodeAnim->mNumPositionKeys);
        
        float lerpFactor = compLerpFactor(timeTick, currentId, pNodeAnim->mPositionKeys);
        return lerp(lerpFactor, currentId, pNodeAnim->mPositionKeys);
    }

    const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string &nodeName)
    {
        for(uint i = 0; i < pAnimation->mNumChannels; ++i)
        {
            const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
            if(std::string(pNodeAnim->mNodeName.data) == nodeName)
            {
                return pNodeAnim;
            }
        }
        
        return nullptr;
    }

/*
    void _readNodeHierarchy(float timeTick, const aiScene* pScene, const aiNode* pNode, const simd::float4x4& parentTransform)
    {
        std::string nodeName(pNode->mName.data);
        // TODO: change the animation index and handle multiple animations
        const aiAnimation* pAnimation = pScene->mAnimations[0];
        simd::float4x4 nodeTransform = Math::convertAssimpM(pNode->mTransformation);
        
        const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);
        
        if(pNodeAnim)
        {
            // get linear interpolations for every channel for current time.
            simd::float4x4 scaleM = Math::scale(lerpScale(timeTick, pNodeAnim));
            simd::float4x4 rotateM = simd::float4x4(lerpRotation(timeTick, pNodeAnim));
            simd::float4x4 translateM = Math::translate(lerpTranslate(timeTick, pNodeAnim));
            nodeTransform = translateM * rotateM * scaleM;
        }
        
        // offset matrix and inverse t matrix is not used.
        simd::float4x4 globalTrans = parentTransform * nodeTransform;
        
        _skeletonPose->updateGlobalPose(nodeName, globalTrans);
        
        //update poses of the child nodes if exists
        for(size_t i = 0; i < pNode->mNumChildren; ++i)
        {
            readNodeHierarchy(timeTick, pScene, pNode->mChildren[i], globalTrans);
        }
    }
 */


} // namespace AnimationSystem
