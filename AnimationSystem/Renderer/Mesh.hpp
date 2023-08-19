#ifndef MESH_HPP
#define MESH_HPP

#include <Metal/Metal.hpp>
#include <vector>
#include <Base/Component.hpp>
#include "Shader/ShaderTypes.hpp"
#include <simd/simd.h>
#include "Shapes/Cube.hpp"
#include <Animation/SkinnedVertex.hpp>
#include <Animation/SkeletonPose.hpp>
#include <Renderer/RendererManager.hpp>

#include <unordered_map>

namespace AnimationSystem
{
// static const int kMaxBufferCount{32};
    class Mesh
{
    public:
        ~Mesh();
        void initSkinnedVertex();
    
//        void buildBuffersFrom(size_t vertexDataSize, ShaderTypes::VertexData *vertexDataArr, size_t indexDataSize, uint16_t *indices);
        //void buildInstanceBufferFrom(size_t instanceDataSize);
        void buildBuffers();
        void updateJointBuffer();
        void buildInstanceBuffer();
        
        void addVertex(simd::float3 position, simd::float3 normal, simd::float2 texcoord);
        void addIndex(uint16_t index);
        void addSkinnedVertexWeight(size_t vertexId, int jointIndex, float weight);
    
        void setSkeletonPose(std::unique_ptr<SkeletonPose> &&pose){_animSkeletonPose = std::move(pose);}
        [[nodiscard]] SkeletonPose* getSkeletonPose(){return _animSkeletonPose.get();}
        /*
         Applies skinning to the effected vertices. And updates the vertex buffer.
         */
        void skin();
        
        // Vertex Buffer of the mesh
        MTL::Buffer *pVertexBuffer{nullptr};
        uint64_t numberOfVertices{0};
        
        // Index buffer of the mesh
        MTL::Buffer *pIndexBuffer{nullptr};
        uint64_t numberOfIndices{0};
    
        MTL::Buffer *pJointBuffer{nullptr};
        uint64_t jointCount{0};
        
    private:
        std::vector<ShaderTypes::VertexData> _vertexData{};
        std::vector<SkinnedVertex> _skinnedVertices{};
        std::vector<uint16_t> _indexData{};
        std::unique_ptr<SkeletonPose> _animSkeletonPose;
        
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
