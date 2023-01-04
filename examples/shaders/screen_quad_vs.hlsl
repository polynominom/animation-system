/*  VS that generates an oversized triangle w/o 
    vertex buffer. Use it with null input layout
    and Draw(3)
*/

void main_VS(in uint VertexID     : SV_VERTEXID,
             out float4 PosH      : SV_POSITION,
             out float2 TexCoord0 : TEXTURE0)
{
    // VertexID will be 0, 1 and 3 only.

    // TexCoord0 will be (0, 0), (0, 2) and (2, 0)
    TexCoord0 = float2(uint2(VertexID, VertexID << 1) & 2);
    // PosH will be (-1, 1), (-1, -3), (3, 1)
    PosH = float4(lerp(float2(-1, 1), float2(1, -1), TexCoord0), 0, 1);
}
