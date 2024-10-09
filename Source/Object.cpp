#include <imgui.h>
#include "Object.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "StageManager.h"

void ObjectBase::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX s = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 位置行列を作成
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX w = s * r * t;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, w);
}


static Start* startInstance = nullptr;

// インスタンス取得
Start& Start::Instance()
{
	return *startInstance;
}


// コンストラクタ
Start::Start(Enemy* enemy)
{
	// インスタンスポイント設定
	startInstance = this;
	EnemyManager& enemyManager = EnemyManager::Instance();
	Stage* stage = StageManager::Instance().GetStage(0);
	//position = StageManager::Instance().GetStage(0)->GetIndexWayPoint(StageManager::Instance().GetStage(0)->NearWayPointIndex(enemy->GetPosition()));
}

// デストラクタ
Start::~Start()
{
}

// プレイヤー更新処理
void Start::Update(float elapsedTime, Enemy* enemy)
{
	// オブジェクト行列を更新
	UpdateTransform();

	if (enemy->GetHealth() <= 0)
		position = StageManager::Instance().GetStage(0)->GetIndexWayPoint(StageManager::Instance().GetStage(0)->NearWayPointIndex(enemy->GetPosition()));
}

// 描画
void Start::Render(const RenderContext& rc, ModelShader* shader)
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

static Goal* goalInstance = nullptr;

// インスタンス取得
Goal& Goal::Instance()
{
	return *goalInstance;
}


// コンストラクタ
Goal::Goal()
{
	// インスタンスポイント設定
	goalInstance = this;
	Stage* stage = StageManager::Instance().GetStage(0);
	position = stage->GetIndexWayPoint(stage->NearWayPointIndex(Player::Instance().GetPosition()));
}

// デストラクタ
Goal::~Goal()
{
}

void Goal::Update(float elapsedTime)
{
	UpdateTransform();
	Stage* stage = StageManager::Instance().GetStage(0);
	position = stage->GetIndexWayPoint(stage->NearWayPointIndex(Player::Instance().GetPosition()));
}

// 描画
void Goal::Render(const RenderContext& rc, ModelShader* shader)
{
	// 描画
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

// コンストラクタ
Obstacle::Obstacle()
{
}

// デストラクタ
Obstacle::~Obstacle()
{
}

// 描画
void Obstacle::Render(const RenderContext& rc, ModelShader* shader)
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

void ObstacleManager::Render(const RenderContext& rc, ModelShader* shader)
{
	for (auto const obstacle : obstacles)
	{
		obstacle->Render(rc, shader);
	}
}
void ObstacleManager::AddObstacle(Obstacle* terrain)
{
	obstacles.emplace_back(terrain);
}
void ObstacleManager::EraseObstacle(int index)
{
	int obstaclesIndex = 0;
	for (const auto obstacle : obstacles)
	{
		if (obstacle->posIndex == index) {
			delete obstacle;
			break;
		}
		++obstaclesIndex;
	}
	obstacles.erase(obstacles.begin() + obstaclesIndex);
}
int ObstacleManager::GetSearchObstacleIndex(int index)
{
	int wayIndex = -1;
	for (const auto& obstacle : obstacles)
	{
		if (obstacle->posIndex == index)wayIndex = index;
	}
	return wayIndex;
}

void ObstacleManager::Clear()
{
	for (Obstacle* obstacle : obstacles)
	{
		delete obstacle;
	}
	obstacles.clear();
}

// コンストラクタ
Terrain::Terrain()
{

}

// デストラクタ
Terrain::~Terrain()
{
}

// 描画
void Terrain::Render(const RenderContext& rc, ModelShader* shader)
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

void TerrainManager::Render(const RenderContext& rc, ModelShader* shader)
{
	for (auto const terrain : terrains)
	{
		terrain->Render(rc, shader);
	}
}

void TerrainManager::AddTerrain(Terrain* terrain)
{
	terrains.emplace_back(terrain);
}
void TerrainManager::EraseTerrain(int index)
{
	int terrainIndex = 0;
	for (const auto terrain : terrains)
	{
		if (terrain->posIndex == index) {
			delete terrain;
			break;
		}
		++terrainIndex;
	}
	terrains.erase(terrains.begin() + terrainIndex);
}
int TerrainManager::GetSearchTerrainIndex(int index)
{
	int wayIndex = -1;
	for (const auto& terrain : terrains)
	{
		if (terrain->posIndex == index)wayIndex = index;
	}
	return wayIndex;
}

void TerrainManager::Clear()
{
	for (Terrain* terrain : terrains)
	{
		delete terrain;
	}
	terrains.clear();
}

// コンストラクタ
Arrow::Arrow() :height(1.0f), radius(1.0f)
{
}

// デストラクタ
Arrow::~Arrow()
{
}
void Arrow::SetTransfrom(DirectX::XMFLOAT4X4 matrix)
{
	transform = matrix;
}
void Arrow::SetTransfrom(DirectX::XMMATRIX matrix)
{
	DirectX::XMStoreFloat4x4(&transform, matrix);
}
// 描画
void Arrow::Render(const RenderContext& rc, ModelShader* shader)
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

void ArrowManager::Render(const RenderContext& rc, ModelShader* shader)
{
	for (auto const arrow : arrows)
	{
		arrow->Render(rc, shader);
	}
}

void ArrowManager::AddArrow(Arrow* arrow)
{
	arrows.emplace_back(arrow);
}


void ArrowManager::Clear()
{
	for (Arrow* arrow : arrows)
	{
		delete arrow;
	}
	arrows.clear();
}
