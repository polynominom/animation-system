#include "CoreTestViewDelegate.hpp"
#include <iostream>
namespace Example
{
    CoreTestViewDelegate::CoreTestViewDelegate(MTL::Device *pDevice) : MTK::ViewDelegate()
    {
        std::string characterPath = "./characters";
        std::string shaderPath = "./shaders";
        std::string blendSpecificationsPath = "./blend_specifications";
#if defined(CHARACTER_MODEL_FOLDER)
        characterPath = CHARACTER_MODEL_FOLDER;
#endif
#if defined(SHADER_FOLDER)
        shaderPath = SHADER_FOLDER;
#endif
#if defined(BLEND_SPEC_FOLDER)
        blendSpecificationsPath = BLEND_SPEC_FOLDER;
#endif
        characterPath += "/character.fbx";
        shaderPath+="/phong_shader.metal";
        std::string locomotionBlendSpecPath = blendSpecificationsPath + "/locomotion.b3";
        
        _pAnimSystem = std::make_unique<AnimationSystem::AnimationSystem>();

        // initialize the resources of the animation system
        _pAnimSystem->setCommonShaderPath(shaderPath);
        _pAnimSystem->setCommonCharacterPath(characterPath);
        _pAnimSystem->setLocomotionBlendSpecificationPath(locomotionBlendSpecPath);
        
        // set the renderer
        _pAnimSystem->setRenderer(pDevice);
    }
    CoreTestViewDelegate::~CoreTestViewDelegate()
    {
//        delete this->_pAnimSystem;
    }

    void CoreTestViewDelegate::drawInMTKView(MTK::View *pView)
    {
        _pAnimSystem->getRenderer()->draw(pView);
    }
} // namespace AnimationSystem
