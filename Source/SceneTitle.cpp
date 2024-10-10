#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "Graphics\LightManager.h"
//#include "TutorialZombie.h"
//#include "TitleZombie.h"
#include "StageMain.h"

// ������
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	// �X�v���C�g������
	title1 = std::make_unique<Sprite>("Data/Sprite/Title.png");
	select = std::make_unique<Sprite>("Data/Sprite/select.png");
	scope = std::make_unique<Sprite>("Data/Sprite/Scope.png");

	// �J�����R���g���[���[������
	cameraController = std::make_unique<CameraController>();

	//// �G�l�~�[������
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//for (int i = 0; i < 2; ++i)
	//{
	//	TitleZombie* zombie = new TitleZombie();
	//	zombie->SetPosition(DirectX::XMFLOAT3(i * 3.0f, 0, 5.0f));
	//	enemyManager.Register(zombie);
	//}

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

	Enemy* enemy1 = EnemyManager::Instance().GetEnemy(0);
	Enemy* enemy2 = EnemyManager::Instance().GetEnemy(1);

	enemy1->SetAngle({ 0, DirectX::XMConvertToRadians(20), 0 });
	enemy2->SetAngle({ 0, DirectX::XMConvertToRadians(-30), 0 });

	Model* list[] =
	{
		enemy1->GetModel(),
		enemy2->GetModel(),

	};
	for (Model* model : list)
	{
		shadowmapRenderer->RegisterRenderModel(model);
		sceneRenderer->RegisterRenderModel(model);
		const ModelResource* resource = model->GetResource();
		for (const ModelResource::Material& material : resource->GetMaterials())
		{
			ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
			mat.shaderId = static_cast<int>(ModelShaderId::Phong);
		}
	}

	// �J���������ݒ�
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(1.6, 2.5, 9.0f),
		DirectX::XMFLOAT3(
			(enemy1->GetPosition().x + enemy2->GetPosition().x) / 2,
			enemy1->GetHeight() / 2,
			(enemy1->GetPosition().z + enemy2->GetPosition().z) / 2),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// ���s������ǉ�
	{
		Light* light = new Light(LightType::Directional);
		light->SetDirection({ 0, 1, -1 });
		light->SetColor({ 1,1,1,1 });
		light->SetPosition({ 0,0,0 });
		LightManager::Instance().Register(light);
	}

	StageMain& stage = StageMain::Instance();

	stage.SetStageNum(-1);

	fire = Audio::Instance().LoadAudioSource("Data/Audio/Fire.wav");
	title = Audio::Instance().LoadAudioSource("Data/Audio/title.wav");

	title->Play(true);
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

	Enemy* enemy1 = EnemyManager::Instance().GetEnemy(0);
	Enemy* enemy2 = EnemyManager::Instance().GetEnemy(1);

		// ���N���b�N�Ń��[�f�B���O�V�[��������ŃQ�[���V�[���֐؂�ւ�
	if (CollisionScopeVsEnemy(enemy1) && enemy1->GetHealth() > 0 && enemy2->GetHealth() > 0)
	{
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			enemy1->ApplyDamage(1000, 0.0f);
			fire->Play(false);
		}
	}

	if (!enemy1->GetModel()->IsPlayAnimation())
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}

	// ���N���b�N�ŏI���
	if (CollisionScopeVsEnemy(enemy2) && enemy1->GetHealth() > 0 && enemy2->GetHealth() > 0)
	{
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			enemy2->ApplyDamage(1000, 0.0f);
			fire->Play(false);
		}
	}

	if (!enemy2->GetModel()->IsPlayAnimation())
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
	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
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

	{
		
	}

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
		float textureWidth = static_cast<float>(title1->GetTextureWidth());
		float textureHeight = static_cast<float>(title1->GetTextureHeight());

		// �^�C�g���X�v���C�g�`��
		title1->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		select->Render(dc,
			-75, -200, screenWidth + 150, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

	}

	// 2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(scope->GetTextureWidth());
		float textureHeight = static_cast<float>(scope->GetTextureHeight());

		Mouse& mouse = Input::Instance().GetMouse();

		scope->Render(dc,
			mouse.GetPositionX() - textureWidth / 2, mouse.GetPositionY() - textureHeight / 2, textureWidth, textureHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);
	}

}

bool SceneTitle::CollisionScopeVsEnemy(Enemy* enemy)
{
	Mouse& mouse = Input::Instance().GetMouse();
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	RenderContext rc;
	rc.directionalLightData.direction = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// �r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();



	// �}�E�X�J�[�\�����W���擾
	DirectX::XMFLOAT3 screenPosition;
	screenPosition.x = static_cast<float>(mouse.GetPositionX());
	screenPosition.y = static_cast<float>(mouse.GetPositionY());
	screenPosition.z = 0.0f;

	// �X�N���[�����W���烏�[���h���W�֕ϊ�
	DirectX::XMVECTOR StartPosition = DirectX::XMVector3Unproject(
		DirectX::XMLoadFloat3(&screenPosition),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0.0f,
		1.0f,
		Projection,
		View,
		World);

	screenPosition.z = 1.0f;

	DirectX::XMVECTOR EndPosition = DirectX::XMVector3Unproject(
		DirectX::XMLoadFloat3(&screenPosition),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0.0f,
		1.0f,
		Projection,
		View,
		World);

	DirectX::XMFLOAT3 start;
	DirectX::XMStoreFloat3(&start, StartPosition);

	DirectX::XMFLOAT3 end;
	DirectX::XMStoreFloat3(&end, EndPosition);

	HitResult hit;
	return Collision::IntersectRayVsModel(start, end, enemy->GetModel(), hit);
}
