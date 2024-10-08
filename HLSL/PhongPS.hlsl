#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); //法線マップ
//Texture2D shadowMap : register(t2); //シャドウマップ

Texture2D shadowMap[ShadowmapCount] : register(t2); //シャドウマップ(配列にしているのでShadowmapCount分
//レジスターを利用していると考えてください。ShadowmapCount = 4　の場合はt2,t3,t4,t5まで使われています)

SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
    
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;
    
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

    float3 N = normalize(mul(normal * 2.0f - 1.0f, CM));
    
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);
    

    
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(1, 1, 1);
    float shiness = 128;
    
    
	// 環境光の計算
    float3 ambient = ka.rgb * ambientLightColor.rgb * diffuseColor.rgb;
    //float3 ambient = float()1;
	

	// 平行光源のライティング計算
    float3 diffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd.rgb);
    float3 specular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks.rgb);

	// スポットライトの処理
    for (int i = 0; i < spotLightCount; ++i)
    {
		// ライトベクトルを算出
        float3 lightVector = pin.world_position - spotLightData[i].position.xyz;

		// ライトベクトルの長さを算出
        float lightLength = length(lightVector);

        if (lightLength >= spotLightData[i].range)
            continue;

		// 距離減衰を算出する
        float attenuate = saturate(1.0f - lightLength / spotLightData[i].range);

        lightVector = normalize(lightVector);

		// 角度減衰を算出してattenuateに乗算する
        float3 spotDirection = normalize(spotLightData[i].direction.xyz);
        float angle = dot(spotDirection, lightVector);
        float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
        attenuate *= saturate(1.0f - (spotLightData[i].innerCorn - angle) / area);

        diffuse += CalcLambertDiffuse(N, lightVector,
									spotLightData[i].color.rgb, kd.rgb) * attenuate;
        specular += CalcPhongSpecular(N, lightVector,
									spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;

    }
    
    // スポットライトの処理
    for (i = 0; i < spotLightCount; ++i)
    {
		// ライトベクトルを算出
        float3 lightVector = pin.world_position - spotLightData[i].position.xyz;

		// ライトベクトルの長さを算出
        float lightLength = length(lightVector);

        if (lightLength >= spotLightData[i].range)
            continue;

		// 距離減衰を算出する
        float attenuate = saturate(1.0f - lightLength / spotLightData[i].range);

        lightVector = normalize(lightVector);

		// 角度減衰を算出してattenuateに乗算する
        float3 spotDirection = normalize(spotLightData[i].direction.xyz);
        float angle = dot(spotDirection, lightVector);
        float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
        attenuate *= saturate(1.0f - (spotLightData[i].innerCorn - angle) / area);

        diffuse += CalcLambertDiffuse(N, lightVector,
									spotLightData[i].color.rgb, kd.rgb) * attenuate;
        specular += CalcPhongSpecular(N, lightVector,
									spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;

    }
    
    return float4((diffuseColor.rgb * (ambient + diffuse) + specular), diffuseColor.a);
    
 //   //float3 N = normalize(pin.normal);
 //   // 法線マップからxyz成分を取得して( -1 ～ +1 )の間にスケーリング
 //   //float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

 //   // 変換用の3X3行列を用意する
	//// 注意点として、頂点シェーダー内で算出した単位ベクトル類はラスタライズの際に数値が頂点間で補間されます。
	//// そのため、ピクセルシェーダーのタイミングでは単位ベクトルではないものが入ってくる可能性があるので、
	//// 正規化を忘れないようにしてください。
 //   float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

 //   // 法線マップで取得した法線情報に変換行列を掛け合わせましょう。正規化は忘れずに
 //   //normalマップ持ってる情報は 0 ~ 1 しかない、使いたいなら -1 ~ 1 に変換
 //   //float3 N = normalize(mul(normal * 2.0f - 1.0f, CM));
    
 //   float3 L = normalize(directionalLightData.direction.xyz);
 //   float3 E = normalize(pin.world_position.xyz - viewPosition.xyz);
    
 //   //マテリアル定数
 //   float3 ka = float3(1, 1, 1); //環境反射率　
 //   float3 kd = float3(1, 1, 1); //拡散反射率
 //   float3 ks = float3(1, 1, 1); //鏡面反射率
 //   float shiness = 128;

 //   //環境光の計算
 //   //float3 ambient = ka * ambientLightColor.rgb;
 //   float3 ambient = (float3)1;

    
 //   // 平行光源のライティング計算
 //   //float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
 //   ////float3 directionalDiffuse = CalcHalfLambert(N, L, directionalLightData.color.rgb, kd);
 //   //float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

 //   //// 平行光源の影なので、平行光源に対して影を適応
 //   //float3 shadow = CalcShadowColor(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor,
 //   //shadowBias);
 //   //float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
    
 //   //float3 shadow = 1;
 //   //for (int j = 0; j < ShadowmapCount; ++j)
 //   //{
 //   //    float3 shadowTexcoord = pin.shadowTexcoord[j];
 //   //    //シャドウマップのUV範囲内か、深度値が範囲内か判定する
 //   //    if (shadowTexcoord.z >= 0 && shadowTexcoord.z <= 1 &&
 //   //        shadowTexcoord.x >= 0 && shadowTexcoord.x <= 1 &&
 //   //        shadowTexcoord.y >= 0 && shadowTexcoord.y <= 1)
 //   //    {
 //   //        //シャドウマップから深度値取得
 //   //        float depth = shadowMap[j].Sample(shadowMapSamplerState, shadowTexcoord.xy).r;
 //   //        //深度値を比較して影かどうかを判定する
 //   //        if (shadowTexcoord.z - depth > shadowBias[j])
 //   //            shadow = shadowColor;
 //   //        break;
 //   //    }
 //   //}
 //   //directionalDiffuse *= shadow;
 //   //directionalSpecular *= shadow;
    
 //   //点光源の処理
 //   float3 pointDiffuse = (float3) 0;
 //   float3 pointSpecular = (float3) 0;
 //   for (int i = 0; i < pointLightCount; ++i)
 //   {
 //       //ライトベクトルを算出
 //       float3 lightVector = pin.world_position.xyz - pointLightData[i].position.xyz;
        
 //       //ライトベクトルの長さを算出
 //       float lightLength = length(lightVector);
        
 //       //ライトの影響範囲外なら後の計算をしない
 //       if (lightLength >= pointLightData[i].range)
 //           continue;
        
 //       //距離減衰を算出する
 //       float attenuate = saturate(1.0f - lightLength / pointLightData[i].range) * pointLightData[i].power;
        
 //       lightVector = lightVector / lightLength;
 //       //pointDiffuse += CalcLambertDiffuse(N, lightVector,
	//							//	pointLightData[i].color.rgb, kd.rgb) * attenuate;
 //       //pointSpecular += CalcPhongSpecular(N, lightVector,
	//							//	pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;

        

 //   }
    
 //   //スポットライトの処理
 //   float3 spotDiffuse = (float) 0;
 //   float3 spotSpecular = (float) 0;
 //   for (i = 0; i < spotLightCount; ++i)
 //   {
 //       float3 lightVector = pin.world_position.xyz - spotLightData[i].position.xyz;
        
 //       float lightLength = length(lightVector);
        
 //       if (lightLength >= spotLightData[i].range)
 //           continue;
        
 //       //距離減衰を算出する
 //       float attenuate = saturate(1.0f - lightLength / spotLightData[i].range);
        
 //       lightVector = normalize(lightVector);
        
 //       //角度減衰を算出してattenuateに乗算する
 //       float3 spotDirection = normalize(spotLightData[i].direction.xyz);
 //       float angle = dot(spotDirection, lightVector);
 //       float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
 //       attenuate *= saturate(1.0f - (spotLightData[i].innerCorn - angle) / area) * spotLightData[i].power;
        
 //       //spotDiffuse += CalcLambertDiffuse(N, lightVector,
 //       //            spotLightData[i].color.rgb, kd.rgb) * attenuate;
 //       //spotSpecular += CalcPhongSpecular(N, lightVector,
 //       //            spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;

 //   }
    
    
    
 //   //float4 color = diffuseColor;
 //   //color.rgb *= ambient + directionalDiffuse;
 //   //color.rgb += directionalSpecular;
    
 //   //float4 color = float4(ambient, diffuseColor.a);
 //   //color.rgb += diffuseColor.rgb * directionalDiffuse;
 //   //color.rgb += directionalSpecular;
    
 //   //float4 color = float4(ambient, diffuseColor.a);
 //   //color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse);
 //   //color.rgb += directionalSpecular + pointSpecular;

 //   float4 color = float4(ambient, diffuseColor.a);
 //   //color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
 //   //color.rgb += directionalSpecular + pointSpecular + spotSpecular;
 //   //color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);
 //   return color;

}