//
//  BufferHelper.hpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 04.01.23.
//

#ifndef BufferHelper_hpp
#define BufferHelper_hpp

#include <Renderer/Shader/ShaderTypes.hpp>
#include <Metal/Metal.hpp>
#include <Base/Math.hpp>

namespace AnimationSystem
{
    class BufferHelper
    {
    public:
        static void updateUniformBuffer(MTL::Buffer *pUniformBuffer, ShaderTypes::CameraData *cData, simd::float4x4 modelMatrix);
    };
}

#endif /* BufferHelper_h */
