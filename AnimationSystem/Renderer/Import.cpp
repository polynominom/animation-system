#include "Import.hpp"
#include <algorithm>
#include <Base/Math.hpp>

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded)

#define MAX_NUM_BONES_PER_VERTEX 4

// #define ASSIMP_LOAD_FLAGS (aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded)
//  aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded
// ai_scene = aiApplyPostProcessing(ai_scene, aiProcess_FixInfacingNormals | aiProcess_MakeLeftHanded);


namespace AnimationSystem
{
    namespace
    {
        std::shared_ptr<Mesh> assimpToMesh(const aiMesh *assimpMesh, const aiScene *pScene)
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
                {
                    pos = (simd::float3){assimpMesh->mVertices[vi].x, assimpMesh->mVertices[vi].y, assimpMesh->mVertices[vi].z};
                }

                if (assimpMesh->HasNormals())
                {
                    nor = (simd::float3){assimpMesh->mNormals[vi].x, assimpMesh->mNormals[vi].y, assimpMesh->mNormals[vi].z};
                }

                if (assimpMesh->HasTextureCoords(0))
                {
                    tex = (simd::float2){assimpMesh->mTextureCoords[0][vi].x, assimpMesh->mTextureCoords[0][vi].y};
                }

   
                if (assimpMesh->HasTangentsAndBitangents())
                {
                    // TODO: handle tangents and bit tangents
                }
                // std::cout << "Adding vertex, pos: (" << pos.x << "," << pos.y << "," << pos.z << "), "
                //           << "nor: (" << nor.x << "," << nor.y << "," << nor.z << "), "
                //           << " tex: (" << tex.x << "," << tex.y << ")"
                //           << "\n";
                m->addVertex(pos, nor, tex);
            }
            

            if (assimpMesh->HasFaces())
            {
                for (size_t f = 0; f < assimpMesh->mNumFaces; ++f)
                {
                    const aiFace face = assimpMesh->mFaces[f];
                    for (size_t k = 0; k < face.mNumIndices; k++)
                    {
                        // std::cout << "Adding index: " << face.mIndices[k] << "\n";
                        m->addIndex(face.mIndices[k]);
                    }
                }
            }
            
            if (assimpMesh->HasBones())
            {
                m->initSkinnedVertex();
                
                std::shared_ptr<SkeletonPose> skeletonPose = std::make_shared<SkeletonPose>(pScene);
                
                std::vector<std::shared_ptr<Joint>> jointsTest;
                
                for(size_t ji = 0; ji < assimpMesh->mNumBones; ++ji)
                {
                    //std::cout << "ji:"<<ji<<"\n";
                    auto assimpBone = assimpMesh->mBones[ji];
                    if( !assimpBone )
                        continue;
                    
                    // create joint with name
                    std::shared_ptr<Joint> j = std::make_shared<Joint>(assimpBone->mName.C_Str());
                    skeletonPose->addJointName(j->getName(), ji);
                    
                    // Get And Set inverse of the T Pose
                    auto invTPose = Math::convertAssimpM(assimpBone->mOffsetMatrix);
                    j->setInvTPose(invTPose);
                    
                    // get vertex id and weights that the joint influences.
                    for(size_t wi = 0; wi < assimpBone->mNumWeights; ++wi )
                    {
                        m->addSkinnedVertexWeight(assimpBone->mWeights[wi].mVertexId, ji, assimpBone->mWeights[wi].mWeight);
                    }
                    
                    // update skeleton
                    jointsTest.push_back(std::move(j));
                    //pSkeleton->addJoint(j);
                }
                skeletonPose->getSkeleton()->setJoints(std::move(jointsTest));
                // adding joint is done. Now initialize the global poses
                skeletonPose->initGlobalPoses();
                m->setSkeletonPose(skeletonPose);
                m->getSkeletonPose()->computeGlobalPosesFromAssimp(0.0, pScene);
                m->getSkeletonPose()->compFinalTransformations();
                //m->skin();
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
                meshes.push_back(assimpToMesh(pmesh, pScene));
            }

            return meshes;
        }
    }

    std::vector<std::shared_ptr<Mesh>> Import::loadMeshes(const char *filename)
    {
        Assimp::Importer Importer;
        const aiScene *pScene = Importer.ReadFile(filename, ASSIMP_LOAD_FLAGS);
        // pScene = aiApplyPostProcessing(pScene, aiProcess_FixInfacingNormals | aiProcess_MakeLeftHanded);

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
