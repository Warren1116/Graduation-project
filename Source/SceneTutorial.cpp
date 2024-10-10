//#include "SceneTutorial.h"
//#include "Graphics/Graphics.h"
//#include "Camera.h"
//#include "Graphics/Light.h"
//#include "Graphics/LightManager.h"
//#include "StageManager.h"
//#include "Input/Input.h"
//#include "EnemyManager.h"
//#include "EffectManager.h"
//#include "ItemManager.h"
//#include "TutorialZombie.h"
//#include "ItemKey.h"
//#include "ItemManager.h"
//
//void SceneTutorial::Initialize()
//{
//	player = std::make_unique<Player>(false);	
//	//gun = std::make_unique<Gun>();
//
//	// �J���������ݒ�
//	Graphics& graphics = Graphics::Instance();
//	Camera& camera = Camera::Instance();
//	camera.SetLookAt(
//		DirectX::XMFLOAT3(0, 10, -10),
//		DirectX::XMFLOAT3(0, 0, 0),
//		DirectX::XMFLOAT3(0, 1, 0)
//	);
//	camera.SetPerspectiveFov(
//		DirectX::XMConvertToRadians(45),
//		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
//		0.1f,
//		1000.0f
//	);
//
//	crossHair = std::make_unique<Sprite>();
//
//	// �J�����R���g���[���[������
//	cameraController = std::make_unique<CameraController>();
//
//	// ���s������ǉ�
//	{
//		Light* light = new Light(LightType::Directional);
//		light->SetDirection({ 1, -1, -1 });
//		light->SetColor({ 1,1,1,1 });
//		light->SetPosition({ 0,3,0 });
//		LightManager::Instance().Register(light);
//	}
//
//	// �X�|�b�g���C�g
//	static	const	int	Range = 500;
//	static	const	int	HalfRange = Range / 2;
//	for (int i = 0; i < SpotLightMax; ++i)
//	{
//		Light* light = new Light(LightType::Spot);
//		light->SetPosition({ 0,5,10 });
//		light->SetRange(10);
//		light->SetDirection({ 0,-1,0 });
//		light->SetColor({ 1,1,1,1 });
//		LightManager::Instance().Register(light);
//	}
//
//	TutorialZombie* zombie = new TutorialZombie;
//	zombie->SetPosition(DirectX::XMFLOAT3(10, 0, 10));
//	EnemyManager::Instance().Register(zombie);
//
//	ItemManager& itemManager = ItemManager::Instance();
//	ItemKey* item = new ItemKey();
//	item->SetPosition(DirectX::XMFLOAT3(6.0f, 0, 8.0f));
//	item->SetAngle({ DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90) });
//	itemManager.Register(item);
//
//	task = std::make_unique<Task>();
//}
//
//void SceneTutorial::Finalize()
//{
//	StageManager::Instance().Clear();
//
//	EnemyManager::Instance().Clear();
//
//	ItemManager::Instance().Clear();
//}
//
//void SceneTutorial::Update(float elapsedTime)
//{
//
//	Graphics& graphics = Graphics::Instance();
//	POINT point{ graphics.GetScreenWidth() / 2, graphics.GetScreenHeight() / 2 };
//	SetCursorPos((int)point.x, (int)point.y);
//
//	GamePad& gamePad = Input::Instance().GetGamePad();
//	ShowCursor(false);
//
//	// �J�����R���g���[���[�X�V����
//	Camera& camera = Camera::Instance();
//
//	// �X�e�[�W�X�V����
//	StageManager::Instance().Update(elapsedTime);
//
//	DirectX::XMVECTOR V = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&player->GetFront()), 27.08f);
//	DirectX::XMFLOAT3 v;
//	DirectX::XMStoreFloat3(&v, V);
//
//	DirectX::XMFLOAT3 target = { player->GetPosition().x + v.x, player->GetPosition().y + v.y, player->GetPosition().z + v.z };
//
//	target.y += player->GetHeight() - 0.35f;
//	cameraController->SetTarget(target);
//	cameraController->Update(elapsedTime);
//
//	// �v���C���[�X�V����
//	//player->SetAngle(cameraController->GetAngle());
//	player->Update(elapsedTime);
//
//	V = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&player->GetFront()), 27.08f);
//	DirectX::XMStoreFloat3(&v, V);
//
//	target = { player->GetPosition().x + v.x, player->GetPosition().y + v.y, player->GetPosition().z + v.z };
//
//	target.y += player->GetHeight() - 0.35f;
//	cameraController->SetTarget(target);
//	cameraController->Update(elapsedTime);
//
//	//// �e�X�V����
//	//gun->Update(elapsedTime);
//
//	// �G�l�~�[�X�V����
//	EnemyManager::Instance().Update(elapsedTime);
//
//	// �A�C�e���X�V����
//	ItemManager::Instance().Update(elapsedTime);
//
//	// �G�t�F�N�g�X�V����
//	EffectManager::Instance().Update(elapsedTime);
//
//	MoveTask();
//
//	if (task->contents[static_cast<size_t>(Task::Contents::Move)])
//	{
//		ShotTask();
//	}
//
//	if (task->contents[static_cast<size_t>(Task::Contents::Shot)])
//	{
//		PickupTask();
//	}
//}
//
//void SceneTutorial::Render()
//{
//	Graphics& graphics = Graphics::Instance();
//	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
//	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
//
//	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
//	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
//	dc->ClearRenderTargetView(rtv, color);
//	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//	dc->OMSetRenderTargets(1, &rtv, dsv);
//
//	// �`�揈��
//	RenderContext rc;
//	rc.deviceContext = dc;
//	LightManager::Instance().PushRenderContext(rc);
//
//	// �J�����p�����[�^�ݒ�
//	Camera& camera = Camera::Instance();
//	rc.view = camera.GetView();
//	rc.projection = camera.GetProjection();
//	rc.viewPosition.x = camera.GetEye().x;
//	rc.viewPosition.y = camera.GetEye().y;
//	rc.viewPosition.z = camera.GetEye().z;
//	rc.viewPosition.w = 1;
//
//	// 3D���f���`��
//	{
//		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
//		shader->Begin(rc);
//		
//		shader->End(rc);
//	}
//
//	// 3D�G�t�F�N�g�`��
//	{
//		// �G�t�F�N�g�`�揈��
//		EffectManager::Instance().Render(rc.view, rc.projection);
//	}
//
//	// 3D�f�o�b�O�`��
//	{
//		// �v���C���[�f�o�b�O�v���~�e�B�u�`��
//		player->DrawDebugPrimitive();
//
//		// �G�l�~�[�[�f�o�b�O�v���~�e�B�u�`��
//		EnemyManager::Instance().DrawDebugPrimitive();
//		ItemManager::Instance().DrawDebugPrimitive();
//
//		DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
//
//		
//
//		// ���C�������_���`����s
//		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
//
//		// �f�o�b�O�����_���`����s
//		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
//	}
//
//	// 2D�X�v���C�g�`��
//	{
//		crossHair->Render(dc,
//			graphics.GetScreenWidth() / 2.0f + 5.0f, graphics.GetScreenHeight() / 2.0f + 5.0f,
//			3.5, 3.5,
//			0, 0,
//			static_cast<float>(crossHair->GetTextureWidth()), static_cast<float>(crossHair->GetTextureHeight()),
//			0,
//			1, 1, 1, 1);
//	}
//
//	// 2D�f�o�b�OGUI�`��
//	{
//		//player->DrawDebugGUI();
//		
//	}
//}
//
//void SceneTutorial::MoveTask()
//{
//}
//
//void SceneTutorial::ShotTask()
//{
//}
//
//void SceneTutorial::PickupTask()
//{
//}
//
//void SceneTutorial::ChangeStage()
//{
//}
