#include "Particle.hlsli"

Texture2D particleTexture : register(t0);
SamplerState particleSampler : register(s0);


float4 main(PS_IN input) : SV_TARGET
{
    float4 texColor = particleTexture.Sample(particleSampler, input.uv);
    float alpha = saturate(input.life);
    texColor.rgb *= texColor.a;
    texColor.rgb *= alpha;
    texColor.a *= alpha;   
    return texColor;
    
   
}