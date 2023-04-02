//
//  BlendTreeParser.cpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 02.04.23.
//

#include <BlendTreeParser.hpp>
#include <unordered_map>

namespace AnimationSystem
{
    template <typename jsonReader>
    BlendTree BlendTreeParser<jsonReader>::parse(const std::string &filename)
    {
        auto reader = jsonReader::create(filename);
        auto name = reader.get("name");

        auto blendTree = BlendTree(name);
        std::unique_ptr<RootBlendNode> root = std::make_unique<RootBlendNode>();

        auto operationNode = std::make_shared<OperationNode>(name);
        for (auto &state : reader.get("states"))
        {
            auto name = state.get("name");
            auto moti = state.get("motion");
            // TODO: how to get leaf or operation node?
            auto node = std::make_shared<LeafNode>(std::move(name), std::move(moti));
            for (auto &[tName, tValue] : reader.get("thresholds"))
            {
                // TODO: how to get 1d or 2d or additive
                BlendThreshold1D blendTreshold(tValue);
                node.setBlendThreshold(std::move(tName), blendTreshold);
                operationNode->addNode(node);
            }
        }

        root->addNewHead(operationNode);
        blendTree.setRootBlendNode(std::move(root));
    }
}
