#include "Finalpass.hlsli"



// シーンテクスチャ
Texture2D sceneTexture : register(t0);

// ブルームテクスチャ
Texture2D bloomTexture : register(t1);

// シーンサンプラステート
SamplerState sceneSampler : register(s0);

//
Texture2D<float> DepthMap : register(t2);

float ComputeFogFactor(float fogDensity, float fogDistance, float fogHeightFalloff, float groundLevel, float3 viewPosition)
{
    float verticalDistance = viewPosition.y - groundLevel;
  
    float fogFactor = 0.0;
  
    if (verticalDistance > 0)
    {
        //  高さフォグ
        fogFactor = max(0.0f, 1.0f - exp(-fogDensity * verticalDistance));
        //  距離フォグ
        fogFactor *= 1 - saturate((fogCutoffDistance - fogDistance) / fogCutoffDistance);

    }
    return fogFactor;
}



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
    float2 scene_map_size;
    sceneTexture.GetDimensions(scene_map_size.x, scene_map_size.y);
    
    float4 color = sceneTexture.Sample(sceneSampler, pin.texcoord);
    float4 result_color = color;
    
    float2 blur_vector = (blur_center - pin.texcoord);
    blur_vector *= (blur_radius / scene_map_size.xy) / blur_sampling_count;
    for (int index = 1; index < blur_sampling_count; ++index)
    {
        result_color += sceneTexture.Sample(sceneSampler, pin.texcoord + blur_vector * index);
    }
    
      //指定の範囲内は適応量を変える
    float mask_radius = blur_mask_radius / min(scene_map_size.x, scene_map_size.y);
    float mask_value = saturate(length(pin.texcoord - blur_center) / mask_radius);
    color = lerp(color, result_color / blur_sampling_count, mask_value);

     //ブルームテクスチャを加算する
    color.rgb += bloomTexture.Sample(sceneSampler, pin.texcoord).rgb;

      //深度値を取る
    float depth = DepthMap.Sample(sceneSampler, pin.texcoord).r;
    
      //NDC座標と深度値からワールド座標に変換する
    float4 ndc = float4(pin.texcoord.xy, depth, 1);
    ndc.x = ndc.x * 2 - 1;
    ndc.y = ndc.y * -2 + 1;
    float4 pos = mul(ndc, mul(inverseProjection, inverseView));
    pos /= pos.w;
      //カメラからピクセル座標への距離を求める
    depth = length(pos.xyz - viewPosition.xyz);
    
     //Compute fog factor
    float fogFactor = ComputeFogFactor(fogDensity, depth, fogHeightFalloff, groundLevel, viewPosition.xyz);

     //Apply fog to color
    color = ApplyFog(color, fogColor, fogFactor);
        		
     //色調補正処理
    {
         //RGB > HSVに変換
        color.rgb = RGB2HSV(color.rgb);

         //色相調整
        color.r += hueShift;

         //彩度調整
        color.g *= saturation;

         //明度調整
        color.b *= brightness;

         //HSV > RGBに変換
        color.rgb = HSV2RGB(color.rgb);
    }

    return color;
    
    
}

