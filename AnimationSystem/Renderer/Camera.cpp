#include "Camera.hpp"

namespace AnimationSystem
{
    namespace
    {
        simd::float4x4 getViewMatrix(simd::float4 pos)
        {
            auto t = Math::translate(pos.xyz);
            // TODO: camera orientation multiply it
            return t;
        }
    }
    Camera::~Camera()
    {
        if(_e)
            delete _e;
    }

    void Camera::initData(simd::float3 pos)
    {
        _e = new Entity();
        _e->setPosition((simd::float4){pos.x, pos.y, pos.z, 1.0f});
    }

    void Camera::updateData()
    {
        _cameraData = std::make_shared<ShaderTypes::CameraData>();
        _cameraData->viewM = getViewMatrix(_e->getPosition());
        _cameraData->projectionM = Math::makePerspective(60.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
    }
} // namespace AnimationSystem
