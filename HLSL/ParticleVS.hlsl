#include "Particle.hlsli"


VS_OUT main(Particle p)
{
    VS_OUT o;
    //o.worldPos = p.position;
    o.worldPos = mul(float4(p.position, 1.0f), view);
    o.life = p.life;
     
    return o;
}