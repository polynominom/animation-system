#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
namespace AnimationSystem
{
    class Component
    {
    public:
        virtual ~Component() {}
        virtual std::string getName() noexcept { return ""; }
    };
} // namespace AnimationSystem

#endif
