#ifndef MESH_HPP
#define MESH_HPP

#include <Metal/Metal.hpp>
#include <vector>
#include <Base/Component.hpp>
#include "Shader/ShaderTypes.hpp"
#include <simd/simd.h>

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
        MTL::Buffer *pVertexBuffer{nullptr};
        uint64_t numberOfVertices{0};

        MTL::Buffer *pIndexBuffer{nullptr};
        uint64_t numberOfIndices{0};

        MTL::Buffer *pTriangles{nullptr};
        uint64_t numberOfTriangles{0};

        MTL::Buffer *pInstanceBuffer{nullptr};
        uint64_t numberOfInstances{0};

    private:
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