#ifndef MESH_HPP
#define MESH_HPP

#include <Metal/Metal.hpp>
#include <vector>
#include <Base/Component.hpp>
#include "Shader/ShaderTypes.hpp"

namespace AnimationSystem
{
    class Mesh
    {
    public:
        ~Mesh();
        void buildBuffers(MTL::Device *pDevice, size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices);
        void buildInstanceBuffer(MTL::Device *pDevice, size_t instanceDataSize);
        [[nodiscard]] ShaderTypes::InstanceData *getInstanceData();

        // static const int kMaxBufferCount{32};
        uint64_t numberOfIndices{0};
        MTL::Buffer *pVertexBuffer{nullptr};
        MTL::Buffer *pIndexBuffer{nullptr};
        MTL::Buffer *pInstanceBuffer{nullptr};
    };

    class MeshComponent : public Component
    {
    public:
        std::shared_ptr<Mesh> mesh;
        virtual std::string getName() noexcept override;
    };
} // namespace AnimationSystem

#endif