#include "PostprocessingRenderer.h"
#include "Graphics/Graphics.h"
#include "Graphics/LightManager.h"
#include "Camera.h"
#include "Player.h"

PostprocessingRenderer::PostprocessingRenderer(UINT width, UINT height)
{
	Graphics& graphics = Graphics::Instance();

	renderSprite = std::make_unique<Sprite>();

	// 高輝度抽出用描画ターゲットを生成
	workRenderTarget[0].release();
	workRenderTarget[0] = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

	// 高輝度抽出ぼかし用描画ターゲットを生成
	workRenderTarget[1].release();
	workRenderTarget[1] = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

}

PostprocessingRenderer::~PostprocessingRenderer()
{
}

void PostprocessingRenderer::Render(ID3D11DeviceContext* dc)

{
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();

	// 現在設定されているバッファを退避
	CacheRenderTargets(dc);


	RenderContext rc;
	rc.deviceContext = dc;
	rc.screenSize.x = graphics.GetScreenWidth();
	rc.screenSize.y = graphics.GetScreenHeight();
	rc.screenSize.z = camera.GetNearZ();
	rc.screenSize.w = camera.GetFarZ();

	// カメラパラメータ設定
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// ライトの情報をセット
	LightManager::Instance().PushRenderContext(rc);

	//// シェーダー用情報の設定
	//	元のバッファに戻す
	RestoreRenderTargets(dc);

	// 最終パス
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Finalpass);
		shader->Begin(rc);

		renderSprite->SetShaderResourceView(sceneData.color, sceneData.width, sceneData.height);
		renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
			0,
			1, 1, 1, 1);

		//	シェーダーに渡す情報を設定
		rc.finalpassData.bloomTexture = workRenderTarget[1]->GetShaderResourceView().Get();
		rc.finalpassData.colorMap = sceneData.color.Get();
		rc.finalpassData.depthMap = sceneData.depth.Get();

		//ラジアルブラー
        rc.radialblurData = radialblurData;

		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}
}

//	RenderTargetのShaderResourceViewを取得
const SRVHandleList PostprocessingRenderer::GetRenderTargetShaderResourceViews() const
{
	return	SRVHandleList();
}

//	DepthStencilのShaderResourceViewを取得
const SRVHandle PostprocessingRenderer::GetDepthStencilShaderResourceView() const
{
	return	SRVHandle();
}

//	シーン情報の設定
void	PostprocessingRenderer::SetSceneData(ShaderResourceViewData srv)
{
	sceneData = srv;
}

//	デバッグ情報の表示
void	PostprocessingRenderer::DrawDebugGUI()
{
	if (ImGui::TreeNode("PostProcess"))
	{
        //ラジアルブラー
		if (ImGui::TreeNode("radial blur"))
		{
			ImGui::SliderFloat2("center", &radialblurData.blur_center.x, 0, 1);
			ImGui::SliderFloat("radius", &radialblurData.blur_radius, 0.001f, +100.0f);
			ImGui::SliderInt("sampling count", &radialblurData.blur_sampling_count, 1, 100);
			ImGui::SliderFloat("mask radius", &radialblurData.blur_mask_radius, 0, +500.0f);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

//　ラジアルブラーの有効無効
void PostprocessingRenderer::radialblurActive(bool active)
{
	if (active)
	{
		if (radialblurData.blur_radius < 50.0f)
		{
			radialblurData.blur_radius++;
		}
	}
	else
	{
        radialblurData.blur_radius = 0.0f;
	}
}

