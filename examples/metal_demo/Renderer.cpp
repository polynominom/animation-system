#include <Renderer.hpp>
#include <AnimationSystem.h>
#include "Math.hpp"
#include <Cube.hpp>

namespace AnimationSystem
{
    namespace
    {
        // PRIVATE FREE FUNCTIONS
    }

    Renderer::Renderer(MTL::Device *device) : _pDevice(device->retain())
    {
        _pCommandQueue = _pDevice->newCommandQueue();
        buildShaders();
        buildDepthStencilStates();
        buildBuffers();

        _semaphore = dispatch_semaphore_create(Renderer::kMaxFrames);
    }

    Renderer::~Renderer()
    {
        _pShaderLibrary->release();
        _pDepthStencilState->release();
        _pVertexDataBuffer->release();

        for (int i = 0; i < Renderer::kMaxFrames; ++i)
        {
            _pInstanceDataBuffer[i]->release();
            _pCameraDataBuffer[i]->release();
        }

        _pIndexBuffer->release();
        _pPSO->release();
        _pCommandQueue->release();
        _pDevice->release();
    }

    void Renderer::draw(MTK::View *pView)
    {
        using simd::float3;
        using simd::float4;
        using simd::float4x4;

        NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();

        _frame = (_frame + 1) % Renderer::kMaxFrames;
        MTL::Buffer *pInstanceDataBuffer = _pInstanceDataBuffer[_frame];

        MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();
        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
        Renderer *pRenderer = this;
        // complete handler
        std::function<void(MTL::CommandBuffer *)> drawCallback = [pRenderer](MTL::CommandBuffer *pCmd)
        { dispatch_semaphore_signal(pRenderer->_semaphore); };
        pCmd->addCompletedHandler(drawCallback);

        _angle += 0.002f;

        const float scl = 0.2f;
        ShaderTypes::InstanceData *pInstanceData = reinterpret_cast<ShaderTypes::InstanceData *>(pInstanceDataBuffer->contents());

        float3 objectPosition = {0.f, 0.f, -10.f};

        // Update instance positions:

        float4x4 rt = Math::translate(objectPosition);
        float4x4 rr1 = Math::rotateY(-_angle);
        float4x4 rr0 = Math::rotateX(_angle * 0.5f);
        float4x4 rtInv = Math::translate({-objectPosition.x, -objectPosition.y, -objectPosition.z});
        float4x4 fullObjectRot = rt * rr1 * rr0 * rtInv;

        size_t ix = 0;
        size_t iy = 0;
        size_t iz = 0;
        for (size_t i = 0; i < kNumInstances; ++i)
        {
            if (ix == kInstanceRows)
            {
                ix = 0;
                iy += 1;
            }

            if (iy == kInstanceColumns)
            {
                iy = 0;
                iz += 1;
            }

            // Use the tiny math library to apply a 3D transformation to the instance.
            float4x4 scale = Math::scale((float3){scl, scl, scl});
            float4x4 zrot = Math::rotateZ(_angle * sinf((float)ix));
            float4x4 yrot = Math::rotateY(_angle * cosf((float)iy));

            float x = ((float)ix - (float)kInstanceRows / 2.f) * (2.f * scl) + scl;
            float y = ((float)iy - (float)kInstanceColumns / 2.f) * (2.f * scl) + scl;
            float z = ((float)iz - (float)kInstanceDepth / 2.f) * (2.f * scl);
            float4x4 translate = Math::translate(Math::add(objectPosition, {x, y, z}));

            pInstanceData[i].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;
            pInstanceData[i].instanceNormalTransform = Math::discardTranslation(pInstanceData[i].instanceTransform);

            float r = i / (float)kNumInstances;
            float g = 1.0f - r;
            float b = sinf(M_PI * 2.0f * r);
            pInstanceData[i].instanceColor = (float4){r, g, b, 1.0f};

            ix += 1;
        }
        pInstanceDataBuffer->didModifyRange(NS::Range::Make(0, pInstanceDataBuffer->length()));

        // Update camera state:

        MTL::Buffer *pCameraDataBuffer = _pCameraDataBuffer[_frame];
        ShaderTypes::CameraData *pCameraData = reinterpret_cast<ShaderTypes::CameraData *>(pCameraDataBuffer->contents());
        pCameraData->perspectiveTransform = Math::makePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
        pCameraData->worldTransform = Math::makeIdentity();
        pCameraData->worldNormalTransform = Math::discardTranslation(pCameraData->worldTransform);
        pCameraDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(ShaderTypes::CameraData)));

        // Begin render pass:

        MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
        MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

        pEnc->setRenderPipelineState(_pPSO);
        pEnc->setDepthStencilState(_pDepthStencilState);

        pEnc->setVertexBuffer(_pVertexDataBuffer, /* offset */ 0, /* index */ 0);
        pEnc->setVertexBuffer(pInstanceDataBuffer, /* offset */ 0, /* index */ 1);
        pEnc->setVertexBuffer(pCameraDataBuffer, /* offset */ 0, /* index */ 2);

        pEnc->setCullMode(MTL::CullModeBack);
        pEnc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

        pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                    6 * 6, MTL::IndexType::IndexTypeUInt16,
                                    _pIndexBuffer,
                                    0,
                                    kNumInstances);

        pEnc->endEncoding();
        pCmd->presentDrawable(pView->currentDrawable());
        pCmd->commit();

        pPool->release();
    }

    void Renderer::buildShaders()
    {
        using NS::StringEncoding::UTF8StringEncoding;

        const std::filesystem::path shaderPath = "./shaders/example_shader.glsl";

        auto shaderSrc = Reader<std::string>(shaderPath).read();
        const char *c_shaderSrc = shaderSrc.c_str();
        if (shaderSrc.empty())
        {
            std::cout << "[ERROR] When reading File " << shaderPath << ". It does not exist or something worse...\n";
            return;
        }

        NS::Error *pError = nullptr;
        MTL::Library *pLibrary = _pDevice->newLibrary(NS::String::string(c_shaderSrc, UTF8StringEncoding), nullptr, &pError);

        if (!pLibrary)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

        MTL::Function *pVertexFn = pLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
        MTL::Function *pFragFn = pLibrary->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

        MTL::RenderPipelineDescriptor *pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
        pDesc->setVertexFunction(pVertexFn);
        pDesc->setFragmentFunction(pFragFn);
        pDesc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
        pDesc->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth16Unorm);

        _pPSO = _pDevice->newRenderPipelineState(pDesc, &pError);
        if (!_pPSO)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

        pVertexFn->release();
        pFragFn->release();
        pDesc->release();
        _pShaderLibrary = pLibrary;
    }

    void Renderer::buildDepthStencilStates()
    {
        MTL::DepthStencilDescriptor *pDsDesc = MTL::DepthStencilDescriptor::alloc()->init();
        pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
        pDsDesc->setDepthWriteEnabled(true);

        _pDepthStencilState = _pDevice->newDepthStencilState(pDsDesc);

        pDsDesc->release();
    }

    void Renderer::buildBuffers()
    {
        const float s = 0.5f;
        const auto cube = Shapes::Cube(s);

        ShaderTypes::VertexData verts[] = {
            //   Positions          Normals
            {{-s, -s, +s}, {0.f, 0.f, 1.f}},
            {{+s, -s, +s}, {0.f, 0.f, 1.f}},
            {{+s, +s, +s}, {0.f, 0.f, 1.f}},
            {{-s, +s, +s}, {0.f, 0.f, 1.f}},

            {{+s, -s, +s}, {1.f, 0.f, 0.f}},
            {{+s, -s, -s}, {1.f, 0.f, 0.f}},
            {{+s, +s, -s}, {1.f, 0.f, 0.f}},
            {{+s, +s, +s}, {1.f, 0.f, 0.f}},

            {{+s, -s, -s}, {0.f, 0.f, -1.f}},
            {{-s, -s, -s}, {0.f, 0.f, -1.f}},
            {{-s, +s, -s}, {0.f, 0.f, -1.f}},
            {{+s, +s, -s}, {0.f, 0.f, -1.f}},

            {{-s, -s, -s}, {-1.f, 0.f, 0.f}},
            {{-s, -s, +s}, {-1.f, 0.f, 0.f}},
            {{-s, +s, +s}, {-1.f, 0.f, 0.f}},
            {{-s, +s, -s}, {-1.f, 0.f, 0.f}},

            {{-s, +s, +s}, {0.f, 1.f, 0.f}},
            {{+s, +s, +s}, {0.f, 1.f, 0.f}},
            {{+s, +s, -s}, {0.f, 1.f, 0.f}},
            {{-s, +s, -s}, {0.f, 1.f, 0.f}},

            {{-s, -s, -s}, {0.f, -1.f, 0.f}},
            {{+s, -s, -s}, {0.f, -1.f, 0.f}},
            {{+s, -s, +s}, {0.f, -1.f, 0.f}},
            {{-s, -s, +s}, {0.f, -1.f, 0.f}},
        };

        uint16_t indices[] = {
            0, 1, 2, 2, 3, 0,       /* front */
            4, 5, 6, 6, 7, 4,       /* right */
            8, 9, 10, 10, 11, 8,    /* back */
            12, 13, 14, 14, 15, 12, /* left */
            16, 17, 18, 18, 19, 16, /* top */
            20, 21, 22, 22, 23, 20, /* bottom */
        };

        const size_t vertexDataSize = sizeof(verts);
        const size_t indexDataSize = sizeof(indices);

        _pVertexDataBuffer = _pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        _pIndexBuffer = _pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        memcpy(_pVertexDataBuffer->contents(), verts, vertexDataSize);
        memcpy(_pIndexBuffer->contents(), indices, indexDataSize);

        _pVertexDataBuffer->didModifyRange(NS::Range::Make(0, _pVertexDataBuffer->length()));
        _pIndexBuffer->didModifyRange(NS::Range::Make(0, _pIndexBuffer->length()));

        const size_t instanceDataSize = kMaxFrames * kNumInstances * sizeof(ShaderTypes::InstanceData);
        const size_t cameraDataSize = kMaxFrames * sizeof(ShaderTypes::CameraData);
        for (size_t i = 0; i < kMaxFrames; ++i)
        {
            _pInstanceDataBuffer[i] = _pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
            _pCameraDataBuffer[i] = _pDevice->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged);
        }
    }
}
