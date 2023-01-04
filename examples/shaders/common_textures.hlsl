/*  Common textures
*/

#ifndef PPGE_COMMON_TEXTURES
#define PPGE_COMMON_TEXTURES

Texture2D      g_material_albedo     : register(t0);
Texture2D      g_material_roughness  : register(t1);
Texture2D      g_material_metalic    : register(t2);
Texture2D      g_material_normal     : register(t3);
Texture2D      g_material_occlusion  : register(t4);
Texture2D      g_material_emission   : register(t5);
Texture2D      g_material_height     : register(t6);
Texture2D      g_material_alpha_mask : register(t7);

Texture2D      g_buffer_albedo          : register(t8);
Texture2D      g_buffer_normal          : register(t9);
Texture2D      g_buffer_position        : register(t10);
Texture2D      g_buffer_material        : register(t11);
Texture2D      g_buffer_emission        : register(t12);

Texture2DArray g_shadow_map_dir_light   : register(t13);
TextureCube    g_shadow_map_point_light : register(t14);
Texture2D      g_shadow_map_spot_light  : register(t15);

#endif // PPGE_COMMON_TEXTURES
