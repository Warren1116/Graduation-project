#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//	平行光源情報
struct DirectionalLightData
{
    DirectX::XMFLOAT4	direction;	// 向き
    DirectX::XMFLOAT4	color;		// 色
};

// ラジアルブラー情報
struct RadialBlurData
{
    float blur_radius = 0.0f;
    int blur_sampling_count = 10;
    DirectX::XMFLOAT2 blur_center = { 0.5f,0.5f };
    float blur_mask_radius = 500;		//	centerからの指定の範囲はブラーを適応しないようにする
    DirectX::XMFLOAT3  blur_dummy;
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
    ID3D11ShaderResourceView* shadowMap;		//シャドウマップテクスチャ
    DirectX::XMFLOAT4X4 lightViewProjection;	//ライトビュープロジェクション行列
    DirectX::XMFLOAT3 shadowColor;				//影の色
    float shadowBias;							//深度比較用のオフセット値
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

    //	最終パス情報
    FinalpassData				finalpassData;

    //	シャドウマップ情報
    ShadowMapData				shadowMapData;

    //	スカイボックス情報
    SkyboxData					skyboxData;

    //ラジアルブラー情報
    RadialBlurData            radialblurData;

};
