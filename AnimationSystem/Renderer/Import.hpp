#ifndef __AnimationSystem_IMPORT__HPP
#define __AnimationSystem_IMPORT__HPP

#include "Mesh.hpp"
#include <Core/Core.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Renderer/Shader/ShaderTypes.hpp>
#include <Animation/Skeleton.hpp>
#include <Animation/Manager.hpp>

#include <iostream>
#include <memory>


namespace AnimationSystem
{
    class Import
    {
    public:
        static void loadMeshesAndAnimations(const char *filename,
                                            std::vector<std::shared_ptr<Mesh>> &meshesToBeLoaded,
                                            std::shared_ptr<AnimationSystem::Manager> pManager);
    };

} // namespace AnimationSystem

#endif
