#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); //法線マップ
Texture2D shadowMap : register(t2); //シャドウマップ

//Texture2D shadowMap[ShadowmapCount] : register(t2); //シャドウマップ(配列にしているのでShadowmapCount分
//レジスターを利用していると考えてください。ShadowmapCount = 4　の場合はt2,t3,t4,t5まで使われています)

SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
    
    // 法線マップからxyz成分を取得して( -1 ～ +1 )の間にスケーリング
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

    // 変換用の3X3行列を用意する
	// 注意点として、頂点シェーダー内で算出した単位ベクトル類はラスタライズの際に数値が頂点間で補間されます。
	// そのため、ピクセルシェーダーのタイミングでは単位ベクトルではないものが入ってくる可能性があるので、
	// 正規化を忘れないようにしてください。
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

    // 法線マップで取得した法線情報に変換行列を掛け合わせましょう。正規化は忘れずに
    //normalマップ持ってる情報は 0 ~ 1 しかない、使いたいなら -1 ~ 1 に変換
    float3 N = normalize(mul(normal * 2.0f - 1.0f, CM));
    
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);
    
    //マテリアル定数
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(0, 0, 0);
    float shiness = 128;
    
    
	// 環境光の計算
    float3 ambient = ka.rgb * ambientLightColor.rgb/* * diffuseColor.rgb*/;	

    float3 diffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    float3 specular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

    // 平行光源の影なので、平行光源に対して影を適応
    float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
    
    diffuse *= shadow;
    specular *= shadow;
            
    return float4((diffuseColor.rgb * (ambient + diffuse) + specular), diffuseColor.a);
    
}