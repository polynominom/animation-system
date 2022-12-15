#include <Renderer.hpp>

namespace AnimationSystem
{
    Renderer::Renderer(MTL::Device *device) : _pDevice(device->retain())
    {
        _pCommandQueue = _pDevice->newCommandQueue();
    }

    Renderer::~Renderer()
    {
        _pCommandQueue->release();
        _pDevice->release();
    }

    void Renderer::draw(MTK::View *pView)
    {
        NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();

        MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();
        MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
        MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

        pEnc->endEncoding();
        pCmd->presentDrawable(pView->currentDrawable());
        pCmd->commit();

        pPool->release();
    }
}