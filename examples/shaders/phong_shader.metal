/*
 Phong shading model.
 */

#include <metal_stdlib>
#include <metal_common>
#include <simd/simd.h>

using namespace metal;

        struct SkinnedVertexData
        {
            simd::float3 position;
            simd::float3 normal;
            simd::float2 texcoord;
            simd::float4 jointWeights;
            simd::int4 jointIndex;
        };

        struct UniformData
        {
            simd::float4x4 model_matrix;
            simd::float4x4 view_matrix;
            simd::float4x4 projection_matrix;
        };

        struct JointGlobalPoseData
        {
            simd::float4x4 globalPose;
        };

struct ColorInOut {
    float4 position [[position]];
    float3 normal_cameraspace;
    float3 eye_direction_cameraspace;
    float3 light_direction_cameraspace;
};

// Global constants
constant float3 light_position = float3(-1.0, 1.0, -1.0);
constant float4 light_color = float4(1.0, 1.0, 1.0, 1.0);
constant float4 materialAmbientColor = float4(0.18, 0.18, 0.18, 1.0);
constant float4 materialDiffuseColor = float4(0.4, 0.4, 0.4, 1.0);
constant float4 materialSpecularColor = float4(1.0, 1.0, 1.0, 1.0);
constant float  materialShine = 50.0;
constant float4x4 identity = float4x4({1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0});
//constant float4x4 zeroM = float4x4({0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0});

// Phong vertex shader function
vertex ColorInOut phong_vertex( device const SkinnedVertexData* vertexData [[ buffer(0) ]],
                                constant UniformData& uniforms [[ buffer(1) ]],
                                device const JointGlobalPoseData* jointPoses [[ buffer(2) ]],
                                unsigned int vid [[ vertex_id ]])
{
    ColorInOut out;
    
    float4x4 model_matrix = uniforms.model_matrix;
    float4x4 view_matrix = uniforms.view_matrix;
    float4x4 mvp_matrix = uniforms.projection_matrix * view_matrix * model_matrix;
    float4x4 model_view_matrix = view_matrix * model_matrix;
    
    // calculate effected joint weights and their poses.
    int4 jIndex = vertexData[vid].jointIndex;
    float4 jWieghts = vertexData[vid].jointWeights;
    
    float4x4 joint_transform = jointPoses[jIndex[0]].globalPose * jWieghts.x;
    joint_transform += jointPoses[jIndex[1]].globalPose * jWieghts.y;
    joint_transform += jointPoses[jIndex[2]].globalPose * jWieghts.z;
    joint_transform += jointPoses[jIndex[3]].globalPose * jWieghts.w;
    
    // Calculate the position of the object from the perspective of the camera
    float4 skinned_vertex_position_model_space = joint_transform * float4(float3(vertexData[vid].position), 1.0f );

    out.position = mvp_matrix * skinned_vertex_position_model_space;
    
    // Calculate the normal from the perspective of the camera
    float3 normal = vertexData[vid].normal;
    out.normal_cameraspace = (normalize(model_view_matrix * float4(normal, 0.0f))).xyz;
    
    // Calculate the view vector from the perspective of the camera
    float3 vertex_position_cameraspace = ( view_matrix * model_matrix * skinned_vertex_position_model_space ).xyz;
    out.eye_direction_cameraspace = float3(0.0f,0.0f,0.0f) - vertex_position_cameraspace;
    
    // Calculate the direction of the light from the position of the camera
    float3 light_position_cameraspace = ( view_matrix * float4(light_position,1.0f)).xyz;
    out.light_direction_cameraspace = light_position_cameraspace + out.eye_direction_cameraspace;
    
    return out;
}

// Phong fragment shader function
fragment half4 phong_fragment(ColorInOut in [[stage_in]])
{
    half4 color;
    
    // Get the ambient color (the color that represents all the light that bounces around
    // the scene and illuminates the object).
    float4 ambient_color = materialAmbientColor;
    
    // Calculate the diffuse color (the color of the object given by direct illumination).
    // This is done by using the dot product between the surface normal and the light
    // vector to estimate how much the suface is facing towards the light.
    float3 n = normalize(in.normal_cameraspace);
    float3 l = normalize(in.light_direction_cameraspace);
    float n_dot_l = saturate( dot(n, l) );
    
    float4 diffuse_color = light_color * n_dot_l * materialDiffuseColor;
    
    // Calculate the specular color (the color given by the bright higlight of a shiny
    // object). This is done by using the dot product to calculate how close the
    // reflection of the light is pointing towards the viewer (e). The angle is raised by
    // the materialShine factor to control the size of the highlight.
    float3 e = normalize(in.eye_direction_cameraspace);
    float3 r = -l + 2.0f * n_dot_l * n;
    float e_dot_r =  saturate( dot(e, r) );
    float4 specular_color = materialSpecularColor * light_color * pow(e_dot_r, materialShine);
    
    // Combine the ambient, specular and diffuse colors to get the final color
    color = half4(ambient_color + diffuse_color + specular_color);
    
    return color;
};
