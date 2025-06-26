#include "Particle.hlsli"


[maxvertexcount(6)]
void main(point VS_OUT input[1], inout TriangleStream<PS_IN> triStream)
{
    float3 pos = input[0].worldPos;
    float size = 2.5f;
    //float size = lerp(0.2f, 1.0f, input[0].life);
    
    float3 right = float3(view._11, view._21, view._31);
    float3 up = float3(view._12, view._22, view._32);

    float3 offsets[4] =
    {
        -right + up,
         right + up,
        -right - up,
         right - up
    };

    float2 uvs[4] =
    {
        float2(0, 0),
        float2(1, 0),
        float2(0, 1),
        float2(1, 1)
    };

    PS_IN v0 = { mul(float4(pos + offsets[0] * size, 1), viewProjection), uvs[0], input[0].life };
    PS_IN v1 = { mul(float4(pos + offsets[1] * size, 1), viewProjection), uvs[1], input[0].life };
    PS_IN v2 = { mul(float4(pos + offsets[2] * size, 1), viewProjection), uvs[2], input[0].life };
    PS_IN v3 = { mul(float4(pos + offsets[3] * size, 1), viewProjection), uvs[3], input[0].life };

    triStream.Append(v0);
    triStream.Append(v1);
    triStream.Append(v2);
    triStream.Append(v2);
    triStream.Append(v1);
    triStream.Append(v3);
    triStream.RestartStrip();
}
