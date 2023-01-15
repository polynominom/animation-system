#include "MetalFrameDebugger.hpp"

namespace AnimationSystem
{
    extern "C" NS::String *NSTemporaryDirectory(void);

    void MetalFrameDebugger::triggerCapture(MTL::Device *pDevice)
    {
        bool success;

        MTL::CaptureManager *pCaptureManager = MTL::CaptureManager::sharedCaptureManager();
        success = pCaptureManager->supportsDestination(MTL::CaptureDestinationGPUTraceDocument);
        if (!success)
        {
            __builtin_printf("Capture support is not enabled\n");
            assert(false);
        }

        char filename[NAME_MAX];
        std::time_t now;
        std::time(&now);
        std::strftime(filename, NAME_MAX, "capture-%H-%M-%S_%m-%d-%y.gputrace", std::localtime(&now));

        _pTraceSaveFilePath = NSTemporaryDirectory()->stringByAppendingString(NS::String::string(filename, NS::UTF8StringEncoding));
        NS::URL *pURL = NS::URL::alloc()->initFileURLWithPath(_pTraceSaveFilePath);

        MTL::CaptureDescriptor *pCaptureDescriptor = MTL::CaptureDescriptor::alloc()->init();

        pCaptureDescriptor->setDestination(MTL::CaptureDestinationGPUTraceDocument);
        pCaptureDescriptor->setOutputURL(pURL);
        pCaptureDescriptor->setCaptureObject(pDevice);

        NS::Error *pError = nullptr;

        success = pCaptureManager->startCapture(pCaptureDescriptor, &pError);
        if (!success)
        {
            __builtin_printf("Failed to start capture: \"%s\" for file \"%s\"\n", pError->localizedDescription()->utf8String(), _pTraceSaveFilePath->utf8String());
            assert(false);
        }

        pURL->release();
        pCaptureDescriptor->release();
    }
} // namespace AnimationSystem
