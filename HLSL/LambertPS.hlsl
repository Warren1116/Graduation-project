
#include "Lambert.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

Texture2D<float> DepthMap : register(t1);

//  霧因子の計算
float ComputeFogFactor(float fogDensity, float fogDistance, float fogHeightFalloff, float groundLevel, float3 viewPosition)
{
    float verticalDistance = viewPosition.y - groundLevel;
  
    float fogFactor = 0.0;
  
    if (verticalDistance > 0)
    {
    //    fogFactor = saturate(1.0 - exp(-fogDensity * fogDistance));
    //    fogFactor *= exp(-verticalDistance * fogHeightFalloff);
    //    fogFactor *= saturate((fogCutoffDistance - fogDistance) / fogCutoffDistance);
        //  高さフォグ
        fogFactor = max(0.0f, 1.0f - exp(-fogDensity * verticalDistance));
        //  距離フォグ
        fogFactor *= 1 - saturate((fogCutoffDistance - fogDistance) / fogCutoffDistance);

    }
    return fogFactor;
}
//  色に霧効果を足す
float4 ApplyFog(float4 color, float3 fogColor, float fogFactor)
{
    float3 blendedColor = lerp(color.rgb, fogColor, fogFactor);
    
    return float4(blendedColor, color.a);
}

float4 CalcFog(in float4 color, float4 fog_color, float eye_length)
{
    float2 fog_range = float2(100, 500);
    float fogAlpha = saturate((eye_length - fog_range.x) / (fog_range.y - fog_range.x));
    return lerp(color, fog_color, fogAlpha);
}

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

    //float fogFactor = ComputeFogFactor(fogDensity, length(viewPosition), fogHeightFalloff, groundLevel, viewPosition.xyz);
    
    //  深度値を取る
    float depth = DepthMap.Sample(diffuseMapSamplerState, pin.texcoord).r;
    
    //  NDC座標と深度値からワールド座標に変換する
    float4 ndc = float4(pin.texcoord.xy, depth, 1);
    ndc.x = ndc.x * 2 - 1;
    ndc.y = ndc.y * -2 + 1;
    float4 pos = mul(ndc, mul(inverseProjection, inverseView));
    pos /= pos.w;
    //  カメラからピクセル座標への距離を求める
    depth = length(pos.xyz - viewPosition.xyz);
    
    float fogFactor = ComputeFogFactor(fogDensity, depth, fogHeightFalloff, groundLevel, viewPosition.xyz);

    diffuseColor = ApplyFog(diffuseColor, fogColor, fogFactor);

    
    return diffuseColor;
}

