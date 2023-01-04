//
//  CommonConsts.hpp
//  metal_example
//
//  Created by Çağrı Yıldız on 03.01.23.
//

#ifndef CommonConsts_h
#define CommonConsts_h

#include <string>

namespace AnimationSystem {

//MESHES
const std::string_view COMMON_dudeMeshPath = "/Users/cagriyildiz/Downloads/Dude/Dude.obj";
const std::string_view COMMON_exampleMeshPath = "/Users/cagriyildiz/projects/animation-system/meshes/Spider/Spider.obj";
const std::string_view COMMON_dudeFBXPath = "/Users/cagriyildiz/Downloads/DigitalRune/Samples/Content/Dude/Dude.fbx";

// SHADERS
const std::string_view COMMON_hlslCommonShaderPath = "/Users/cagriyildiz/Downloads/shaders/common_vs.hlsl";
const std::string_view COMMON_applPhongShaderPath = "/Users/cagriyildiz/.shaders/AAPLPhongShader.metal";
const std::string_view COMMON_exampleShaderPath = "/Users/cagriyildiz/projects/animation-system/examples/shaders/example_shader.metal";

}


#endif /* CommonConsts_h */
