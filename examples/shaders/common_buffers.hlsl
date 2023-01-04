/*  Common constant buffers
*/

#ifndef PPGE_COMMON_BUFFERS
#define PPGE_COMMON_BUFFERS

cbuffer cb_renderer : register(b0)
{
    uint   g_renderer_options              : packoffset(c0);
    float2 g_renderer_resolution           : packoffset(c1);
    float  g_renderer_shadowmap_resolution : packoffset(c1.z);
} 

cbuffer cb_per_frame : register(b1)
{
    float4x4 g_view            : packoffset(c0);
    float4x4 g_viewInv         : packoffset(c4);
    float4x4 g_proj            : packoffset(c8);
    float4x4 g_projInv         : packoffset(c12);
    float4x4 g_viewProj        : packoffset(c16);
    float4x4 g_viewProjInv     : packoffset(c20);

    float3   g_camera_position  : packoffset(c24);
    float3   g_camera_direction : packoffset(c25);
};

cbuffer cb_per_draw : register(b2)
{
    float4x4 g_world            : packoffset(c0);
    float4x4 g_worldInvTran     : packoffset(c4);
    uint     g_entity_id        : packoffset(c8.x);
};

cbuffer cb_material : register(b3)
{
    float4   g_albedo_color     : packoffset(c0);
    float4   g_emissive_color   : packoffset(c1);

    float    g_roughness_factor : packoffset(c2.x);
    float    g_metalic_factor   : packoffset(c2.y);
    float    g_alpha_cutoff     : packoffset(c2.z);

    uint     g_material_options : packoffset(c3.x);
}

cbuffer cb_light : register(b4)
{
    float4x4 g_light_viewProj[6]       : packoffset(c0);
    float3   g_light_position          : packoffset(c24);
    float3   g_light_direction         : packoffset(c25);
    uint     g_light_options           : packoffset(c26);
    float3   g_light_color             : packoffset(c27);    // Valid for all light types (r, g, b)
    float    g_light_intensity         : packoffset(c27.w);  // Valid for all light types
    float3   g_light_dist_attenuation  : packoffset(c28);    // Only valid for point and spot light (a0, a1, a2)
    float    g_light_range             : packoffset(c28.w);  // Only valid for point and spot light
    float3   g_light_angle_attenuation : packoffset(c29);    // Only valid for spot light (theta_max, theta_inner, decay_rate)
};

#endif // PPGE_COMMON_BUFFERS
