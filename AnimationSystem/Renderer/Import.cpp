#include "Import.hpp"
// #define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph)
#define ASSIMP_LOAD_FLAGS (aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded)
//  aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded
// ai_scene = aiApplyPostProcessing(ai_scene, aiProcess_FixInfacingNormals | aiProcess_MakeLeftHanded);

namespace AnimationSystem
{
    namespace
    {
        std::shared_ptr<Mesh> assimpToMesh(const aiMesh *assimpMesh)
        {
            auto m = std::make_shared<Mesh>();

            m->numberOfVertices = assimpMesh->mNumVertices;
            m->numberOfIndices = assimpMesh->mNumFaces * 3;
            std::cout << "2. Parsing " << m->numberOfVertices << " NUMBER OF VERTICES...\n";
            for (size_t vi = 0; vi < assimpMesh->mNumVertices; ++vi)
            {
                simd::float3 pos;
                simd::float3 nor;
                simd::float2 tex;
                if (assimpMesh->HasPositions())
                    pos = (simd::float3){assimpMesh->mVertices[vi].x, assimpMesh->mVertices[vi].y, assimpMesh->mVertices[vi].z};
                if (assimpMesh->HasNormals())
                    nor = (simd::float3){assimpMesh->mNormals[vi].x, assimpMesh->mNormals[vi].y, assimpMesh->mNormals[vi].z};
                if (assimpMesh->HasTextureCoords(0))
                    tex = (simd::float2){assimpMesh->mTextureCoords[0][vi].x, assimpMesh->mTextureCoords[0][vi].y};
                if (assimpMesh->HasBones())
                {
                    // uint64_t nJoints = pmesh->mNumBones;
                    // TODO: import joints and create skeletons and skinned vertices
                }
                if (assimpMesh->HasTangentsAndBitangents())
                {
                    // handle tangents and bit tangents
                }

                m->addVertex(pos, nor, tex);
            }

            if (assimpMesh->HasFaces())
            {
                for (size_t f = 0; f < assimpMesh->mNumFaces; ++f)
                {
                    const aiFace face = assimpMesh->mFaces[f];
                    for (size_t k = 0; k < face.mNumIndices; k++)
                    {
                        m->addIndex(face.mIndices[k]);
                    }
                }
            }

            return m;
        }

        std::vector<std::shared_ptr<Mesh>> parseMeshes(const aiScene *pScene)
        {
            std::vector<std::shared_ptr<Mesh>> meshes;
            for (int i = 0; i < pScene->mNumMeshes; ++i)
            {
                std::cout << "1. Getting MESH ID " << i << "\n";
                const aiMesh *pmesh = pScene->mMeshes[i];
                meshes.push_back(assimpToMesh(pmesh));
            }

            return meshes;
        }
    }

    std::vector<std::shared_ptr<Mesh>> Import::loadMeshes(const char *filename)
    {
        Assimp::Importer Importer;
        const aiScene *pScene = Importer.ReadFile(filename, ASSIMP_LOAD_FLAGS);

        if (!pScene)
        {
            std::cout << "Error while importing mesh: " << Importer.GetErrorString()
                      << "\n";
            return {};
        }

        if (!pScene->HasMeshes())
        {
            std::cout << "Error! File does not have mesh: " << Importer.GetErrorString()
                      << "\n";
            return {};
        }

        return parseMeshes(pScene);
    }
} // namespace AnimationSystem
