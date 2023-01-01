
#include "Mesh.hpp"

namespace AnimationSystem
{
    Mesh::~Mesh()
    {
        pVertexBuffer->release();
        pIndexBuffer->release();
        pInstanceBuffer->release();
    }

    void Mesh::buildBuffers(MTL::Device *pDevice, size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices)
    {
        pVertexBuffer = pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        pIndexBuffer = pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        memcpy(pVertexBuffer->contents(), vertexDataArr, vertexDataSize);
        memcpy(pIndexBuffer->contents(), indices, indexDataSize);

        pVertexBuffer->didModifyRange(NS::Range::Make(0, pVertexBuffer->length()));
        pIndexBuffer->didModifyRange(NS::Range::Make(0, pIndexBuffer->length()));
    }

    void Mesh::buildInstanceBuffer(MTL::Device *pDevice, size_t instanceDataSize)
    {
        pInstanceBuffer = pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
    }

    ShaderTypes::InstanceData *Mesh::getInstanceData()
    {
        return reinterpret_cast<ShaderTypes::InstanceData *>(pInstanceBuffer->contents());
    }

    std::string MeshComponent::getName() noexcept
    {
        return "MeshComponent";
    }

} // namespace AnimationSystem
