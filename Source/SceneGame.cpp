#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Input/Input.h"
//#include "Stage.h"
#include "StageManager.h"
#include "StageMain.h"
#include "StageMoveFloor.h"

// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	StageManager& stagemanager = StageManager::Instance();
	//StageMain* stageMain = new StageMain();
	stageMain = new StageMain();
	stagemanager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stagemanager.Register(stageMoveFloor);

	player = new Player();
	cameraController = new CameraController;
	gauge = new Sprite();


	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	//�G�l�~�[������
	for (int i = 0; i < 2; ++i)
	{
		EnemyManager& enemyManager = EnemyManager::Instance();
#if 1
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i*2.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);
#else
		for (int i = 0; i < 50; ++i)
		{
			EnemySlime* slime = new EnemySlime();
			slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
			enemyManager.Register(slime);
		}
#endif
	}

	//EnemyManager& enemyManager = EnemyManager::Instance();
	//EnemySlime* slime = new EnemySlime();
	//slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	//enemyManager.Register(slime);
	//EnemyManager::Instance().Register(new EnemySlime);

}

// �I����
void SceneGame::Finalize()
{
	//�X�e�[�W�I����
	StageManager::Instance().Clear();

	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}


	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�X�e�[�W�X�V
	StageManager::Instance().Update(elapsedTime);

	player->Update(elapsedTime);

	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;

	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//�G�l�~�[�X�V
	EnemyManager::Instance().Update(elapsedTime);

	//EffectManager�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�[�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//// �r���[�s��
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// �J�����̎��_�i�ʒu�j
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// �J�����̒����_�i�^�[�Q�b�g�j
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// �J�����̏����
	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// �v���W�F�N�V�����s��
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// ����p
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// ��ʏc���䗦
	//	float nearZ = 0.1f;	// �J�������f���o���̍ŋߋ���
	//	float farZ = 1000.0f;	// �J�������f���o���̍ŉ�����
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		player->Render(dc, shader);

		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc,shader);

		shader->End(dc);


	}

	//3D Effect�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();

		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
	{
		player->DrawDebugGUI();
	}
}

//�G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�S�Ă̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemy->GetPosition());
		//���[���h���W���炷��X�N���[�����W�ւ̕ϊ�
		DirectX::XMVECTOR HpPosition = DirectX::XMVector3Project(
			enemyPosition,							//���[���h���W
			viewport.TopLeftX, viewport.TopLeftY,	//�r���[�|�[�g���� X �� Y �̈ʒu
			viewport.Width, viewport.Height,		//�r���[�|�[�g�̕��ƍ���
			viewport.MinDepth, viewport.MaxDepth,	//�[�x�l�͈̔͂�\���ŏ��l�ƍő�l
			Projection, View, World					//�v���W�F�N�V�����s��A�r���[�s��A���[���h�s��
		);

		//DirectX::XMFLOAT3 hpPosition;
		//DirectX::XMStoreFloat3(&hpPosition, HpPosition);

		DirectX::XMFLOAT3 screenPosition;
		DirectX::XMStoreFloat3(&screenPosition, HpPosition);


		float gaugeWidth = enemy->GetMaxHealth() * enemy->GetHealth() * 2.0f;
		float gaugeHeight = 5.0f;

		// ������Z�l���g���ăJ�����O���s��
		if (screenPosition.z < 0 || screenPosition.z > 1) continue;
		// ��_�𑫌��ɂ��邱�Ƃŕ\�����Y��ɂ���
		gauge->Render(dc, screenPosition.x - gaugeWidth * 0.5f, screenPosition.y - gaugeHeight, gaugeWidth, gaugeHeight, 0.0f, 0.0f, gauge->GetTextureWidth(), gauge->GetTextureHeight(), 0, 1.0f, 0.0f, 0.0f, 1.0f);
	}
//�搶�̉�
#if 0 
		//const float gaugeWidth = 30.0f;
		//const float gaugeHeight = 5.0f;
		//float HealthRate = enemy->GetHeight() / static_cast<float>(enemy->GetMaxHealth());
		//gauge->Render(
		//	dc,
		//	hpPosition.x - gaugeWidth * 0.5f,
		//	hpPosition.y - gaugeHeight,
		//	gaugeWidth * HealthRate,
		//	gaugeHeight,
		//	0, 0,
		//	static_cast<float>(gauge->GetTextureWidth()),
		//	static_cast<float>(gauge->GetTextureHeight()),
		//	0.0f,
		//	1.0f, 0.0f, 0.0f, 1.0f);
#endif
		
	}

