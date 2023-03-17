#ifndef BLEND_TREE_HPP
#define BLEND_TREE_HPP

#include "AnimationClip.hpp"
#include "SkeletonPose.hpp"
#include <BlendWeight.hpp>
#include <memory>
#include <vector>
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
        BlendNode(int id, BlendWeight w) : _id(id), _blendWeight(w) {}
        virtual ~BlendNode();
        virtual std::shared_ptr<SkeletonPose> operate();
        int nodeId() const { return _id; };
        const BlendWeight getBlendWeight() const { return _blendWeight; }
        
    private:
        // unique id of the node
        // each node in the system will have a unique id
        int _id;

        // the weight for blending for this node
        // e.g., 0....(point)......1
        // Note that this weight is used inside operate function of PARENT node,
        //  and it is actually this node's influence on parent's skeleton node calculation.
        //  should not be used in this node's operation !!
        BlendWeight _blendWeight;
    };

    class LeafNode : BlendNode
    {
    public:
        std::shared_ptr<SkeletonPose> operate();
        std::shared_ptr<AnimationClip> getClip() const { return _clip; }
    private:
        std::shared_ptr<SkeletonPose> _pose;
        std::shared_ptr<AnimationClip> _clip;
    };

    // operation node: connected to multiple nodes that can be leaf node and/or other operation node
    class OperationNode : public BlendNode
    {
    public:
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
        RootBlendNode(uint8_t size);
        void AddNewHead(std::shared_ptr<BlendNode> newHead);
        void RemoveHead(int nodeId);

    private:
        uint8_t id;
        std::vector<std::shared_ptr<BlendNode>> _heads;
    };

    class BlendTree
    {
    public:
        BlendTree();

    private:
        std::unique_ptr<RootBlendNode> _rootNode;
    };
}

#endif
