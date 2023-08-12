#include "Manager.hpp"
#include "BlendTree.hpp"
#include "BlendOperations.hpp"

namespace AnimationSystem
{
    simd::float4x4 Manager::_compGlobalPoseRecursively(float timeInSec, SkeletonPose *pose, std::string name, const simd::float4x4 &parentTransform)
    {
        if(this->_assimpAnimations.size() == 0)
        {
            return simd::float4x4{};
        }
        
        auto theAnim = this->_assimpAnimations[0].get();

        // Calculate JointPose transformation matrix for the time t.
        simd::float4x4 nodeTransform = theAnim->getInitialTransformation(name);
        if (theAnim->nameExist(name))
        {
            float timeTick = theAnim->getTimeTick(timeInSec);
            auto scl = Math::scaleM(theAnim->compScale(timeTick, name));
            auto trs = Math::translate(theAnim->compPosition(timeTick, name));
            auto rot = simd::float4x4(theAnim->compRotation(timeTick, name));
            nodeTransform = trs * rot * scl;
        }

        // calculate global transformation matrix.
        simd::float4x4 globalTrans = parentTransform * nodeTransform;

        // Update the global pose if the joint exsits.
        auto id = pose->getJointId(name);
        if (id != -1)
        {
            // get inv T pose
            auto invT = pose->getSkeleton()->getJoint(id)->getInverseTPose();
            // set global transformation
            pose->updateGlobalPose(name, globalTrans * invT);
        }

        // recursively iterates through children.
        for (auto children : theAnim->getChildList(name))
        {
            _compGlobalPoseRecursively(timeInSec, pose, children, globalTrans);
        }
    }

    void Manager::calculateSkeletonPoses()
    {
    }

    void Manager::calculateSkeletonPosesWithTime(float timeInSec, Mesh &mesh)
    {

        if (this->_assimpAnimations.size() == 0)
            return;

        auto firstParentTransform = Math::makeIdentity();
        auto pose = mesh.getSkeletonPose();
        _compGlobalPoseRecursively(timeInSec, pose,
                                   this->_assimpAnimations[0]->getRootNodeName(),
                                   firstParentTransform);

        // every related poses are set. Copute the final transformations of all
        pose->compFinalTransformations();

        // calculate the joint buffer
        // at this point every possible global pose is calculated for the given mesh
        mesh.updateJointBuffer();
    }
} // namespace AnimationSystem
