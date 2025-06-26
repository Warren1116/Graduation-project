struct Particle
{
    float3 position : POSITION;
    float life : LIFE;
};

struct VS_OUT
{
    float3 worldPos : POSITION;
    float life : LIFE;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float life : LIFE;
};

cbuffer CbScene : register(b0)
{
    row_major float4x4 view;
    row_major float4x4 viewProjection;
};