cbuffer CbScene : register(b0)
{
    matrix viewProjection;
};

struct Particle
{
    float3 position : POSITION;
    float life : LIFE;
};

struct VS_OUT
{
    float3 worldPos : WORLDPOS;
    float life : LIFE;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float life : LIFE;
};