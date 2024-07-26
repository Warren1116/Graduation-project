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

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
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


	//カメラ初期設定
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

	//エネミー初期化
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

// 終了化
void SceneGame::Finalize()
{
	//ステージ終了化
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


	//エネミー終了化
	EnemyManager::Instance().Clear();
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//ステージ更新
	StageManager::Instance().Update(elapsedTime);

	player->Update(elapsedTime);

	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;

	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//エネミー更新
	EnemyManager::Instance().Update(elapsedTime);

	//EffectManager更新処理
	EffectManager::Instance().Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメーター設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//// ビュー行列
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// カメラの視点（位置）
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// カメラの注視点（ターゲット）
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// カメラの上方向
	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// プロジェクション行列
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// 視野角
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// 画面縦横比率
	//	float nearZ = 0.1f;	// カメラが映し出すの最近距離
	//	float farZ = 1000.0f;	// カメラが映し出すの最遠距離
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		player->Render(dc, shader);

		//エネミー描画
		EnemyManager::Instance().Render(dc,shader);

		shader->End(dc);


	}

	//3D Effect描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2DデバッグGUI描画
	{
		player->DrawDebugGUI();
	}
}

//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemy->GetPosition());
		//ワールド座標からするスクリーン座標への変換
		DirectX::XMVECTOR HpPosition = DirectX::XMVector3Project(
			enemyPosition,							//ワールド座標
			viewport.TopLeftX, viewport.TopLeftY,	//ビューポート左上 X と Y の位置
			viewport.Width, viewport.Height,		//ビューポートの幅と高さ
			viewport.MinDepth, viewport.MaxDepth,	//深度値の範囲を表す最小値と最大値
			Projection, View, World					//プロジェクション行列、ビュー行列、ワールド行列
		);

		//DirectX::XMFLOAT3 hpPosition;
		//DirectX::XMStoreFloat3(&hpPosition, HpPosition);

		DirectX::XMFLOAT3 screenPosition;
		DirectX::XMStoreFloat3(&screenPosition, HpPosition);


		float gaugeWidth = enemy->GetMaxHealth() * enemy->GetHealth() * 2.0f;
		float gaugeHeight = 5.0f;

		// ここでZ値を使ってカリングを行う
		if (screenPosition.z < 0 || screenPosition.z > 1) continue;
		// 基準点を足元にすることで表示を綺麗にする
		gauge->Render(dc, screenPosition.x - gaugeWidth * 0.5f, screenPosition.y - gaugeHeight, gaugeWidth, gaugeHeight, 0.0f, 0.0f, gauge->GetTextureWidth(), gauge->GetTextureHeight(), 0, 1.0f, 0.0f, 0.0f, 1.0f);
	}
//先生の解答
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

