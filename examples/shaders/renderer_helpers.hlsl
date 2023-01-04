/*  Helpers for renderer options
*/

#ifndef PPGE_RENDERER_HELPERS
#define PPGE_RENDERER_HELPERS

#include "common_buffers.hlsl"
#include "common_constants.hlsl"

#define RENDERER_ENABLE_NORMAL_MAP   uint(1U << 0)

inline bool   renderer_is_normal_map_enabled()    { return g_renderer_options & RENDERER_ENABLE_NORMAL_MAP; }
inline float2 renderer_get_resolution()           { return g_renderer_resolution;                           }
inline float  renderer_get_width()                { return g_renderer_resolution.x;                         }
inline float  renderer_get_height()               { return g_renderer_resolution.y;                         }
inline float  renderer_get_shadowmap_resolution() { return g_renderer_shadowmap_resolution;                 }
inline float  renderer_get_shadowmap_texel_size() { return 1.0f / g_renderer_shadowmap_resolution;          }

#endif // PPGE_RENDERER_HELPERS
