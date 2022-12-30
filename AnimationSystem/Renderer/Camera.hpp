#pragma once
#include "Shader/ShaderTypes.hpp"
#include <Base/Math.hpp>
#include <Metal/Metal.hpp>

namespace AnimationSystem
{
    namespace Camera
    {
        void setBuffer(MTL::Buffer *cameraBuffer)
        {
            pCameraData = reinterpret_cast<ShaderTypes::CameraData *>(cameraBuffer->contents());
            pCameraData->perspectiveTransform = Math::makePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
            pCameraData->worldTransform = Math::makeIdentity();
            pCameraData->worldNormalTransform = Math::discardTranslation(pCameraData->worldTransform);
            cameraBuffer->didModifyRange(NS::Range::Make(0, sizeof(ShaderTypes::CameraData)));
        }
    };
} // namespace AnimationSystem
