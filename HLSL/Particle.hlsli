struct Particle
{
    float3 position : POSITION;
    float life : LIFE;
};

cbuffer CbScene : register(b0)
{
    matrix viewProjection;
};
