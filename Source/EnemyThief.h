#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "Telegram.h"

#define USESTATEMACHINE3



// エネミーアニメーション
enum class EnemyAnimation
{
	Idle,
	Walk,
	Run,
	AttackShot,
	Die,
	GetHit,
	AttackPunch,
	GetThrow,
	HoldGun,
	KickDown,
};

class StateMachine;

class EnemyThief :public Enemy
{
public:
	EnemyThief();
	~EnemyThief()override;

    // 攻撃タイプ
	enum class AttackType
	{
		Null,
		Punch,
		Shot,
	};
	AttackType randomType = AttackType::Null;


	// 更新処理
	void Update(float elapsedTime)override;

	//// 描画処理
	//void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// デバッグエネミー情報表示
	void DrawDebugGUI();
	// ターゲット位置をランダム設定
	void SetRandomTargetPosition();

	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);

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
	float GetPunchRange() { return punchRange; }

	//2週目
	//ステートマシン取得
	StateMachine* GetStateMachine() { return stateMachine; }
	//モデル取得
	Model* GetModel() { return model.get(); }

	//	5週目　
	//TODO 05_02 メッセージ受信関数を追加
	bool OnMessage(const Telegram& msg);

	void SetThrown(bool thrown) { isThrown = thrown; }
	bool IsThrown() const { return isThrown; }

private:

	// 死亡ステートへ遷移
	void TransitionDeathState();

	// 死亡ステート更新処理
	void UpdateDeathState(float elapsedTime);

	// ダメージステートへ遷移
	void TransitionDamageState();
    // ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);


protected:
	void OnDead();


public:
	enum class State
	{
		Search,
		Battle,
		// MetaAIからメッセージを受信したときのステートを追加
		Recieve,
		Dead,

		Damage,
	};

	enum class Search
	{
		Wander,
		Idle,
	};

	enum class Battle
	{
		Pursuit,
		Attack,
		Punch,
		Shot,
		// 戦闘中に攻撃権を持っていないときの処理を追加
		Standby,
		Dead,
	};

	//TODO を経由して他のエネミーから呼ばれたときの処理
	enum class Recieve
	{
		Called,
	};

	float				stateTimer = 0.0f;


private:
	std::unique_ptr<Model> model;
	State				state = State::Search;

	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				moveSpeed = 3.5f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);

	float				searchRange = 7.0f;
	float				attackRange = 10.0f;
	float				punchRange = 1.5f;
	StateMachine* stateMachine = nullptr;


	float timeElapsed = 0.0f;
	float totalFlightTime;
	float webTimer;
	bool ThrowFlag;


};