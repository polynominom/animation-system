#ifndef SCENE_HPP
#define SCENE_HPP

#include <Base/Entity.hpp>
#include "Camera.hpp"
#include <vector>

namespace AnimationSystem
{
    class Scene
    {
    public:
        [[nodiscard]] std::vector<Entity> filterEntities(std::string componentName);
        std::shared_ptr<Camera> camera;
        std::vector<Entity> entities;
    };
} // namespace AnimationSystem

#endif
