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
	// �X�P�[���s����쐬
	DirectX::XMMATRIX s = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// �ʒu�s����쐬
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX w = s * r * t;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, w);
}


static Start* startInstance = nullptr;

// �C���X�^���X�擾
Start& Start::Instance()
{
	return *startInstance;
}


// �R���X�g���N�^
Start::Start(Enemy* enemy)
{
	// �C���X�^���X�|�C���g�ݒ�
	startInstance = this;
	EnemyManager& enemyManager = EnemyManager::Instance();
	Stage* stage = StageManager::Instance().GetStage(0);
	//position = StageManager::Instance().GetStage(0)->GetIndexWayPoint(StageManager::Instance().GetStage(0)->NearWayPointIndex(enemy->GetPosition()));
}

// �f�X�g���N�^
Start::~Start()
{
}

// �v���C���[�X�V����
void Start::Update(float elapsedTime, Enemy* enemy)
{
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	if (enemy->GetHealth() <= 0)
		position = StageManager::Instance().GetStage(0)->GetIndexWayPoint(StageManager::Instance().GetStage(0)->NearWayPointIndex(enemy->GetPosition()));
}

// �`��
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

// �C���X�^���X�擾
Goal& Goal::Instance()
{
	return *goalInstance;
}


// �R���X�g���N�^
Goal::Goal()
{
	// �C���X�^���X�|�C���g�ݒ�
	goalInstance = this;
	Stage* stage = StageManager::Instance().GetStage(0);
	position = stage->GetIndexWayPoint(stage->NearWayPointIndex(Player::Instance().GetPosition()));
}

// �f�X�g���N�^
Goal::~Goal()
{
}

void Goal::Update(float elapsedTime)
{
	UpdateTransform();
	Stage* stage = StageManager::Instance().GetStage(0);
	position = stage->GetIndexWayPoint(stage->NearWayPointIndex(Player::Instance().GetPosition()));
}

// �`��
void Goal::Render(const RenderContext& rc, ModelShader* shader)
{
	// �`��
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&transform, S * R * T);

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

// �R���X�g���N�^
Obstacle::Obstacle()
{
}

// �f�X�g���N�^
Obstacle::~Obstacle()
{
}

// �`��
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

// �R���X�g���N�^
Terrain::Terrain()
{

}

// �f�X�g���N�^
Terrain::~Terrain()
{
}

// �`��
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

// �R���X�g���N�^
Arrow::Arrow() :height(1.0f), radius(1.0f)
{
}

// �f�X�g���N�^
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
// �`��
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
