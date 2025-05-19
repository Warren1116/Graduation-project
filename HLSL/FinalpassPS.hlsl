#include "Finalpass.hlsli"



// シーンテクスチャ
Texture2D sceneTexture : register(t0);

// ブルームテクスチャ
Texture2D bloomTexture : register(t1);

// シーンサンプラステート
SamplerState sceneSampler : register(s0);

//
Texture2D<float> DepthMap : register(t2);


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
   
    return color;
    
}

