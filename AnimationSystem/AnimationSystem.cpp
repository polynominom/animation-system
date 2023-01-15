#include "AnimationSystem.hpp"
#include "CommonConsts.hpp"

namespace AnimationSystem
{
    AnimationSystem::AnimationSystem(MTL::Device *device, std::string shaderPath, std::string characterPath)
    {
        
        // init the resource manager with given pre-determined paths
        _resourceManager = std::make_shared<ResourceManager>();
        _resourceManager->registerCharacterName(COMMON_exampleCharacterKey.data(), characterPath);
        _resourceManager->registerShaderName(COMMON_phongSahderKey.data(), shaderPath);
        
        _renderer = std::make_shared<MetalRenderer>(device, _resourceManager);
    }
} // namespace AnimationSystem
