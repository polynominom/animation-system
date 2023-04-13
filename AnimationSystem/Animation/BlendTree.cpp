//
//  BlendTree.cpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 02.04.23.
//

#include <BlendTree.hpp>

namespace AnimationSystem
{
    // @mark BLEND NODE
    std::optional<BlendThreshold> BlendNode::getBlendTreshold(const std::string &tresholdName) const
    {
        auto it = _thresholds.find(tresholdName);
        if (it == _thresholds.end())
            return {};

        return it->second;
    }

    // @mark OPERATION NODE
    void OperationNode::addNode(std::shared_ptr<BlendNode> node)
    {
        _inputs.push_back(node);
    }

    void OperationNode::setType(BlendOperationType type)
    {
        _type = type;
    }

    // @mark ROOT BLEND NODE for the tree
    void RootBlendNode::addNewHead(std::shared_ptr<BlendNode> newHead)
    {
        _heads.push_back(newHead);
    }

}
