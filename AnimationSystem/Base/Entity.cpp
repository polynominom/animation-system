#include "Entity.hpp"

namespace AnimationSystem
{
    Entity::Entity() : _position((simd::float4){0., 0., 0., 1.}), _scale((simd::float4){1., 1., 1., 1.})
    {
    }

    Entity::Entity(simd::float4 pos, simd::float4 scale) : _position(pos), _scale(scale)
    {
    }

    Entity::Entity(simd::float4 pos, simd::float4 scale, Base::Quaternion rot) : _position(pos), _scale(scale), _rotation(rot)
    {
    }

    [[nodiscard]] std::shared_ptr<Component> Entity::getComponent(std::string name)
    {
        auto it = std::find_if(std::begin(_components), std::end(_components), [&name](std::shared_ptr<Component> c)
                               { return name == c->getName(); });
        return *it;
    }

    [[nodiscard]] bool Entity::hasComponent(std::string name)
    {
        auto it = std::find_if(std::begin(_components), std::end(_components), [&name](std::shared_ptr<Component> c)
                               { return c->getName() == name; });

        return it != std::end(_components);
    }

    void Entity::addComponent(std::shared_ptr<Component> &comp)
    {
        _components.push_back(comp);
    }

} // namespace AnimationSystem
