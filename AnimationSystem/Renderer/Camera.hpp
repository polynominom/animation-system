#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "Shader/ShaderTypes.hpp"
#include <Base/Math.hpp>
#include <Base/Entity.hpp>
#include <Metal/Metal.hpp>


namespace AnimationSystem
{
    class Camera
    {
    public:
        ~Camera();
        //TODO: set rotation
        void initData(simd::float3 pos);
        void setBuffer();
        void updateData();
        [[nodiscard]]ShaderTypes::CameraData* data() {return _cameraData;}

    private:
        ShaderTypes::CameraData* _cameraData{nullptr};
        Entity* _e{nullptr};
        
    };
} // namespace AnimationSystem
#endif
