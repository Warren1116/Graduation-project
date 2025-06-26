#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D shadowMap : register(t2);

SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 baseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
    float3 normalSample = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;
    float3x3 TBN = float3x3(normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal));
    float3 N = normalize(mul(normalSample * 2.0f - 1.0f, TBN));

    float3 L = normalize(-directionalLightData.direction.xyz);
    float3 V = normalize(viewPosition.xyz - pin.world_position.xyz);

    float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);

    float NoL = dot(N, L);
    float3 toonDiffuse = directionalLightData.color.rgb * (NoL > 0.5f ? 1.0f : 0.3f);

    // Specular
    float3 toonSpecular = 0;
    float3 H = normalize(L + V);
    float NoH = dot(N, H);
    toonSpecular = step(0.98f, NoH) * directionalLightData.color.rgb * 0.5f;
    if (N.y > 0.9f)
    {
        toonSpecular = 0; // 地面除外
    }

    // Rim
    float3 rimColor = 0;
    float rim = 1.0f - saturate(dot(V, N));
    rim = pow(rim, 2.5f);
    if (abs(N.y) < 0.6f)
    {
        rimColor = rim * float3(0.7f, 0.7f, 0.7f);
    }

    // Ambient
    float3 ambient = ambientLightColor.rgb * 0.5f;

    // 合成
    float3 lit = ambient + (toonDiffuse + toonSpecular) * shadow;
    float3 finalColor = baseColor.rgb * lit + rimColor;

    // 地面抑光（N.y 近 1）
    if (N.y > 0.95f)
    {
        finalColor *= 0.9f;
    }

    return float4(finalColor, baseColor.a);
}
