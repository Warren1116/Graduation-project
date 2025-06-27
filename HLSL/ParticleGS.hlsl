#include "Particle.hlsli"

[maxvertexcount(6)]
void main(point VS_OUT input[1], inout TriangleStream<PS_IN> triStream)
{
    float3 pos = input[0].worldPos;
    float size = 0.5f;

    float3 right = float3(view._11, view._21, view._31);
    float3 up = float3(view._12, view._22, view._32);

    float3 offset[] =
    {
        -right + up,
         right + up,
        -right - up,
         right - up
    };

    float2 uv[] =
    {
        float2(0, 0),
        float2(1, 0),
        float2(0, 1),
        float2(1, 1)
    };

    PS_IN v0, v1, v2;

    v0.pos = mul(float4(pos + offset[0] * size, 1.0f), viewProjection);
    v1.pos = mul(float4(pos + offset[1] * size, 1.0f), viewProjection);
    v2.pos = mul(float4(pos + offset[2] * size, 1.0f), viewProjection);
    v0.uv = uv[0];
    v1.uv = uv[1];
    v2.uv = uv[2];
    v0.life = v1.life = v2.life = input[0].life;
    triStream.Append(v0);
    triStream.Append(v1);
    triStream.Append(v2);

    PS_IN v3, v4, v5;
    v3.pos = mul(float4(pos + offset[2] * size, 1.0f), viewProjection);
    v4.pos = mul(float4(pos + offset[1] * size, 1.0f), viewProjection);
    v5.pos = mul(float4(pos + offset[3] * size, 1.0f), viewProjection);
    v3.uv = uv[2];
    v4.uv = uv[1];
    v5.uv = uv[3];
    v3.life = v4.life = v5.life = input[0].life;
    triStream.Append(v3);
    triStream.Append(v4);
    triStream.Append(v5);

    triStream.RestartStrip();
}
