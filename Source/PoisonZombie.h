#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include"Effect.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;


class PoisonZombie :public Enemy
{
public:
	PoisonZombie();
	~PoisonZombie()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	void GunSetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// デバッグエネミー情報表示
	void DrawDebugGUI() override;

	// ジャンプターゲット位置をランダム設定
	void SetRandomTargetPosition();

	// 銃攻撃ターゲット範囲をランダム設定
	void SetGunRandomTargetPosition();

	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);

	void JumpMoveToTarget(float elapsedTime, float speedRate);

	// プレイヤー索敵
	bool SearchPlayer();

	// ターゲットポジション設定
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// ターゲットポジション取得
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	// ポジション取得
	DirectX::XMFLOAT3 GetPosition() { return position; }

	// ステートタイマー設定
	void SetStateTimer(float timer) {
		stateTimer = timer;
	}
	// ステートタイマー取得
	float GetStateTimer() { return stateTimer; }

	// 攻撃範囲取得
	float GetAttackRange() { return attackRange; }
	float GetAttackGeroRange() { return geroattackRange; }

	// ジャンプ攻撃範囲取得
	float GetJumpAttackRange() { return jumpAttackRange; }

	// 銃攻撃範囲取得
	float GetGunAttackRange() { return gunAttackRange; }

	// モデル取得
	Model* GetModel() { return model; }

	static PoisonZombie& Instance();

	// 実行タイマー設定
	void SetRunTimer(float timer) { runTimer = timer; }

	// 実行タイマー取得
	float GetRunTimer() { return runTimer; }

	void SetHealth(int setHealth) { health = setHealth; }

	float PlayerToBoss() override;

protected:
	void OnDead();
public:
	// TODO 01 必要なステートをenum classで準備しなさい。
	// （Wanderのみ記述済み）
	enum class State
	{
		Wander,
		Idle,
		Pursuit,
		AttackIdle,
		GunAttack,
		HandAttack,
		JumpAttack
	};
	// エネミーアニメーション
	enum class EnemyAnimation
	{
		IdleNormal,
		Walk,
		GunAttack,
		HandAttack,
		Die,
		AttackIdle,
		JumpAttack,
	};


	// 銃のフラグゲッターセッター
	void SetGunFlg(bool flg) { gunflg = flg; }
	float GetGunFlg() { return gunflg; }

	void SetAttackFlg(bool flg) { attackFlag = flg; }
	float GetAttackFlg() { return attackFlag; }

	void SetAttackCollisionFlg(float min, float max, float animationTime) 
	{
		attackCollisionFlag = animationTime >= min && animationTime <= max;
	}
	bool GetAttackCollisionFlg() { return attackCollisionFlag; }
	
	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

public:
	DirectX::XMFLOAT3 jumpPosition;
	Effect* gero = nullptr;
	int timer = 0.0f;
private:

	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				territoryGunRange = 20.0f;
	float				moveSpeed = 10.0f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				stateTimer = 0.0f;
	float				searchRange = 10.0f;
	float				attackRange = 3.0f;
	float				geroattackRange = 5.0f;
	float				jumpAttackRange = 10.0f;
	float				gunAttackRange = 150.0f;
	bool attackCollisionFlag = false;


	float				runTimer = 0.0f;
	bool				gunflg = false;
	bool attackFlag = false;

	BehaviorTree* aiTree = nullptr;
	BehaviorData* behaviorData = nullptr;
	NodeBase* activeNode = nullptr;
};