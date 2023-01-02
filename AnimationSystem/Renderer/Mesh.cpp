
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

    void Mesh::buildBuffers(MTL::Device *pDevice)
    {
        this->pVertexBuffer = pDevice->newBuffer(this->numberOfVertices, MTL::ResourceStorageModeManaged);
        this->pIndexBuffer = pDevice->newBuffer(this->numberOfIndices, MTL::ResourceStorageModeManaged);

        memcpy(this->pVertexBuffer->contents(), this->_verexData.data(), this->numberOfVertices);
        memcpy(this->pIndexBuffer->contents(), this->_indexData.data(), this->numberOfIndices);

        this->pVertexBuffer->didModifyRange(NS::Range::Make(0, this->pVertexBuffer->length()));
        this->pIndexBuffer->didModifyRange(NS::Range::Make(0, this->pIndexBuffer->length()));
    }

    void Mesh::buildBuffersFrom(MTL::Device *pDevice, size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices)
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

    void Mesh::buildInstanceBuffer(MTL::Device *pDevice)
    {
        pInstanceBuffer = pDevice->newBuffer(sizeof(ShaderTypes::InstanceData), MTL::ResourceStorageModeManaged);
    }

    void Mesh::buildInstanceBufferFrom(MTL::Device *pDevice, size_t instanceDataSize)
    {
        pInstanceBuffer = pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
    }

    void Mesh::addVertex(simd::float3 position, simd::float3 normal, simd::float2 texcoord)
    {
        {
            auto &vertex = _verexData.emplace_back();
            vertex.position = position;
            vertex.normal = normal;
            vertex.texcoord = texcoord;
        }
    }

    void Mesh::addIndex(uint16_t index)
    {
        _indexData.push_back(index);
    }

} // namespace AnimationSystem
