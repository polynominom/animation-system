#include "MetalRenderer.hpp"
#include <Core/CommonConsts.hpp>
#include <Renderer/BufferHelper.hpp>
#include <sys/time.h>

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

    MetalRenderer::MetalRenderer(MTL::Device *device) : _frame{0},
                                                        _angle{0.0f},
                                                        _drawIndex{0},
                                                        _animationIndex{0}
    {
        RendererManager::init(device);

        _pDevice = RendererManager::getDevice();
        _zoom = 0;

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

        const std::filesystem::path shaderPath{COMMON_applPhongShaderPath};
        AnimationSystem::ShaderResource::load(shaderPath, _shaderLibrary);
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

        uint8_t *pTextureData = (uint8_t *) alloca(tw * th * 4);
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
        auto meshes = Import::loadMeshes(COMMON_dudeFBXPath.data());

        std::cout << "meshes successfully parsed: " << meshes.size() << "\n";
        Shapes::Cube cube;
        int index = 0;
        // handle meshes
        for (auto m : meshes)
        {
            Entity e;
            e.setPosition({2.f, -2.f, -140.f, 1.f});
            e.setScale((simd::float4){.5f, .5f, .5f, 1.f});

            std::shared_ptr<MeshComponent> meshComp = std::make_shared<MeshComponent>();

            meshComp->mesh = std::move(m);
            meshComp->mesh->buildBuffers();
            auto c = static_cast<std::shared_ptr<Component>>(meshComp);
            e.addComponent(c);
            _scene->entities.push_back(e);
        }
        
        // build camera
        _scene->camera = std::make_shared<Camera>();
        _scene->camera->initData({.0, .0, -10.f});

        // build uniform buffer
        _pUniformBuffer = _pDevice->newBuffer(sizeof(ShaderTypes::UniformData), MTL::ResourceStorageModeManaged);
    }

    void MetalRenderer::draw(MTK::View *pView)
    {
        NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();
        _scene->camera->updateData();
        
        // render entities that has mesh components.
        MetalRenderer *pRenderer = this;
        // complete handler
        std::function<void(MTL::CommandBuffer *)> drawCallback = [pRenderer](MTL::CommandBuffer *pCmd)
        { dispatch_semaphore_signal(pRenderer->_semaphore); };

        MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();
        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);

        pCmd->addCompletedHandler(drawCallback);
        size_t entityIndex = 0;
        _angle += 0.02f;
        simd::float4x4 rr1 = Math::rotateY(-11);
        simd::float4x4 rr0 = Math::rotateX(6);
        
        long long timeMs = RendererManager::getCurrentTime();
        float timeInSec = ((float)(timeMs - RendererManager::getStartTime())) / 1000.0f;
        
        for (auto &ent : _scene->filterEntities("MeshComponent"))
        {
            auto meshComp = std::dynamic_pointer_cast<MeshComponent>(ent.getComponent("MeshComponent"));
            
            // MATRIX MULTIPLICATIONS FOR ROTATION, POSITION, SCALE
            //std::cout << "x: "<<ent.getScale().x << ", y" << ent.getScale().y << ", Z: "<<ent.getScale().z<<std::endl;
            auto scale = Math::scale({ent.getScale().x,ent.getScale().y,ent.getScale().z});
            auto objectPosition = Math::add(ent.getPosition().xyz, {0., -14., 0.0});
        
            simd::float4x4 rt = Math::translate(objectPosition);
            simd::float4x4 rtInv = Math::translate({-objectPosition.x, -objectPosition.y, -objectPosition.z});
            simd::float4x4 worldM = rt * rr1 * rr0 * scale;
            
            BufferHelper::updateUniformBuffer(_pUniformBuffer, _scene->camera->data(), worldM);
            
            // Begin render pass:
            MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
            
            // Change load action to prevent fleckiring.
            auto loadAction = (entityIndex == 0) ? MTL::LoadActionClear : MTL::LoadActionLoad;
            pRpd->colorAttachments()->object(0)->setLoadAction(loadAction);
            
            MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);
            pEnc->setRenderPipelineState(_pPSO);
            pEnc->setDepthStencilState(_pDepthStencilState);
            pEnc->setVertexBuffer(meshComp->mesh->pVertexBuffer, /* offset */ 0, /* index */ 0);
            pEnc->setVertexBuffer(_pUniformBuffer, /* offset */ 0, /* index */ 1);
            pEnc->setVertexBuffer(meshComp->mesh->pJointBuffer, /* offset */ 0, /* index */ 2);
            pEnc->setFragmentTexture(_pTexture, /* index */ 0);
            pEnc->setCullMode(MTL::CullModeBack);
            pEnc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

            pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                        meshComp->mesh->numberOfIndices,
                                        MTL::IndexType::IndexTypeUInt16,
                                        meshComp->mesh->pIndexBuffer, 0, 1);
            
            pEnc->endEncoding();
            pView->clearDepth();

            ++entityIndex;
        }
        pCmd->presentDrawable(pView->currentDrawable());
        pCmd->commit();
    }

} // namespace AnimationSystem
