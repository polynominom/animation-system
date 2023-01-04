/*  Helpers for PBR theory
*/

#ifndef PPGE_PBR_HELPERS
#define PPGE_PBR_HELPERS

#include "common_constants.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"

// --------------------------------------------------
//                  SPECULAR MODELS
// --------------------------------------------------

float DTrowbridgeReitzGGX(float roughness, float n_dot_h)
{
    // Disney's reparametrization
    float alpha  = roughness * roughness;
    float alpha2 = alpha * alpha;

    float d = (n_dot_h * n_dot_h * (alpha2 - 1.0f) + 1.0f);
    return alpha2 / (PI * d * d); 
}

float VSmithGGXCorrelated(float roughness, float n_dot_l, float n_dot_e)
{
    float alpha = roughness * roughness;

    float l_GGXE = n_dot_l * sqrt((- n_dot_e * alpha + n_dot_e) * n_dot_e + alpha);
    float l_GGXL = n_dot_e * sqrt((- n_dot_l * alpha + n_dot_l) * n_dot_l + alpha);

    return 0.5f / (l_GGXE + l_GGXL);
}

float VSchlickGGX(float roughness, float n_dot_l, float n_dot_e)
{
    // Taken from Real Shading in Unreal Engine 4 by Brian Karis, Epic Games
    // https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
    float k = roughness + 1.0f;
    k = (k * k) / 8.0f;

    float l_GGXE = n_dot_e / (n_dot_e * (1.0f - k) + k);
    float l_GGXL = n_dot_l / (n_dot_l * (1.0f - k) + k);

    return l_GGXE * l_GGXL;
}

float3 FSchlick(float3 f0, float h_dot_e)
{
    return f0 + (1.0f - f0) * pow((1.0f - h_dot_e), 5.0f);
}

float3 SpecularBRDF(float roughness, float metalness, float3 f0, float n_dot_e, float n_dot_l, 
                    float h_dot_e, float h_dot_l, float n_dot_h, inout float3 specular_energy)
{
    float  D = DTrowbridgeReitzGGX(roughness, n_dot_h);
    float  V = VSchlickGGX(roughness, n_dot_l, n_dot_e);
    float3 F = FSchlick(f0, h_dot_e);

    specular_energy *= F;

    return (D * F * V) / max(FLT_MIN, (4 * n_dot_e * n_dot_l));
}

// --------------------------------------------------
//                  DIFFUSE MODELS
// --------------------------------------------------

float3 DisneyDiffuse(float3 color, float roughness, float n_dot_e, float n_dot_l, float h_dot_e)
{
    return float3(0.0f, 0.0f, 0.0f);
}

float3 OrenNayarDiffuse(float3 color, float roughness, float n_dot_e, float n_dot_l, float h_dot_e)
{
    // Conversion is suggested at "Moving Frostbite to Physically Based Rendering" by Lagarde & de Rousiers
    // https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
    float sigma = ROOT_TWO_INV * atan(roughness);
    float s2 = sigma * sigma;

    float EdotL = 2 * h_dot_e * h_dot_e - 1.0f;
    float cos_phi_diff = EdotL - n_dot_e * n_dot_l;

    float A = (1.0f - 0.5f * (s2 / (s2 + 0.65f)));
    float B = 0.45f * (s2 / (s2 + 0.09f));

    return color * PI_INV * (A + B * cos_phi_diff * (cos_phi_diff >= 0 ? rcp(max(n_dot_l + FLT_MIN, n_dot_e + FLT_MIN)) : 1));
}

float3 LambertDiffuse(float3 color)
{
    return color * PI_INV;
}

float3 DiffuseBRDF(float3 color, float roughness, float n_dot_e, float n_dot_l, float h_dot_e)
{
    //return LambertDiffuse(color);
    return OrenNayarDiffuse(color, roughness, n_dot_e, n_dot_l, h_dot_e);
}

#endif // PPGE_PBR_HELPERS
