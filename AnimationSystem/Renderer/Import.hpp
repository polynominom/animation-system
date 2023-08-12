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
        static std::vector<std::unique_ptr<Mesh>> loadMeshes(const char *filename);
        static std::vector<std::unique_ptr<AssimpAnimation>> loadAnimations(const char *filename, const std::vector<std::unique_ptr<Mesh>> &meshesToBeLoaded);
    };

} // namespace AnimationSystem

#endif
