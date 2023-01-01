#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

#include <Metal/Metal.hpp>
#include "Mesh.hpp"

namespace AnimationSystem
{
    class RenderPass
    {
    public:
        MTL::RenderPipelineState *renderPipelineState;
        Mesh m;
    };
} // namespace AnimationSystem

#endif