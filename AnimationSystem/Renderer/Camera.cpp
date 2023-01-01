#include "Camera.hpp"

namespace AnimationSystem
{

    void Camera::updateData()
    {
        ShaderTypes::CameraData *pCameraData = reinterpret_cast<ShaderTypes::CameraData *>(_cameraBuffer->contents());
        pCameraData->perspectiveTransform = Math::makePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
        pCameraData->worldTransform = Math::makeIdentity();
        pCameraData->worldNormalTransform = Math::discardTranslation(pCameraData->worldTransform);
        _cameraBuffer->didModifyRange(NS::Range::Make(0, sizeof(ShaderTypes::CameraData)));
    }

    void Camera::setBuffer(MTL::Device *pDevice)
    {
        const size_t cameraDataSize = sizeof(ShaderTypes::CameraData);
        _cameraBuffer = pDevice->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged);
    }
} // namespace AnimationSystem
