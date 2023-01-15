#include "CoreTestViewDelegate.hpp"
#include <iostream>
namespace Example
{
    CoreTestViewDelegate::CoreTestViewDelegate(MTL::Device *pDevice) : MTK::ViewDelegate()
    {
        std::string characterPath = "./characters";
        std::string shaderPath = "./shaders";
#if defined(CHARACTER_MODEL_FOLDER)
        characterPath = CHARACTER_MODEL_FOLDER;
#endif
#if defined(SHADER_FOLDER)
        shaderPath = SHADER_FOLDER;
#endif
        characterPath += "/character.fbx";
        shaderPath+="/phong_shader.metal";
        _pAnimSystem = new AnimationSystem::AnimationSystem(pDevice,
                                                            shaderPath,
                                                            characterPath);
    }
    CoreTestViewDelegate::~CoreTestViewDelegate()
    {
        delete this->_pAnimSystem;
    }

    void CoreTestViewDelegate::drawInMTKView(MTK::View *pView)
    {
        _pAnimSystem->getRenderer()->draw(pView);
    }
} // namespace AnimationSystem
