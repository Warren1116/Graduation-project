#include "Particle.hlsli"

Texture2D particleTexture : register(t0);
SamplerState particleSampler : register(s0);


float4 main(PS_IN input) : SV_TARGET
{
    float4 texColor = particleTexture.Sample(particleSampler, input.uv);
    texColor.a *= input.life;
    return texColor;
    
    //float4 color = particleTexture.Sample(particleSampler, input.uv);
    //return float4(color.rgb, color.a * input.life);
}