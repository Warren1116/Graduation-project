//#pragma once
//
//#include "Graphics/Model.h"
//#include "Enemy.h"
//#include "Effect.h"
//#include "Object.h"
//
//// スライム
//class TutorialZombie : public Enemy
//{
//public:
//	TutorialZombie();
//	~TutorialZombie() override;
//
//	// 更新処理
//	void Update(float elapsedTime) override;
//
//	// デバッグプリミティブ描画
//	void DrawDebugPrimitive() override;
//
//	void TransitionTitleState();
//	void UpdateTitleState(float elapsedTime);
//
//private:
//	// ターゲット位置をランダムに設定
//	void SetRandomTargetPosition();
//
//	// 目的地点へ移動
//	void MoveToTarget(float elapsedTime, float speedRate);
//
//	// プレイヤー索敵
//	bool SearchPlayer();
//
//	// ノードとプレイヤーの衝突処理
//	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);
//
//	// 徘徊ステートへ遷移
//	void TransitionWanderState();
//
//	// 徘徊ステート更新処理
//	void UpdateWanderState(float elapsedTime);
//
//	// 待機ステートへ遷移
//	void TransitionIdleState();
//
//	// 待機ステート更新処理
//	void UpdateIdleState(float elapsedTime);
//
//	// 追跡ステートへ遷移
//	void TransitionPursuitState();
//
//	// 追跡ステート更新処理
//	void UpdatePursuitState(float elapsedTime);
//
//	// 攻撃ステートへ遷移
//	void TransitionAttackState();
//
//	// 攻撃ステート更新処理
//	void UpdateAttackState(float elapsedTime);
//
//	// 戦闘待機ステートへ遷移
//	void TransitionIdleBattleState();
//
//	// 戦闘待機ステート更新処理
//	void UpdateIdleBattleState(float elapsedTime);
//
//	// ダメージステートへ遷移
//	void TransitionDamageState();
//
//	// ダメージステート更新処理
//	void UpdateDamageState(float elapsedTime);
//
//	// 死亡ステートへ遷移
//	void TransitionDeathState();
//
//	// 死亡ステート更新処理
//	void UpdateDeathState(float elapsedTime);
//
//private:
//	// ステート
//	enum class State
//	{
//		Wander,
//		Idle,
//		Pursuit,
//		Attack,
//		IdleBattle,
//		Damage,
//		Death,
//		Title,
//	};
//
//	// アニメーション
//	enum
//	{
//		Anim_Death,
//		Anim_Attack1,
//		Anim_RunFWD,
//		Anim_IdleNormal,
//	};
//
//protected:
//	// 死亡したときに呼ばれる
//	void OnDamaged() override;
//
//	// 死亡したときに呼ばれる
//	void OnDead() override;
//
//private:
//	State state = State::Wander;
//	DirectX::XMFLOAT3 targetPosition = { 0, 0, 0 };
//	DirectX::XMFLOAT3 territoryOrigin = { 0, 0, 0 };
//	float territoryRange = 10.0f;
//	float moveSpeed = 3.0f;
//	float turnSpeed = DirectX::XMConvertToRadians(360);
//	float stateTimer = 0.0f;
//	float searchRange = 5.0f;
//	float attackRange = 1.5f;
//	bool attackCollisionFlag = false;
//
//	float timer = 0;
//};