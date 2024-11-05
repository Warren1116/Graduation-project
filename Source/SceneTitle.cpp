#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "Graphics\LightManager.h"
#include "StageMain.h"

// ������
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	// �J�����R���g���[���[������
	cameraController = std::make_unique<CameraController>();

	//	�e�탌���_���[����
	{
		UINT width = static_cast<UINT>(graphics.GetScreenWidth());
		UINT height = static_cast<UINT>(graphics.GetScreenHeight());

		//	�V���h�E�}�b�v�����_���[
		shadowmapRenderer = std::make_unique<ShadowmapRenderer>(2048);

		//	�V�[�������_���[
		sceneRenderer = std::make_unique<SceneRenderer>(width, height);

		//	�|�X�g�v���Z�X�����_���[
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>(width / 2, height / 2);
		const SRVHandleList	list = sceneRenderer->GetRenderTargetShaderResourceViews();
		SRVHandle	depth = sceneRenderer->GetDepthStencilShaderResourceView();
		postprocessingRenderer->SetSceneData({ list[0], depth, width, height });

	}

	// ���s������ǉ�
	{
		Light* light = new Light(LightType::Directional);
		light->SetDirection({ 0, 1, -1 });
		light->SetColor({ 1,1,1,1 });
		light->SetPosition({ 0,0,0 });
		LightManager::Instance().Register(light);
	}

	StageMain& stage = StageMain::Instance();

}

// �I����
void SceneTitle::Finalize()
{
	EnemyManager::Instance().Clear();

	LightManager::Instance().Clear();


}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
	ShowCursor(false);
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}

	if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
	{
		PostQuitMessage(1);
	}

	EnemyManager::Instance().Update(elapsedTime);
}

//�`�揈��
void SceneTitle::Render()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.2f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;
	LightManager::Instance().PushRenderContext(rc);

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3D
	{
		//ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		//shader->Begin(rc);
		//// �G�l�~�[�`��
		//EnemyManager::Instance().Render(rc, shader);
		//shader->End(rc);

		 //�V���h�E�}�b�v�̕`��
		shadowmapRenderer->Render(dc);

		//�V�[���̕`��
		sceneRenderer->SetShadowmapData(shadowmapRenderer->GetShadowMapData());
		sceneRenderer->Render(dc);
		postprocessingRenderer->Render(dc);

		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());


	}


}

