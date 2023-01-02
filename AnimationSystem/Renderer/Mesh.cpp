
#include "Mesh.hpp"

namespace AnimationSystem
{
    Mesh::~Mesh()
    {
        pVertexBuffer->release();
        pIndexBuffer->release();
        pInstanceBuffer->release();
        pTriangles->release();
    }

    std::string MeshComponent::getName() noexcept
    {
        return "MeshComponent";
    }

    ShaderTypes::InstanceData *Mesh::getInstanceData()
    {
        return reinterpret_cast<ShaderTypes::InstanceData *>(pInstanceBuffer->contents());
    }

    void Mesh::buildBuffers(MTL::Device *pDevice, size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices)
    {
        this->numberOfVertices = vertexDataSize;
        this->numberOfIndices = indexDataSize;

        this->pVertexBuffer = pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        this->pIndexBuffer = pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        memcpy(this->pVertexBuffer->contents(), vertexDataArr, vertexDataSize);
        memcpy(this->pIndexBuffer->contents(), indices, indexDataSize);

        this->pVertexBuffer->didModifyRange(NS::Range::Make(0, this->pVertexBuffer->length()));
        this->pIndexBuffer->didModifyRange(NS::Range::Make(0, this->pIndexBuffer->length()));
    }

    void Mesh::buildInstanceBuffer(MTL::Device *pDevice, size_t instanceDataSize)
    {
        pInstanceBuffer = pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
    }
} // namespace AnimationSystem
