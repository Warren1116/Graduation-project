#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//	平行光源情報
struct DirectionalLightData
{
    DirectX::XMFLOAT4	direction;	// 向き
    DirectX::XMFLOAT4	color;		// 色
};

// 点光源情報
struct PointLightData
{
    DirectX::XMFLOAT4	position;	// 座標
    DirectX::XMFLOAT4	color;		// 色
    float				range;		// 範囲
    float				power;
    DirectX::XMFLOAT2	dummy;
};

// 点光源の最大数
static	constexpr	int	PointLightMax = 8;

// スポットライト情報
struct SpotLightData
{
    DirectX::XMFLOAT4	position;	// 座標
    DirectX::XMFLOAT4	direction;	// 向き
    DirectX::XMFLOAT4	color;		// 色
    float				range;		// 範囲
    float				power;
    float				innerCorn; 	// インナー角度範囲
    float				outerCorn; 	// アウター角度範囲
};

// スポットライトの最大数
static	constexpr	int	SpotLightMax = 8;

// 色調補正情報
struct ColorGradingData
{
    float	hueShift = 0;	// 色相調整
    float	saturation = 1;	// 彩度調整
    float	brightness = 1;	// 明度調整
    float	dummy;
};


// ガウスフィルターの最大カーネルサイズ
static const int MaxKernelSize = 16;

struct FogData
{

    DirectX::XMFLOAT3 fogColor = { 1.0f,1.0f,1.0f };
    //float fogIntensity = 0.02f;
    float fogDensity = 0.02f;
    float fogHeightFalloff = 0.05f;
    float fogCutoffDistance = 500.0f;
    float groundLevel = 0.0f;
    //float mieScatteringCoef;
    //float timeScale = 0.35f;
    //float noiseScale = 0.2f;

};


// ポストエフェクトの最終パス用情報
struct FinalpassData
{
    //	ブルームテクスチャ
    ID3D11ShaderResourceView* bloomTexture;
    //	カラーテクスチャ
    ID3D11ShaderResourceView* colorMap;
    //	深度テクスチャ
    ID3D11ShaderResourceView* depthMap;

};

//シャドウマップの情報
static const int ShadowmapCount = 4;

//シャドウマップ用情報
struct ShadowMapData
{
    //ID3D11ShaderResourceView* shadowMap;		//シャドウマップテクスチャ
    //DirectX::XMFLOAT4X4 lightViewProjection;	//ライトビュープロジェクション行列
    //DirectX::XMFLOAT3 shadowColor;				//影の色
    //float shadowBias;							//深度比較用のオフセット値


    ID3D11ShaderResourceView* shadowMap[ShadowmapCount];				//	シャドウマップテクスチャ
    DirectX::XMFLOAT4X4			lightViewProjection[ShadowmapCount];	//	ライトビュープロジェクション行列
    DirectX::XMFLOAT3			shadowColor;			//	影の色
    float						shadowBias[ShadowmapCount];				//	深度比較用のオフセット値
};

struct SkyboxData
{
    ID3D11ShaderResourceView* skybox;					//	空テクスチャ
};


// レンダーコンテキスト
struct RenderContext
{
    ID3D11DeviceContext* deviceContext;

    float						timer;		//	タイマー

    DirectX::XMFLOAT4			screenSize;	//	xy : スクリーンのサイズ, z : nearクリップ距離、w : farクリップ距離

    //	カメラ情報
    DirectX::XMFLOAT4			viewPosition;
    DirectX::XMFLOAT4X4			view;
    DirectX::XMFLOAT4X4			projection;

    //	ライト情報
    DirectX::XMFLOAT4			ambientLightColor;				// 環境光情報
    DirectionalLightData		directionalLightData;			// 平行光源情報
    SpotLightData				spotLightData[SpotLightMax];	// スポットライト情報
    int							spotLightCount = 0;				// スポットライト数

    //	色調補正情報
    ColorGradingData			colorGradingData;

    //	最終パス情報
    FinalpassData				finalpassData;

    //	シャドウマップ情報
    ShadowMapData				shadowMapData;

    //	スカイボックス情報
    SkyboxData					skyboxData;


};
