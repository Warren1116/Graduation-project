#include "Particle.hlsli"


VS_OUT main(Particle p)
{
    VS_OUT o;
    o.worldPos = p.position;
    o.life = p.life;
    return o;
}
