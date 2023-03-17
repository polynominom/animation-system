#include <BlendOperations.hpp>
#include <PoseBlender.hpp>
#include <Blend.hpp>
#include <BlendWeight.hpp>
namespace AnimationSystem
{
    std::shared_ptr<SkeletonPose> BlendOperationHandler::handle(BlendNode &node1, BlendNode &node2, float time)
    {
        std::shared_ptr<SkeletonPose> blendedPose = std::make_shared<SkeletonPose>();
        
        auto skeletonPose1 = node1.operate();
        auto skeletonPose2 = node2.operate();
        
        //TODO: compute weight from node's blend points
        auto blendWeight = 0.5f;
        //TODO: get mask from node
        JointMask* mask = new std::vector<float>(skeletonPose1->getSkeleton()->jointCount(), 1.0f);
        
        // Apply local blend
        BlenderLocal<PoseBlender, BlendWeight>(skeletonPose1.get(), skeletonPose2.get(), blendWeight, mask, blendedPose.get());
        
        return blendedPose;
    }
} // namespace AnimationSystem
