#ifndef BLEND_TREE_HPP
#define BLEND_TREE_HPP

#include "AnimationClip.hpp"
#include "SkeletonPose.hpp"
#include <BlendThreshold.hpp>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <type_traits>

namespace AnimationSystem
{

    enum BlendOperationType
    {
        LERP_1D = 80, // binary lerp
        LERP_2D = 81, // two dimentional but Triangular lerps
        ADDITIVE = 82 // additive blending
    };

    class BlendNode
    {
    public:
        BlendNode(std::string &&name, std::string &&motionName) : _name(std::move(name)), _motionName(std::move(motionName)){}
        virtual ~BlendNode();
        virtual std::shared_ptr<SkeletonPose> operate();
        const std::string& getName() const noexcept { return _name; }
        void setBlendThreshold(std::string &&tresholdName, BlendThreshold blendTreshold){_thresholds[std::move(tresholdName)] = blendTreshold;}
        std::optional<BlendThreshold> getBlendTreshold(const std::string &tresholdName) const;
    private:
        // unique name of the node
        // each node in the system will have a unique name
        std::string _name;
        
        // motion name that is in the animation file
        std::string _motionName;

        // the thresholds for blending for this node
        // e.g., 0....(point)......1
        // Note that these thresholds are used inside operate function of PARENT node,
        //  and it is actually this node's influence on parent's skeleton node calculation.
        //  should not be used in this node's operation !!
        std::unordered_map<std::string, BlendThreshold> _thresholds;
    };

    class LeafNode : public BlendNode
    {
    public:
        std::shared_ptr<SkeletonPose> operate();
        std::shared_ptr<AnimationClip> getClip() const { return _clip; }
        std::shared_ptr<SkeletonPose> getCurrentPose() const { return _pose; }
    private:
        std::shared_ptr<SkeletonPose> _pose;
        std::shared_ptr<AnimationClip> _clip;
    };

    // operation node: connected to multiple nodes that can be leaf node and/or other operation node
    class OperationNode : public BlendNode
    {
    public:
        void setType(BlendOperationType type);
        void addNode(std::shared_ptr<BlendNode> node);
        std::shared_ptr<SkeletonPose> operate();

    private:
        // type of the operation = (1d, 2d lerps or additive)
        BlendOperationType _type;

        // BlendNode = OperationNode or LeafNode
        std::vector<std::shared_ptr<BlendNode>> _inputs;
    };

    /**
     * Root node of the blend tree.
     * There could be more than one blend node connected to root node in some conditions such as transition from one state to another (e.g., CrossFasde)
     */
    class RootBlendNode
    {
    public:
        void addNewHead(std::shared_ptr<BlendNode> newHead);
        void removeHead(const std::string& nodeName);

    private:
        std::vector<std::shared_ptr<BlendNode>> _heads;
    };

    class BlendTree
    {
    public:
        BlendTree(std::string &&name): _name(std::move(name)){}
        void setRootBlendNode(std::unique_ptr<RootBlendNode> rootBlendNode) {_rootNode = std::move(rootBlendNode);}
    private:
        std::string _name;
        std::unique_ptr<RootBlendNode> _rootNode;
    };
}

#endif
