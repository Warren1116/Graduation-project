#include "PoisonZombie.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentDerived.h"
#include "ActionDerived.h"
#include "Collision.h"

static PoisonZombie* instance = nullptr;

// インスタンス取得
PoisonZombie& PoisonZombie::Instance()
{
	return *instance;
}



// コンストラクタ
PoisonZombie::PoisonZombie()
{
	health = 5;

	model = new Model("Data/Model/PoisonZombie/PoisonZombie.mdl");


	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;


	radius = 0.5f;
	height = 1.0f;

	// ビヘイビアツリー設定
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);

	// BehaviorTree図を基にBehaviorTreeを構築
	aiTree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
	aiTree->AddNode("Root", "Scout", 1, BehaviorTree::SelectRule::Priority, new WnderJudgment(this), nullptr);
	aiTree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Random, new BattleJudgment(this), nullptr);
	aiTree->AddNode("Root", "Die", 3, BehaviorTree::SelectRule::Non, new DieJudgment(this), new DieAction(this));

	//aiTree->AddNode("Scout", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
	aiTree->AddNode("Scout", "Wander", 0, BehaviorTree::SelectRule::Non, nullptr, new PursuitAction(this));

	aiTree->AddNode("Battle", "BattleMoveTree", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
	aiTree->AddNode("Battle", "GunMoveTree", 1, BehaviorTree::SelectRule::Non, nullptr,new GunActionMove(this));

	aiTree->AddNode("BattleMoveTree", "BattleMove", 0, BehaviorTree::SelectRule::Non, new HandAttackMoveJudgment(this), new NormalActionMove(this));
	aiTree->AddNode("BattleMoveTree", "HandAttack", 1, BehaviorTree::SelectRule::Non, new HandAttackJudgment(this), new NormalAction(this));

	SetPosition(DirectX::XMFLOAT3(5.0f, 0.0f, 25.0f));
	SetTerritory(GetPosition(), 10.0f);
	SetRandomTargetPosition();
	SetGunRandomTargetPosition();
}

// デストラクタ
PoisonZombie::~PoisonZombie()
{
	if (behaviorData != nullptr)
	{
		delete behaviorData;
		behaviorData = nullptr;
	}
	if (aiTree != nullptr)
	{
		delete aiTree;
		aiTree = nullptr;
	}
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void PoisonZombie::Update(float elapsedTime)
{
	if (!isAlive)
		Destroy();

	// 現在実行されているノードが無ければ
	if (activeNode == nullptr)
	{
		// 次に実行するノードを推論する。
		activeNode = aiTree->ActiveNodeInference(behaviorData);
	}
	// 現在実行するノードがあれば
	if (activeNode != nullptr)
	{
		// ビヘイビアツリーからノードを実行。
		activeNode = aiTree->Run(activeNode, behaviorData, elapsedTime);
	}

	// 速力処理更新
	UpdateVelocity(elapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

float PoisonZombie::PlayerToBoss()
{
	float length;
	DirectX::XMStoreFloat(&length,
		DirectX::XMVector3Length(DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&Player::Instance().GetPosition()))));

	return length;
}

// 死亡した時に呼ばれる
void PoisonZombie::OnDead()
{
	model->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Die), false);
	if (!model->IsPlayAnimation())
	{
		isAlive = false;
	}
}

//void PoisonZombie::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}


void PoisonZombie::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 縄張り範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	debugRenderer->DrawCylinder(territoryOrigin, attackRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// 索敵範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}



// 縄張り設定
void PoisonZombie::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

void PoisonZombie::GunSetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryGunRange = range;
}

// ターゲット位置をランダム設定
void PoisonZombie::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(attackRange, jumpAttackRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
	jumpPosition = { targetPosition.x,targetPosition.y,targetPosition.z };

}

void PoisonZombie::SetGunRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(jumpAttackRange, territoryGunRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}


// 目的地点へ移動
void PoisonZombie::MoveToTarget(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// 移動処理
	Move(vx, vz, speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void PoisonZombie::JumpMoveToTarget(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x-0.8f;
	float vz = targetPosition.z - position.z-0.8f;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// 移動処理
	position.x += vx;
	position.z += vz;
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool PoisonZombie::SearchPlayer()
{
	// プレイヤーとの高低差を考慮して3Dで距離判定をする
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		// 単位ベクトル化
		vx /= distXZ;
		vz /= distXZ;

		// 方向ベクトル化
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// デバッグエネミー情報表示
void PoisonZombie::DrawDebugGUI()
{
	std::string str = "";
	if (activeNode != nullptr)
	{
		str = activeNode->GetName();
	}
	//トランスフォーム
	if (ImGui::CollapsingHeader("PoisonZombie", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 位置
		ImGui::InputFloat3("Position", &position.x);
		// 回転
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// スケール
		ImGui::InputFloat3("Scale", &scale.x);

		ImGui::Text(u8"Behavior　%s", str.c_str());
	}
}

void PoisonZombie::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
	// ノード取得
	Model::Node* node = model->FindNode(nodeName);
	if (node != nullptr)
	{
		// ノード位置取得
		DirectX::XMFLOAT3 nodePosition;
		nodePosition.x = node->worldTransform._41;
		nodePosition.y = node->worldTransform._42;
		nodePosition.z = node->worldTransform._43;


		// プレイヤーとのあたり判定
		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;

		if (Collision::IntersectSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{
			// ダメージを与える
			if (player.ApplyDamage(1, 0.5f))
			{
				// 敵を吹っ飛ばすベクトルを算出
				DirectX::XMFLOAT3 vec;
				vec.x = outPosition.x - nodePosition.x;
				vec.z = outPosition.z - nodePosition.z;
				float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
				vec.x /= length;
				vec.z /= length;

				// XZ平面に吹っ飛ばす力をかける
				float power = 10.0f;
				vec.x *= power;
				vec.z *= power;
				// Y方向にも力をかける
				vec.y = 5.0f;
			}
		}
	}
}