#include "Renderer.hpp"
#include <Renderer/Shader/ShaderTypes.hpp>
#include <Renderer/Shapes/Cube.hpp>

namespace AnimationSystem
{
    namespace
    {
        // PRIVATE FREE FUNCTIONS
    }
    Renderer::Renderer(MTL::Device *device) : _pDevice{device->retain()},
                                              _angle{0.f},
                                              _frame{0},
                                              _animationIndex{0}
    {
        std::cout << "----> TESTING IMPORTER \n";

        _pCommandQueue = _pDevice->newCommandQueue();
        buildShaders();
        std::cout << "----> buildShaders finished \n";
        buildComputePipeline();
        std::cout << "----> buildComputePipeline finished \n";
        buildDepthStencilStates();
        std::cout << "----> buildDepthStencilStates finished \n";
        buildTextures();
        std::cout << "----> buildTextures finished \n";
        buildBuffers();
        std::cout << "----> buildBuffers finished \n";

        _semaphore = dispatch_semaphore_create(Renderer::kMaxFrames);
    }

    Renderer::~Renderer()
    {
        _pTextureAnimationBuffer->release();
        _pTexture->release();
        _pShaderLibrary->release();
        _pDepthStencilState->release();
        _pVertexDataBuffer->release();

        for (int i = 0; i < Renderer::kMaxFrames; ++i)
        {
            _pInstanceDataBuffer[i]->release();
            _pCameraDataBuffer[i]->release();
        }

        _pIndexBuffer->release();
        _pComputePSO->release();
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
            float4x4 scale = Math::scaleM((float3){scl, scl, scl});
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
        pCameraData->projectionM = Math::makePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
        pCameraData->viewM = Math::makeIdentity();
        pCameraDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(ShaderTypes::CameraData)));

        // update texture

        generateMandelbrotTexture(pCmd);

        // Begin render pass:

        MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
        MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

        pEnc->setRenderPipelineState(_pPSO);
        pEnc->setDepthStencilState(_pDepthStencilState);

        pEnc->setVertexBuffer(_pVertexDataBuffer, /* offset */ 0, /* index */ 0);
        pEnc->setVertexBuffer(pInstanceDataBuffer, /* offset */ 0, /* index */ 1);
        pEnc->setVertexBuffer(pCameraDataBuffer, /* offset */ 0, /* index */ 2);

        pEnc->setFragmentTexture(_pTexture, /* index */ 0);

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

        const char *shaderSrc = R"(
            #include <metal_stdlib>
            using namespace metal;

            struct v2f
            {
                float4 position [[position]];
                float3 normal;
                half3 color;
                float2 texcoord;
            };

            struct VertexData
            {
                float3 position;
                float3 normal;
                float2 texcoord;
            };

            struct InstanceData
            {
                float4x4 instanceTransform;
                float3x3 instanceNormalTransform;
                float4 instanceColor;
            };

            struct CameraData
            {
                float4x4 perspectiveTransform;
                float4x4 worldTransform;
                float3x3 worldNormalTransform;
            };

            v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
                                   device const InstanceData* instanceData [[buffer(1)]],
                                   device const CameraData& cameraData [[buffer(2)]],
                                   uint vertexId [[vertex_id]],
                                   uint instanceId [[instance_id]] )
            {
                v2f o;

                const device VertexData& vd = vertexData[ vertexId ];
                float4 pos = float4( vd.position, 1.0 );
                pos = instanceData[ instanceId ].instanceTransform * pos;
                pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
                o.position = pos;

                float3 normal = instanceData[ instanceId ].instanceNormalTransform * vd.normal;
                normal = cameraData.worldNormalTransform * normal;
                o.normal = normal;

                o.texcoord = vd.texcoord.xy;

                o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
                return o;
            }

            half4 fragment fragmentMain( v2f in [[stage_in]], texture2d< half, access::sample > tex [[texture(0)]] )
            {
                constexpr sampler s( address::repeat, filter::linear );
                half3 texel = tex.sample( s, in.texcoord ).rgb;

                // assume light coming from (front-top-right)
                float3 l = normalize(float3( 1.0, 1.0, 0.8 ));
                float3 n = normalize( in.normal );

                half ndotl = half( saturate( dot( n, l ) ) );

                half3 illum = (in.color * texel * 0.1) + (in.color * texel * ndotl);
                return half4( illum, 1.0 );
            }
        )";
//        const std::filesystem::path shaderPath = "./shaders/example_shader.metal";
//        auto shaderSrc = Reader<std::string>(shaderPath).read();
//        const char *c_shaderSrc = shaderSrc.c_str();
//        if (shaderSrc.empty())
//        {
//            std::cout << "[ERROR] When reading File " << shaderPath << ". It does not exist or something worse...\n";
//            return;
//        }

        NS::Error *pError = nullptr;
        MTL::Library *pLibrary = _pDevice->newLibrary(NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError);

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

        pVertexFn->release();
        pFragFn->release();

        _pPSO = _pDevice->newRenderPipelineState(pDesc, &pError);
        if (!_pPSO)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

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
        // get uniform cube that has 1/2 size each edge
        auto cube = Shapes::Cube(0.5f);

        const size_t vertexDataSize = sizeof(cube.verts);
        const size_t indexDataSize = sizeof(cube.indices);

        MTL::Buffer *a = _pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
        MTL::Buffer *b = _pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

        _pVertexDataBuffer = a;
        _pIndexBuffer = b;

        memcpy(_pVertexDataBuffer->contents(), cube.verts, vertexDataSize);
        memcpy(_pIndexBuffer->contents(), cube.indices, indexDataSize);

        _pVertexDataBuffer->didModifyRange(NS::Range::Make(0, _pVertexDataBuffer->length()));
        _pIndexBuffer->didModifyRange(NS::Range::Make(0, _pIndexBuffer->length()));

        const size_t instanceDataSize = kMaxFrames * kNumInstances * sizeof(ShaderTypes::InstanceData);
        const size_t cameraDataSize = kMaxFrames * sizeof(ShaderTypes::CameraData);
        for (size_t i = 0; i < kMaxFrames; ++i)
        {
            _pInstanceDataBuffer[i] = _pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
            _pCameraDataBuffer[i] = _pDevice->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged);
        }

        _pTextureAnimationBuffer = _pDevice->newBuffer(sizeof(uint), MTL::ResourceStorageModeManaged);
    }

    void Renderer::buildTextures()
    {
        auto tw = kTextureWidth;
        auto th = kTextureHeight;
        MTL::TextureDescriptor *pTextureDesc = MTL::TextureDescriptor::alloc()->init();
        pTextureDesc->setWidth(kTextureWidth);
        pTextureDesc->setHeight(kTextureHeight);
        pTextureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
        pTextureDesc->setTextureType(MTL::TextureType2D);
        pTextureDesc->setStorageMode(MTL::StorageModeManaged);
        pTextureDesc->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead | MTL::ResourceUsageWrite);
        MTL::Texture *pTexture = _pDevice->newTexture(pTextureDesc);
        _pTexture = pTexture;

        uint8_t *pTextureData = (uint8_t *)alloca(tw * th * 4);
        for (size_t y = 0; y < th; ++y)
        {
            for (size_t x = 0; x < tw; ++x)
            {
                bool isWhite = (x ^ y) & 0b1000000;
                uint8_t c = isWhite ? 0xFF : 0xA;

                size_t i = y * tw + x;

                pTextureData[i * 4 + 0] = c;
                pTextureData[i * 4 + 1] = c;
                pTextureData[i * 4 + 2] = c;
                pTextureData[i * 4 + 3] = 0xFF;
            }
        }

        _pTexture->replaceRegion(MTL::Region(0, 0, 0, tw, th, 1), 0, pTextureData, tw * 4);

        pTextureDesc->release();
    }

    void Renderer::buildComputePipeline()
    {
        const char *kernelSrc = R"(
            #include <metal_stdlib>
            using namespace metal;

            kernel void mandelbrot_set(texture2d< half, access::write > tex [[texture(0)]],
                                       uint2 index [[thread_position_in_grid]],
                                       uint2 gridSize [[threads_per_grid]],
                                       device const uint* frame [[buffer(0)]])
            {
                constexpr float kAnimationFrequency = 0.01;
                constexpr float kAnimationSpeed = 4;
                constexpr float kAnimationScaleLow = 0.62;
                constexpr float kAnimationScale = 0.38;

                constexpr float2 kMandelbrotPixelOffset = {-0.2, -0.35};
                constexpr float2 kMandelbrotOrigin = {-1.2, -0.32};
                constexpr float2 kMandelbrotScale = {2.2, 2.0};

                // Map time to zoom value in [kAnimationScaleLow, 1]
                float zoom = kAnimationScaleLow + kAnimationScale * cos(kAnimationFrequency * *frame);
                // Speed up zooming
                zoom = pow(zoom, kAnimationSpeed);

                //Scale
                float x0 = zoom * kMandelbrotScale.x * ((float)index.x / gridSize.x + kMandelbrotPixelOffset.x) + kMandelbrotOrigin.x;
                float y0 = zoom * kMandelbrotScale.y * ((float)index.y / gridSize.y + kMandelbrotPixelOffset.y) + kMandelbrotOrigin.y;

                // Implement Mandelbrot set
                float x = 0.0;
                float y = 0.0;
                uint iteration = 0;
                uint max_iteration = 1000;
                float xtmp = 0.0;
                while(x * x + y * y <= 4 && iteration < max_iteration)
                {
                    xtmp = x * x - y * y + x0;
                    y = 2 * x * y + y0;
                    x = xtmp;
                    iteration += 1;
                }

                // Convert iteration result to colors
                half color = (0.5 + 0.5 * cos(3.0 + iteration * 0.15));
                tex.write(half4(color, color, color, 1.0), index, 0);
            })";
//        const std::filesystem::path mandelbrotComputationFilePath = "./shaders/mandelbrot";
//        auto kernelSrc = Reader<std::string>(mandelbrotComputationFilePath).read();
//        const char *c_kernelSrc = kernelSrc.c_str();
        NS::Error *pError = nullptr;

        MTL::Library *pComputeLibrary = _pDevice->newLibrary(NS::String::string(kernelSrc, NS::UTF8StringEncoding), nullptr, &pError);
        if (!pComputeLibrary)
        {
            std::cout << "[AnimationSystem::ERROR]" << pError->localizedDescription()->utf8String() << "\n";
            assert(false);
        }

        MTL::Function *pMandelBrotFn = pComputeLibrary->newFunction(NS::String::string("mandelbrot_set", NS::UTF8StringEncoding));
        _pComputePSO = _pDevice->newComputePipelineState(pMandelBrotFn, &pError);
        if (!_pComputePSO)
        {
            std::cout << "[AnimationSystem::ERROR]" << pError->localizedDescription()->utf8String() << "\n";
            assert(false);
        }

        pMandelBrotFn->release();
        pComputeLibrary->release();
    }

    void Renderer::generateMandelbrotTexture(MTL::CommandBuffer *pCommandBuffer)
    {
        assert(pCommandBuffer);

        uint *ptr = reinterpret_cast<uint *>(_pTextureAnimationBuffer->contents());
        *ptr = (_animationIndex++) % 5000;
        _pTextureAnimationBuffer->didModifyRange(NS::Range::Make(0, sizeof(uint)));

        MTL::ComputeCommandEncoder *pComputeEncoder = pCommandBuffer->computeCommandEncoder();
        pComputeEncoder->setComputePipelineState(_pComputePSO);
        pComputeEncoder->setTexture(_pTexture, 0);
        pComputeEncoder->setBuffer(_pTextureAnimationBuffer, 0, 0);

        MTL::Size gridSize = MTL::Size(kTextureWidth, kTextureHeight, 1);
        NS::UInteger uThreadGroupSize = _pComputePSO->maxTotalThreadsPerThreadgroup();
        MTL::Size threadGroupSize(uThreadGroupSize, 1, 1);

        pComputeEncoder->dispatchThreads(gridSize, threadGroupSize);
        pComputeEncoder->endEncoding();
    }
}
