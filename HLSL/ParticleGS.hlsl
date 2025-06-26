cbuffer CbScene : register(b0)
{
    matrix viewProjection;
};

struct VS_OUT
{
    float3 worldPos : POSITION;
    float life : LIFE;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float life : LIFE;
};

[maxvertexcount(3)]
void main(point VS_OUT input[1], inout TriangleStream<GS_OUT> triStream)
{
    GS_OUT o0 = { mul(float4(0.0f, 0.5f, 0.0f, 1.0f), viewProjection), float2(0, 0), 1 };
    GS_OUT o1 = { mul(float4(-0.5f, -0.5f, 0.0f, 1.0f), viewProjection), float2(0, 1), 1 };
    GS_OUT o2 = { mul(float4(0.5f, -0.5f, 0.0f, 1.0f), viewProjection), float2(1, 1), 1 };

    triStream.Append(o0);
    triStream.Append(o1);
    triStream.Append(o2);
    triStream.RestartStrip();
}
