#pragma once

#include "Renderer/BaseRenderer.h"

// シェーダーリソースビューの情報
struct ShaderResourceViewData
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	color;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	depth;
	UINT	width = 0;
	UINT	height = 0;
};

// ポストプロセス用のレンダラー
class PostprocessingRenderer : public BaseRenderer
{
public:
	PostprocessingRenderer(UINT width, UINT height);
	~PostprocessingRenderer();

	//	描画
	void PostprocessingRenderer::Render(ID3D11DeviceContext* dc)override;

	// RenderTargetのShaderResourceViewを取得
	const SRVHandleList GetRenderTargetShaderResourceViews() const override;

	// DepthStencilのShaderResourceViewを取得
	const SRVHandle GetDepthStencilShaderResourceView() const override;

	//	シーン情報の設定
	void	SetSceneData(ShaderResourceViewData srv);

	//	デバッグ情報の表示
	void	DrawDebugGUI();

    void	radialblurActive(bool active);


private:
	// シーン描画データ
	ShaderResourceViewData			sceneData;

	// 描画用スプライト
	std::unique_ptr<Sprite>			renderSprite;

	// 色調補正データ
	ColorGradingData				colorGradingData;

	//ラジアルブラー
	RadialBlurData					radialblurData;

	//	高輝度抽出テクスチャ
	std::unique_ptr<RenderTarget>	workRenderTarget[2];

};

