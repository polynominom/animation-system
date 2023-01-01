#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Shader/ShaderTypes.hpp"
#include <Base/Math.hpp>
#include <Metal/Metal.hpp>

namespace AnimationSystem
{
    class Camera
    {
    public:
        void setBuffer(MTL::Device *pDevice);
        [[nodiscard]] MTL::Buffer *getBuffer() { return _cameraBuffer; }
        void updateData();

    private:
        MTL::Buffer *_cameraBuffer{nullptr};
    };
} // namespace AnimationSystem
#endif