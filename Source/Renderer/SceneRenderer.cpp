#include "SceneRenderer.h"

#include "Graphics/LightManager.h"
#include "Camera.h"
#include "SearchAlgorithm.h"
#include "StageManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Input/Input.h"
#include "EffectManager.h"

SceneRenderer::SceneRenderer(UINT width, UINT height)
{
	renderTarget.release();
	depthStencil.release();

	renderTarget = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	depthStencil = std::make_unique<DepthStencil>(width, height);

	// ��`��p�X�v���C�g
	skyboxRenderer = std::make_unique<Sprite>();

	// ���C�g�f�o�b�O�t���O
	drawDebugPrimitive = false;


}

void SceneRenderer::Render(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	

	// ���ݐݒ肳��Ă���o�b�t�@��ޔ�
	CacheRenderTargets(dc);

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	dc->OMSetDepthStencilState(depthStencilState.Get(), 0);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = static_cast<float>(renderTarget->GetWidth());
	vp.Height = static_cast<float>(renderTarget->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);


	// �`��R���e�L�X�g�ɏ����Z�b�g
	RenderContext rc;
	{
		rc.deviceContext = dc;
		rc.timer = ++timer;
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

		// �V���h�E�}�b�v�̏����Z�b�g
		rc.shadowMapData = shadowMapData;

	}

	// ���`��
	const std::unique_ptr<Texture>& skyboxTexture = LightManager::Instance().GetSkyboxTexture();
	if (skyboxTexture)
	{
		// ��e�N�X�`����ݒ�
		skyboxRenderer->SetShaderResourceView(skyboxTexture->GetShaderResourceView().Get(),
			skyboxTexture->GetWidth(),
			skyboxTexture->GetHeight());
		skyboxRenderer->Update(0, 0, vp.Width, vp.Height,
			0, 0, static_cast<float>(skyboxTexture->GetWidth()), static_cast<float>(skyboxTexture->GetHeight()),
			0, 1, 1, 1, 1);
		// ��`��
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);
		shader->Draw(rc, skyboxRenderer.get());
		shader->End(rc);
	}

	// �o�^���f����`��
	ModelShaderId shaderId = static_cast<ModelShaderId>(0xFFFFFFFF);
	ModelShader* shader = nullptr;
	for (const Model* model : renderModels)
	{
		const ModelResource* resource = model->GetResource();
		const std::vector<Model::Node>& nodes = model->GetNodes();

		for (const ModelResource::Mesh& mesh : resource->GetMeshes())
		{
			bool updateBuffers = true;
			for (const ModelResource::Subset& subset : mesh.subsets)
			{
				//	�V�F�[�_�[�ݒ�
				ModelShaderId currentShaderId = static_cast<ModelShaderId>(subset.material->shaderId);
				if (shaderId != currentShaderId)
				{
					//	�V�F�[�_�[�ύX���̏I�������Ăяo��
					if (shader)
						shader->End(rc);

					//	�V�F�[�_�[�ύX
					shaderId = currentShaderId;
					if (currentShaderId == ModelShaderId::Max)
						continue;
					shader = graphics.GetShader(currentShaderId);

					//	�����_�[�X�e�[�g���̏���ݒ�
					shader->Begin(rc);
					updateBuffers = true;
				}

				//	���_�E�C���f�b�N�X�o�b�t�@���ݒ�
				if (updateBuffers)
				{
					shader->SetBuffers(rc, nodes, mesh);
					updateBuffers = false;
				}

				//	�T�u�Z�b�g�P�ʂŕ`��
				shader->DrawSubset(rc, subset);
			}
		}

		EffectManager::Instance().Render(rc.view, rc.projection);

	}


	//	�V�F�[�_�[������ΏI������
	if (shader)
		shader->End(rc);

	//	�f�o�b�O�v���~�e�B�u�̕\��
	if (drawDebugPrimitive)
	{
		// ���C�g�̃f�o�b�O�v���~�e�B�u�`��
		LightManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(rc.deviceContext, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(rc.deviceContext, rc.view, rc.projection);

		graphics.GetGeometricPrimitiver()->Render(rc.deviceContext, rc.view, rc.projection);
		//Stage* stage = StageManager::Instance().GetStage(0);
		//stage->DrawDebugPrimitive(dc, rc.view, rc.projection);
		
		//SearchAlgorithm::Instance().SearchRender(dc, rc.view, rc.projection, stage);

		Player::Instance().DrawDebugPrimitive();
		EnemyManager::Instance().DrawDebugPrimitive();
		
	}


	//	���̃o�b�t�@�ɖ߂�
	RestoreRenderTargets(dc);
}

//	�f�o�b�OGUI�`��
void SceneRenderer::DrawDebugGUI()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_P)
		drawDebugPrimitive = !drawDebugPrimitive;

	if (ImGui::TreeNode("Scene Rendering"))
	{
		ImGui::Checkbox("DebugPrimitive", &drawDebugPrimitive);

		if (ImGui::TreeNode("RenderTargets"))
		{
			ImGui::Text("Color");
			ImGui::Image(renderTarget->GetShaderResourceView().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Text("Depth");
			ImGui::Image(depthStencil->GetShaderResourceView().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

//	RenderTarget��ShaderResourceView���擾
const SRVHandleList SceneRenderer::GetRenderTargetShaderResourceViews() const
{
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	list;
	list.push_back(renderTarget->GetShaderResourceView());
	return	list;
}

//	DepthStencil��ShaderResourceView���擾
const SRVHandle SceneRenderer::GetDepthStencilShaderResourceView() const
{
	return	depthStencil->GetShaderResourceView();
}
