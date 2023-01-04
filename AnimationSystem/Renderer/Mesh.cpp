
#include "Mesh.hpp"
#include <iostream>

namespace AnimationSystem
{
    namespace
    {
        void copyAndUpdateBuffer(MTL::Buffer *buffer, void *arr, size_t size)
        {
            memcpy(buffer->contents(), arr, size);
            buffer->didModifyRange(NS::Range::Make(0, buffer->length()));
        }
    }
    std::string MeshComponent::getName() noexcept
    {
        return "MeshComponent";
    }

    Mesh::~Mesh()
    {
        pVertexBuffer->release();
        pIndexBuffer->release();
        pInstanceBuffer->release();
        pTriangles->release();
    }

    ShaderTypes::InstanceData *Mesh::getInstanceData()
    {
        return reinterpret_cast<ShaderTypes::InstanceData *>(pInstanceBuffer->contents());
    }

    void Mesh::buildBuffers(MTL::Device *pDevice)
    {
        size_t vertexDataSize = sizeof(ShaderTypes::VertexData) * this->_verexData.size();
        size_t indexDataSize = sizeof(uint16_t) * this->_indexData.size();

        this->pVertexBuffer = pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        this->pIndexBuffer = pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        copyAndUpdateBuffer(this->pVertexBuffer, this->_verexData.data(), vertexDataSize);
        copyAndUpdateBuffer(this->pIndexBuffer, this->_indexData.data(), indexDataSize);
    }

    void Mesh::buildBuffersFrom(MTL::Device *pDevice, size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices)
    {
        this->numberOfVertices = vertexDataSize / sizeof(ShaderTypes::VertexData);
        this->numberOfIndices = indexDataSize / sizeof(uint16_t);

        this->pVertexBuffer = pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        this->pIndexBuffer = pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        copyAndUpdateBuffer(this->pVertexBuffer, vertexDataArr, vertexDataSize);
        copyAndUpdateBuffer(this->pIndexBuffer, indices, indexDataSize);
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

    void Mesh::hebele()
    {
        // std::cout << "Changing indices\n";
        // this->_indexData.clear();
        // this->_indexData = std::vector<uint16_t>(36, 0);
        // Shapes::Cube c(0.4);
        // for (int i = 0; i < 36; ++i)
        //     this->_indexData[i] = c.indices[i];
    }

} // namespace AnimationSystem
