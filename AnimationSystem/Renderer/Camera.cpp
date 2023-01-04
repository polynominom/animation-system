#include "Camera.hpp"

namespace AnimationSystem
{
    Camera::~Camera()
    {
        if(_e)
            delete _e;
        if(_cameraData)
            delete _cameraData;
    }

    void Camera::initData(simd::float3 pos)
    {
        _e = new Entity();
        _e->setPosition((simd::float4){pos.x, pos.y, pos.z, 1.0f});
    }

    void Camera::updateData()
    {
        _cameraData = new ShaderTypes::CameraData();
        _cameraData->viewM = Math::makeIdentity(); // TODO: create this from position?
        _cameraData->projectionM = Math::makePerspective(60.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
    }
} // namespace AnimationSystem
