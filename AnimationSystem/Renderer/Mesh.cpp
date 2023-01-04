
#include "Mesh.hpp"
#include <iostream>
#include <execution>

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
        size_t vertexDataSize = sizeof(ShaderTypes::VertexData) * this->_vertexData.size();
        size_t indexDataSize = sizeof(uint16_t) * this->_indexData.size();

        this->pVertexBuffer = pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        this->pIndexBuffer = pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        copyAndUpdateBuffer(this->pVertexBuffer, this->_vertexData.data(), vertexDataSize);
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
            auto &vertex = _vertexData.emplace_back();
            vertex.position = position;
            vertex.normal = normal;
            vertex.texcoord = texcoord;
        }
    }

    void Mesh::addIndex(uint16_t index)
    {
        _indexData.push_back(index);
    }
    
void Mesh::initSkinnedVertex()
{
    _skinnedVertices.resize(_vertexData.size());
    #pragma omp parallel for
    for(int i = 0; i<_vertexData.size();++i)
    {
        _skinnedVertices[i]._position = _vertexData[i].position;
        _skinnedVertices[i]._normal = _vertexData[i].normal;
        _skinnedVertices[i]._texCoord = _vertexData[i].texcoord;
    };
    
    std::cout << "skinned vertices are initialized: "<<_skinnedVertices[0]._position.y<<"\n";
}
    void Mesh::addSkinnedVertexWeight(size_t vertexId, size_t jointIndex, float weight)
    {
        auto v = _skinnedVertices[vertexId];
        
        // No weight initialized for the vertex, adding 1st weight.
        if(v._jointWeights.x == 0)
        {
            v._jointWeights.x = weight;
            v._jointIndex.x = jointIndex;
            return;
        }
        
        // 2nd weight.
        if(v._jointWeights.y == 0)
        {
            v._jointWeights.y = weight;
            v._jointIndex.y = jointIndex;
            return;
        }
        
        // 3rd weight.
        if(v._jointWeights.z == 0)
        {
            v._jointWeights.z = weight;
            v._jointIndex.z = jointIndex;
            return;
        }
        
        // 4th weight,
        if(v._jointIndex[3] ==  -1)
        {
            v._jointWeights.z = weight;
            v._jointIndex.z = jointIndex;
        }
            
    }

} // namespace AnimationSystem
