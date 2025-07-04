#pragma once

#include "Renderer/ModelRenderer.h"

// シャドウマップレンダラー
class ShadowmapRenderer : public ModelRenderer
{
	static constexpr float Near = 1.0f;
	static constexpr float Far = 10000.0f;
public:
	ShadowmapRenderer(UINT shadowmapSize);
	~ShadowmapRenderer();

	// シャドウマップ情報をRenderContextに積む
	ShadowMapData GetShadowMapData();

	static ShadowmapRenderer& Instance()
	{
		static ShadowmapRenderer* instance;
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
	std::unique_ptr<DepthStencil>	shadowmapDepthStencil;						// シャドウマップ用深度バッファ
	float							shadowRect = 100.0f;				// 描画範囲
	DirectX::XMFLOAT3				shadowColor = { 0.2f, 0.2f, 0.2f };	// 影の色
	float							shadowBias = 0.00001f;				// 深度比較用のオフセット値

};

