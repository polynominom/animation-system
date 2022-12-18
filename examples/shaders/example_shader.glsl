#include <metal_stdlib>
using namespace metal;
struct v2f
{
    float4 position [[position]];
    half3 color;
};
struct VertexData
{
    float3 position;
};
struct InstanceData
{
    float4x4 instanceTransform;
    float4 instanceColor;
};
struct CameraData
{
    float4x4 perspectiveTransform;
    float4x4 worldTransform;
};
v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
                       device const InstanceData* instanceData [[buffer(1)]],
                       device const CameraData& cameraData [[buffer(2)]],
                       uint vertexId [[vertex_id]],
                       uint instanceId [[instance_id]] )
{
    v2f o;
    float4 pos = float4( vertexData[ vertexId ].position, 1.0 );
    pos = instanceData[ instanceId ].instanceTransform * pos;
    pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
    o.position = pos;
    o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
    return o;
}
half4 fragment fragmentMain( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}
