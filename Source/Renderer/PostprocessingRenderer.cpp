#include "PostprocessingRenderer.h"
#include "Graphics/Graphics.h"
#include "Graphics/LightManager.h"
#include "Camera.h"

PostprocessingRenderer::PostprocessingRenderer(UINT width, UINT height)
{
	Graphics& graphics = Graphics::Instance();

	renderSprite = std::make_unique<Sprite>();

	// ���P�x���o�p�`��^�[�Q�b�g�𐶐�
	workRenderTarget[0].release();
	workRenderTarget[0] = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

	// ���P�x���o�ڂ����p�`��^�[�Q�b�g�𐶐�
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

	// ���ݐݒ肳��Ă���o�b�t�@��ޔ�
	CacheRenderTargets(dc);

	////���ݐݒ肳��Ă���o�b�t�@��ޔ����ď��������Ă���
	//UINT cachedViewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
	//D3D11_VIEWPORT cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	//Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cachedRenderTargetView;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cachedDepthStencilView;
	//{
	//	dc->RSGetViewports(&cachedViewportCount, cachedViewports);
	//	dc->OMGetRenderTargets(1,
	//		cachedRenderTargetView.ReleaseAndGetAddressOf(),
	//		cachedDepthStencilView.ReleaseAndGetAddressOf());
	//}

	RenderContext rc;
	rc.deviceContext = dc;
	rc.screenSize.x = graphics.GetScreenWidth();
	rc.screenSize.y = graphics.GetScreenHeight();
	rc.screenSize.z = camera.GetNearZ();
	rc.screenSize.w = camera.GetFarZ();

	// �J�����p�����[�^�ݒ�
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// ���C�g�̏����Z�b�g
	LightManager::Instance().PushRenderContext(rc);

	//// �V�F�[�_�[�p���̐ݒ�
	//	���̃o�b�t�@�ɖ߂�
	RestoreRenderTargets(dc);

	// �ŏI�p�X
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Finalpass);
		shader->Begin(rc);

		renderSprite->SetShaderResourceView(sceneData.color, sceneData.width, sceneData.height);
		renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
			0,
			1, 1, 1, 1);

		//	�V�F�[�_�[�ɓn������ݒ�
		rc.finalpassData.bloomTexture = workRenderTarget[1]->GetShaderResourceView().Get();
		rc.finalpassData.colorMap = sceneData.color.Get();
		rc.finalpassData.depthMap = sceneData.depth.Get();
		rc.colorGradingData = colorGradingData;

		shader->Draw(rc, renderSprite.get());

		shader->End(rc);
	}
}

//	RenderTarget��ShaderResourceView���擾
const SRVHandleList PostprocessingRenderer::GetRenderTargetShaderResourceViews() const
{
	return	SRVHandleList();
}

//	DepthStencil��ShaderResourceView���擾
const SRVHandle PostprocessingRenderer::GetDepthStencilShaderResourceView() const
{
	return	SRVHandle();
}

//	�V�[�����̐ݒ�
void	PostprocessingRenderer::SetSceneData(ShaderResourceViewData srv)
{
	sceneData = srv;
}

//	�f�o�b�O���̕\��
void	PostprocessingRenderer::DrawDebugGUI()
{
	if (ImGui::TreeNode("PostProcess"))
	{

		if (ImGui::TreeNode("ColorGrading"))
		{
			ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, 360.0f);
			ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, 2.0f);
			ImGui::SliderFloat("brightness", &colorGradingData.brightness, 0.0f, 2.0f);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
