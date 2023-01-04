#include "Scene.hpp"

namespace AnimationSystem
{
    std::vector<Entity> Scene::filterEntities(std::string componentName)
    {
        // TODO: use ranges/views from c++20-23 std::views::filter(func);
        std::vector<Entity> filtered;
        std::copy_if(std::begin(entities), std::end(entities), std::back_inserter(filtered),
                     [&](Entity &e)
                     {
                         return e.hasComponent(componentName);
                     });

        return filtered;
    }
} // namespace AnimationSystem
