//
//  BufferHelper.cpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 04.01.23.
//

#include <Renderer/BufferHelper.hpp>

namespace AnimationSystem
{
    void BufferHelper::updateUniformBuffer(MTL::Buffer *pUniformBuffer, ShaderTypes::CameraData *cData, simd::float4x4 modelMatrix)
    {
        auto uniformData = reinterpret_cast<ShaderTypes::UniformData *>(pUniformBuffer->contents());
        uniformData->projectionMatrix = cData->projectionM;
        uniformData->viewMatrix = cData->viewM;
        uniformData->modelMatrix = modelMatrix;

        pUniformBuffer->didModifyRange(NS::Range::Make(0, pUniformBuffer->length()));
    }
}
