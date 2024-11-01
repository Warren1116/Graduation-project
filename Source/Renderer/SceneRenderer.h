#pragma once

#include "Renderer/ModelRenderer.h"
#include "Graphics/geometric_primitive.h"

// シーンレンダラー
class SceneRenderer : public ModelRenderer
{
public:
	SceneRenderer(UINT width, UINT height);
	~SceneRenderer() {}

	// シャドウマップ情報をRenderContextに積む
	void SetShadowmapData(ShadowMapData shadowMapData) { this->shadowMapData = shadowMapData; }

	static SceneRenderer& Instance()
	{
		static SceneRenderer* instance;
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
	std::unique_ptr<RenderTarget>	renderTarget;		//	オフスクリーンレンダリング用描画バッファ
	std::unique_ptr<DepthStencil>	depthStencil;		//	オフスクリーンレンダリング用深度バッファ
	std::unique_ptr<Sprite>			skyboxRenderer;		//	空描画用スプライト
	ShadowMapData					shadowMapData;		//	シャドウマップ情報
	bool							drawDebugPrimitive;	//	デバッグプリミティブ描画
	int								timer;				//時間経過

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;
private:
	//	シャドウマップ用深度ステンシルバッファ
	std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
	//	ライトビュープロジェクション行列
	DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
	//	深度比較用のオフセット値
	float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
	//	影の色
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };


};
