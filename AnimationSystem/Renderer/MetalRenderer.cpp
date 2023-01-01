#include "MetalRenderer.hpp"

namespace AnimationSystem
{
    // hold 2-3 frames in memory
    static const int kMaxFrames{3};
    MetalRenderer::~MetalRenderer()
    {
        _pTexture->release();
        _pPSO->release();
        _pCommandQueue->release();
        _pDevice->release();
    }

    MetalRenderer::MetalRenderer(MTL::Device *device) : _pDevice{device->retain()},
                                                        _frame{0},
                                                        _animationIndex{0}
    {
        _pCommandQueue = _pDevice->newCommandQueue();
        buildShaders();
        std::cout << "----> buildShaders finished \n";
        // buildComputePipeline();
        // std::cout << "----> buildComputePipeline finished \n";
        buildDepthStencilStates();
        std::cout << "----> buildDepthStencilStates finished \n";
        buildTextures();
        std::cout << "----> buildTextures finished \n";
        buildEntities();
        std::cout << "----> buildBuffers finished \n";

        _semaphore = dispatch_semaphore_create(kMaxFrames);
    }

    void MetalRenderer::buildShaders()
    {

        _shaderLibrary = ShaderLibrary{};

        const std::filesystem::path shaderPath = "./shaders/example_shader.glsl";
        AnimationSystem::ShaderResource::load(_pDevice, shaderPath, _shaderLibrary);
        MTL::RenderPipelineDescriptor *pDesc = _shaderLibrary.registerPipelineDescriptor(shaderPath);

        NS::Error *pError = nullptr;
        _pPSO = _pDevice->newRenderPipelineState(pDesc, &pError);
        if (!_pPSO)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

        pDesc->release();
    }

    void MetalRenderer::buildDepthStencilStates()
    {
        MTL::DepthStencilDescriptor *pDsDesc = MTL::DepthStencilDescriptor::alloc()->init();
        pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
        pDsDesc->setDepthWriteEnabled(true);
        _pDepthStencilState = _pDevice->newDepthStencilState(pDsDesc);
        pDsDesc->release();
    }

    void MetalRenderer::buildTextures()
    {
        const auto tw = 128;
        const auto th = 128;

        MTL::TextureDescriptor *pTextureDesc = MTL::TextureDescriptor::alloc()->init();
        pTextureDesc->setWidth(tw);
        pTextureDesc->setHeight(th);
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

    void MetalRenderer::buildEntities()
    {
        // create scene
        _scene = std::make_shared<Scene>();

        // add one cube entity with mesh
        auto cube = Shapes::Cube(0.5f);
        Entity cubeEntity;
        cubeEntity.setPosition({0.f, 0.f, -10.f, 1.f});

        std::shared_ptr<MeshComponent> m = std::make_shared<MeshComponent>();
        m->mesh = std::make_shared<Mesh>();
        m->mesh->buildInstanceBuffer(_pDevice, sizeof(ShaderTypes::InstanceData));
        m->mesh->buildBuffers(_pDevice,
                              sizeof(cube.verts), cube.verts,
                              sizeof(cube.indices), cube.indices);

        // add the cube to the scene
        auto c = static_cast<std::shared_ptr<Component>>(m);
        cubeEntity.addComponent(c);
        _scene->entities.push_back(cubeEntity);

        // build camera
        _scene->camera = std::make_shared<Camera>();
        _scene->camera->setBuffer(_pDevice);

        //_pTextureAnimationBuffer = _pDevice->newBuffer(sizeof(uint), MTL::ResourceStorageModeManaged);
    }

    void MetalRenderer::draw(MTK::View *pView)
    {
        using simd::float3;
        using simd::float4;
        using simd::float4x4;

        NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();
        MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();
        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);

        MetalRenderer *pRenderer = this;
        // complete handler
        std::function<void(MTL::CommandBuffer *)> drawCallback = [pRenderer](MTL::CommandBuffer *pCmd)
        { dispatch_semaphore_signal(pRenderer->_semaphore); };
        pCmd->addCompletedHandler(drawCallback);

        const float scl = 0.2f;

        _scene->camera->updateData();

        // render entities that has mesh components.
        for (auto &ent : _scene->filterEntities("MeshComponent"))
        {
            auto meshComp = std::dynamic_pointer_cast<MeshComponent>(ent.getComponent("MeshComponent"));
            ShaderTypes::InstanceData *pInstanceData = meshComp->mesh->getInstanceData();

            // MATRIX MULTIPLICATIONS FOR ROTATION, POSITION, SCALE
            // INSTANCE MANIPULATION
            auto scale = Math::scale((float3){scl, scl, scl});
            auto translate = Math::translate(Math::add(ent.getPosition().xyz, {0., 2., 2.}));
            pInstanceData->instanceTransform = Math::makeIdentity() * translate * scale;
            pInstanceData->instanceNormalTransform = Math::discardTranslation(pInstanceData->instanceTransform);
            pInstanceData->instanceColor = (simd::float4){0.32, 0.1, 0.1, 1.0f};
            meshComp->mesh->pInstanceBuffer->didModifyRange(NS::Range::Make(0, meshComp->mesh->pInstanceBuffer->length()));

            // Begin render pass:

            MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
            MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

            pEnc->setRenderPipelineState(_pPSO);
            pEnc->setDepthStencilState(_pDepthStencilState);

            pEnc->setVertexBuffer(meshComp->mesh->pVertexBuffer, /* offset */ 0, /* index */ 0);
            pEnc->setVertexBuffer(meshComp->mesh->pInstanceBuffer, /* offset */ 0, /* index */ 1);
            pEnc->setVertexBuffer(_scene->camera->getBuffer(), /* offset */ 0, /* index */ 2);

            pEnc->setFragmentTexture(_pTexture, /* index */ 0);

            pEnc->setCullMode(MTL::CullModeBack);
            pEnc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

            pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                        6 * 6, MTL::IndexType::IndexTypeUInt16,
                                        meshComp->mesh->pIndexBuffer,
                                        0,
                                        1);

            pEnc->endEncoding();
        }

        pCmd->presentDrawable(pView->currentDrawable());
        pCmd->commit();

        pPool->release();
    }

} // namespace AnimationSystem
