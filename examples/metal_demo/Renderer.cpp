#include <Renderer.hpp>
#include <AnimationSystem.h>

namespace AnimationSystem
{
    Renderer::Renderer(MTL::Device *device) : _pDevice(device->retain())
    {
        _pCommandQueue = _pDevice->newCommandQueue();
        buildShaders();
        buildBuffers();
        buildFrameData();

        _semaphore = dispatch_semaphore_create(Renderer::kMaxFrames);
    }

    Renderer::~Renderer()
    {
        _pShaderLibrary->release();
        _pArgBuffer->release();
        _pVertexPositionBuffer->release();
        _pVertexColorBuffer->release();

        for (int i = 0; i < Renderer::kMaxFrames; ++i)
        {
            _pFrameData[i]->release();
        }

        _pPSO->release();
        _pCommandQueue->release();
        _pDevice->release();
    }

    void Renderer::draw(MTK::View *pView)
    {
        NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();

        _frame = (_frame + 1) % Renderer::kMaxFrames;
        MTL::Buffer *pFrameDataBuffer = _pFrameData[_frame];

        MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();

        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
        Renderer *pRenderer = this;

        // complete handler
        std::function<void(MTL::CommandBuffer *)> drawCallback = [pRenderer](MTL::CommandBuffer *pCmd)
        { dispatch_semaphore_signal(pRenderer->_semaphore); };
        pCmd->addCompletedHandler(drawCallback);

        reinterpret_cast<FrameData *>(pFrameDataBuffer->contents())->angle = _angle += 0.01f;
        pFrameDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(FrameData)));

        MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
        MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

        pEnc->setRenderPipelineState(_pPSO);

        pEnc->setVertexBuffer(_pArgBuffer, 0, 0);
        pEnc->useResource(_pVertexPositionBuffer, MTL::ResourceUsageRead);
        pEnc->useResource(_pVertexColorBuffer, MTL::ResourceUsageRead);

        pEnc->setVertexBuffer(pFrameDataBuffer, 0, 1);
        pEnc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

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

    void Renderer::buildBuffers()
    {
        const size_t NumVertices = 3;

        simd::float3 positions[NumVertices] = {
            {-0.8f, 0.8f, 0.0f},
            {0.0f, -0.8f, 0.0f},
            {+0.8f, 0.8f, 0.0f}};

        simd::float3 colors[NumVertices] = {
            {1.0, 0.3f, 0.2f},
            {0.8f, 1.0, 0.0f},
            {0.8f, 0.0f, 1.0}};

        const size_t positionsDataSize = NumVertices * sizeof(simd::float3);
        const size_t colorDataSize = NumVertices * sizeof(simd::float3);

        MTL::Buffer *pVertexPositionBuffer = _pDevice->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
        MTL::Buffer *pVertexColorBuffer = _pDevice->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

        _pVertexPositionBuffer = pVertexPositionBuffer;
        _pVertexColorBuffer = pVertexColorBuffer;

        memcpy(_pVertexPositionBuffer->contents(), positions, positionsDataSize);
        memcpy(_pVertexColorBuffer->contents(), colors, colorDataSize);

        _pVertexPositionBuffer->didModifyRange(NS::Range::Make(0, _pVertexPositionBuffer->length()));
        _pVertexColorBuffer->didModifyRange(NS::Range::Make(0, _pVertexColorBuffer->length()));

        using NS::StringEncoding::UTF8StringEncoding;
        assert(_pShaderLibrary);

        // Create new buffer
        MTL::Function *pVertexFn = _pShaderLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
        MTL::ArgumentEncoder *pArgEncoder = pVertexFn->newArgumentEncoder(0);
        _pArgBuffer = _pDevice->newBuffer(pArgEncoder->encodedLength(), MTL::ResourceStorageModeManaged);

        // set arg buffer
        pArgEncoder->setArgumentBuffer(_pArgBuffer, 0);
        pArgEncoder->setBuffer(_pVertexPositionBuffer, 0, 0);
        pArgEncoder->setBuffer(_pVertexColorBuffer, 0, 1);
        _pArgBuffer->didModifyRange(NS::Range::Make(0, _pArgBuffer->length()));

        // no leaks!
        pVertexFn->release();
        pArgEncoder->release();
    }

    void Renderer::buildFrameData()
    {
        for (int i = 0; i < Renderer::kMaxFrames; ++i)
        {
            _pFrameData[i] = _pDevice->newBuffer(sizeof(FrameData), MTL::ResourceStorageModeManaged);
        }
    }
}
