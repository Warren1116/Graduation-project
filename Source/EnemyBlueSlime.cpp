//#include "Graphics/Graphics.h"
//#include "Mathf.h"
//#include "EnemyBlueSlime.h"
//#include "Player.h"
//#include "ProjectileStraight.h"
//#include "BehaviorTree.h"
//#include "BehaviorData.h"
//#include "NodeBase.h"
//#include "JudgmentDerived.h"
//#include "ActionDerived.h"
//
//// コンストラクタ
//EnemyBlueSlime::EnemyBlueSlime()
//{
//	model = new Model("Data/Model/BlueSlime/BlueSlime.mdl");
//
//	// モデルが大きいのでスケーリング
//	scale.x = scale.y = scale.z = 0.01f;
//
//	radius = 0.5f;
//	height = 1.0f;
//
//	// ビヘイビアツリー設定
//	behaviorData = new BehaviorData();
//	aiTree = new BehaviorTree(this);
//	// BehaviorTree図を基にBehaviorTreeを構築
//	aiTree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
//	aiTree->AddNode("Root", "Escape", 0, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this), nullptr);
//	aiTree->AddNode("Root", "Battle", 1, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
//	aiTree->AddNode("Root", "Scout", 2, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
//
//	aiTree->AddNode("Escape", "Leave", 0, BehaviorTree::SelectRule::Non, nullptr,new LeaveAction(this));
//	aiTree->AddNode("Escape", "Recover", 1, BehaviorTree::SelectRule::Non, nullptr, new RecoverAction(this));
//
//	aiTree->AddNode("Battle", "Attact", 0, BehaviorTree::SelectRule::Random, new AttackJudgment(this), nullptr);
//	aiTree->AddNode("Battle", "Pursiut", 1, BehaviorTree::SelectRule::Non, nullptr, new PursuitAction(this));
//
//	aiTree->AddNode("Attact", "Normal", 0, BehaviorTree::SelectRule::Non, nullptr, new AttackAction(this));
//	aiTree->AddNode("Attact", "Skill", 1, BehaviorTree::SelectRule::Non, new SkillShotJudgment(this), new SkillAction(this));
//
//	aiTree->AddNode("Scout", "Wander",0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
//	aiTree->AddNode("Scout", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
//
//	
//
//	SetPosition(DirectX::XMFLOAT3(5.0f, 0.0f, 25.0f));
//	SetTerritory(GetPosition(), 10.0f);
//	SetRandomTargetPosition();
//}
//
//// デストラクタ
//EnemyBlueSlime::~EnemyBlueSlime()
//{
//	delete model;
//	delete aiTree;
//	delete behaviorData;
//}
//
//void EnemyBlueSlime::Update(float elapsedTime)
//{
//	// 現在実行されているノードが無ければ
//	if (activeNode == nullptr)
//	{
//		// 次に実行するノードを推論する。
//		activeNode = aiTree->ActiveNodeInference(behaviorData);
//	}
//	// 現在実行するノードがあれば
//	if (activeNode != nullptr)
//	{
//		// ビヘイビアツリーからノードを実行。
//		activeNode = aiTree->Run(activeNode, behaviorData, elapsedTime);
//	}
//
//
//	// 速力処理更新
//	UpdateVelocity(elapsedTime);
//
//	// 無敵時間更新
//	UpdateInvincibleTimer(elapsedTime);
//
//	// オブジェクト行列更新
//	UpdateTransform();
//
//	model->UpdateAnimation(elapsedTime);
//
//	// モデル行列更新
//	model->UpdateTransform(transform);
//}
//
////void EnemyBlueSlime::Render(ID3D11DeviceContext* dc, Shader* shader)
////{
////	shader->Draw(dc, model);
////}
//
//
//// 死亡した時に呼ばれる
//void EnemyBlueSlime::OnDead()
//{
//	Destroy();
//}
//
//void EnemyBlueSlime::DrawDebugPrimitive()
//{
//	// 基底クラスのデバッグプリミティブ描画
//	Enemy::DrawDebugPrimitive();
//
//	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
//
//	// 縄張り範囲をデバッグ円柱描画
//	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
//
//	// ターゲット位置をデバッグ球描画
//	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
//
//	// 索敵範囲をデバッグ円柱描画
//	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
//}
//
//// 縄張り設定
//void EnemyBlueSlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
//{
//	territoryOrigin = origin;
//	territoryRange = range;
//}
//// ターゲット位置をランダム設定
//void EnemyBlueSlime::SetRandomTargetPosition()
//{
//	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
//	float range = Mathf::RandomRange(0.0f, territoryRange);
//	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
//	targetPosition.y = territoryOrigin.y;
//	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
//}
//
//// 目的地点へ移動
//void EnemyBlueSlime::MoveToTarget(float elapsedTime, float speedRate)
//{
//	// ターゲット方向への進行ベクトルを算出
//	float vx = targetPosition.x - position.x;
//	float vz = targetPosition.z - position.z;
//	float dist = sqrtf(vx * vx + vz * vz);
//	vx /= dist;
//	vz /= dist;
//
//	// 移動処理
//	Move(vx, vz, moveSpeed * speedRate);
//	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
//}
//
//
//bool EnemyBlueSlime::SearchPlayer()
//{
//
//	// プレイヤーとの高低差を考慮して3Dで距離判定をする
//	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
//	float vx = playerPosition.x - position.x;
//	float vy = playerPosition.y - position.y;
//	float vz = playerPosition.z - position.z;
//	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
//
//	if (dist < searchRange)
//	{
//		float distXZ = sqrtf(vx * vx + vz * vz);
//		// 単位ベクトル化
//		vx /= distXZ;
//		vz /= distXZ;
//
//		// 方向ベクトル化
//		float frontX = sinf(angle.y);
//		float frontZ = cosf(angle.y);
//		// 2つのベクトルの内積値で前後判定
//		float dot = (frontX * vx) + (frontZ * vz);
//		if (dot > 0.0f)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//// デバッグエネミー情報表示
//void EnemyBlueSlime::DrawDebugGUI()
//{
//	std::string str = "";
//	if(activeNode!=nullptr)
//	{
//		str = activeNode->GetName();
//	}
//	//トランスフォーム
//	if (ImGui::CollapsingHeader("EnemyBlueSlime", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		// 位置
//		ImGui::InputFloat3("Position", &position.x);
//		// 回転
//		DirectX::XMFLOAT3 a;
//		a.x = DirectX::XMConvertToDegrees(angle.x);
//		a.y = DirectX::XMConvertToDegrees(angle.y);
//		a.z = DirectX::XMConvertToDegrees(angle.z);
//		ImGui::InputFloat3("Angle", &a.x);
//		angle.x = DirectX::XMConvertToRadians(a.x);
//		angle.y = DirectX::XMConvertToRadians(a.y);
//		angle.z = DirectX::XMConvertToRadians(a.z);
//		// スケール
//		ImGui::InputFloat3("Scale", &scale.x);
//
//		ImGui::Text(u8"Behavior　%s", str.c_str());
//	}
//}