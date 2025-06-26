#include "Particle.hlsli"

//struct VS_OUT
//{
//    float4 pos : SV_POSITION;
//    float life : LIFE;
//};

//VS_OUT main(Particle p)
//{
//    VS_OUT o;
//    o.pos = mul(float4(p.position, 1), viewProjection);
//    o.life = p.life;
//    return o;
//}

VS_OUT main(Particle p)
{
    VS_OUT o;
    o.worldPos = p.position;
    o.life = p.life;
    return o;
}