#pragma once

#include "Renderer/ModelRenderer.h"

// シャドウマップレンダラー
class ShadowmapCasterRenderer : public ModelRenderer
{
	static constexpr float Near = 1.0f;
	static constexpr float Far = 10000.0f;
public:
	ShadowmapCasterRenderer(UINT shadowmapSize);
	~ShadowmapCasterRenderer();

	// シャドウマップ情報をRenderContextに積む
	ShadowMapData GetShadowMapData();

	static ShadowmapCasterRenderer& Instance()
	{
		static ShadowmapCasterRenderer* instance;
		return *instance;
	}

public:
	//	描画実行
	void Render(ID3D11DeviceContext* dc) override;

	//	デバッグGUI描画
	void DrawDebugGUI() override;

	//	RenderTargetのShaderResourceViewを取得
	const SRVHandleList GetRenderTargetShaderResourceViews() const override;

	//	DepthStencilのShaderResourceViewを取得
	const SRVHandle GetDepthStencilShaderResourceView() const override;

private:
	//	シャドウマップで使用する各行列を算出
	void CalcShadowmapMatrix(DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection);


private:
	//std::unique_ptr<DepthStencil>	depthStencil;						// シャドウマップ用深度バッファ
	float							shadowRect = 500.0f;				// 描画範囲
	//DirectX::XMFLOAT3				shadowColor = { 0.2f, 0.2f, 0.2f };	// 影の色
	//float							shadowBias = 0.0001f;				// 深度比較用のオフセット値

	std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
	DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
	float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };


};

