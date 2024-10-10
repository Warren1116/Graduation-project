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
//	// カメラ初期設定
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
//	// カメラコントローラー初期化
//	cameraController = std::make_unique<CameraController>();
//
//	// 平行光源を追加
//	{
//		Light* light = new Light(LightType::Directional);
//		light->SetDirection({ 1, -1, -1 });
//		light->SetColor({ 1,1,1,1 });
//		light->SetPosition({ 0,3,0 });
//		LightManager::Instance().Register(light);
//	}
//
//	// スポットライト
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
//	// カメラコントローラー更新処理
//	Camera& camera = Camera::Instance();
//
//	// ステージ更新処理
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
//	// プレイヤー更新処理
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
//	//// 銃更新処理
//	//gun->Update(elapsedTime);
//
//	// エネミー更新処理
//	EnemyManager::Instance().Update(elapsedTime);
//
//	// アイテム更新処理
//	ItemManager::Instance().Update(elapsedTime);
//
//	// エフェクト更新処理
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
//	// 画面クリア＆レンダーターゲット設定
//	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
//	dc->ClearRenderTargetView(rtv, color);
//	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//	dc->OMSetRenderTargets(1, &rtv, dsv);
//
//	// 描画処理
//	RenderContext rc;
//	rc.deviceContext = dc;
//	LightManager::Instance().PushRenderContext(rc);
//
//	// カメラパラメータ設定
//	Camera& camera = Camera::Instance();
//	rc.view = camera.GetView();
//	rc.projection = camera.GetProjection();
//	rc.viewPosition.x = camera.GetEye().x;
//	rc.viewPosition.y = camera.GetEye().y;
//	rc.viewPosition.z = camera.GetEye().z;
//	rc.viewPosition.w = 1;
//
//	// 3Dモデル描画
//	{
//		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
//		shader->Begin(rc);
//		
//		shader->End(rc);
//	}
//
//	// 3Dエフェクト描画
//	{
//		// エフェクト描画処理
//		EffectManager::Instance().Render(rc.view, rc.projection);
//	}
//
//	// 3Dデバッグ描画
//	{
//		// プレイヤーデバッグプリミティブ描画
//		player->DrawDebugPrimitive();
//
//		// エネミーーデバッグプリミティブ描画
//		EnemyManager::Instance().DrawDebugPrimitive();
//		ItemManager::Instance().DrawDebugPrimitive();
//
//		DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
//
//		
//
//		// ラインレンダラ描画実行
//		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
//
//		// デバッグレンダラ描画実行
//		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
//	}
//
//	// 2Dスプライト描画
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
//	// 2DデバッグGUI描画
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
