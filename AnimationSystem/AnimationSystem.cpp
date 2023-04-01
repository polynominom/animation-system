#include "AnimationSystem.hpp"
#include "CommonConsts.hpp"

namespace AnimationSystem
{
    AnimationSystem::AnimationSystem(MTL::Device *device)
    {
        // init the resource manager with given pre-determined paths
        _resourceManager = std::make_shared<ResourceManager>();
        _renderer = std::make_shared<MetalRenderer>(device, _resourceManager);
    }
    void AnimationSystem::setCommonCharacterPath(const std::string& characterPath)
    {
        _resourceManager->registerCharacterName(COMMON_exampleCharacterKey.data(), characterPath);
    }

    void AnimationSystem::setCommonShaderPath(const std::string& shaderPath)
    {
        _resourceManager->registerShaderName(COMMON_phongSahderKey.data(), shaderPath);
    }

    void AnimationSystem::setLocomotionBlendSpecificationPath(const std::string& locomotionBlendSpecPath)
    {
        _resourceManager->registerBlendSpecificationName(COMMON_locomotionBlendSpecKey.data(), locomotionBlendSpecPath);
    }
} // namespace AnimationSystem
