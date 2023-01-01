#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Quaternion.hpp"
#include "Component.hpp"

#include <simd/simd.h>
#include <vector>

namespace AnimationSystem
{
    class Entity
    {
    public:
        Entity();
        Entity(simd::float4 pos, simd::float4 scale);
        Entity(simd::float4 pos, simd::float4 scale, Base::Quaternion rot);
        void setPosition(simd::float4 pos) { _position = pos; }
        void addComponent(std::shared_ptr<Component> &comp);

        [[nodiscard]] Base::Quaternion getRotation() { return _rotation; }
        [[nodiscard]] simd::float4 getPosition() { return _position; }
        [[nodiscard]] simd::float4 getScale() { return _scale; }
        bool hasComponent(std::string name);
        std::shared_ptr<Component> getComponent(std::string name);

    private:
        [[no_unique_address]] Base::Quaternion _rotation{0., 0., 0., 1.};
        [[no_unique_address]] simd::float4 _position;
        [[no_unique_address]] simd::float4 _scale;
        std::vector<std::shared_ptr<Component>> _components{};
    };
} // namespace AnimationSystem
#endif
