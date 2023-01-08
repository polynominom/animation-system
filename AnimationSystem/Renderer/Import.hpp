#ifndef __AnimationSystem_IMPORT__HPP
#define __AnimationSystem_IMPORT__HPP

#include "Mesh.hpp"
#include <Core/Core.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Renderer/Shader/ShaderTypes.hpp>
#include <Animation/Skeleton.hpp>


#include <iostream>
#include <memory>


namespace AnimationSystem
{
    class Import
    {
    public:
        [[nodiscard]] static std::vector<std::shared_ptr<Mesh>> loadMeshes(const char *filename);
    };

} // namespace AnimationSystem

#endif
