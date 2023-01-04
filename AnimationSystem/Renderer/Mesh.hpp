#ifndef MESH_HPP
#define MESH_HPP

#include <Metal/Metal.hpp>
#include <vector>
#include <Base/Component.hpp>
#include "Shader/ShaderTypes.hpp"
#include <simd/simd.h>
#include "Shapes/Cube.hpp"
#include <Animation/SkinnedVertex.hpp>

namespace AnimationSystem
{
    class Mesh
    {
    public:
        ~Mesh();

        void buildBuffersFrom(MTL::Device *pDevice, size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices);
        void buildInstanceBufferFrom(MTL::Device *pDevice, size_t instanceDataSize);

        void buildBuffers(MTL::Device *pDevice);
        void buildInstanceBuffer(MTL::Device *pDevice);

        [[nodiscard]] ShaderTypes::InstanceData *getInstanceData();

        void addVertex(simd::float3 position, simd::float3 normal, simd::float2 texcoord);
        void initSkinnedVertex();
        void addSkinnedVertexWeight(size_t vertexId, size_t jointIndex, float weight);
        void addIndex(uint16_t index);
        

        // static const int kMaxBufferCount{32};
        // Vertex Buffer of the mesh
        MTL::Buffer *pVertexBuffer{nullptr};
        uint64_t numberOfVertices{0};

        // Index buffer of the mesh
        MTL::Buffer *pIndexBuffer{nullptr};
        uint64_t numberOfIndices{0};

        MTL::Buffer *pTriangles{nullptr};
        uint64_t numberOfTriangles{0};

        MTL::Buffer *pInstanceBuffer{nullptr};
        uint64_t numberOfInstances{0};

    private:
        [[no_unique_address]] std::vector<ShaderTypes::VertexData> _vertexData{};
        [[no_unique_address]] std::vector<SkinnedVertex> _skinnedVertices{};
        [[no_unique_address]] std::vector<uint16_t> _indexData{};
        // bounding box points TODO: calculate when building mesh?
        simd::float3 _bbMin;
        simd::float3 _bbMax;
    };

    class MeshComponent : public Component
    {
    public:
        std::shared_ptr<Mesh> mesh;

        virtual std::string getName() noexcept override;
    };
} // namespace AnimationSystem

#endif
