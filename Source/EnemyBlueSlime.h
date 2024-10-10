//#pragma once
//#include "Graphics/Model.h"
//#include "EnemyPeople.h"
//
//class BehaviorTree;
//class BehaviorData;
//class NodeBase;
//
//class EnemyBlueSlime :public Enemy
//{
//public:
//	EnemyBlueSlime();
//	~EnemyBlueSlime()override;
//
//	// 更新処理
//	void Update(float elapsedTime)override;
//
//	//// 描画処理
//	//void Render(ID3D11DeviceContext* dc, Shader* shader)override;
//
//	// デバッグプリミティブ描画
//	void DrawDebugPrimitive() override;
//
//	// 縄張り設定
//	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
//
//	// ターゲット位置をランダム設定
//	void SetRandomTargetPosition();
//
//	// 目標地点へ移動
//	void MoveToTarget(float elapsedTime, float speedRate);
//
//	// プレイヤー索敵
//	bool SearchPlayer();
//
//	// ターゲットポジション設定
//	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }
//
//	// ターゲットポジション取得
//	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
//
//	// ポジション取得
//	DirectX::XMFLOAT3 GetPosition() { return position; }
//
//	// 攻撃範囲取得
//	float GetAttackRange() { return attackRange; }
//
//	// デバッグエネミー情報表示
//	void DrawDebugGUI();
//
//	// 実行タイマー設定
//	void SetRunTimer(float timer) { runTimer = timer; }
//
//	// 実行タイマー取得
//	float GetRunTimer() { return runTimer; }
//
//	Model* GetModel() { return model; }
//
//	void SetHealth(int setHealth) { health = setHealth; }
//
//protected:
//	// 死亡したときに呼ばれる
//	void OnDead() override;
//public:
//	// アニメーション
//	enum class EnemyAnimation
//	{
//		IdleNormal,
//		IdleBattle,
//		Attack01,
//		Attack02,
//		WalkFWD,
//		WalkBWD,
//		WalkLeft,
//		WalkRight,
//		RunFWD,
//		SenseSomethingST,
//		SenseSomethingRPT,
//		Taunt,
//		Victory,
//		GetHit,
//		Dizzy,
//		Die,
//	};
//
//
//private:
//	Model* model = nullptr;
//	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
//	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
//	float				territoryRange = 10.0f;
//	float				moveSpeed = 3.0f;
//	float				turnSpeed = DirectX::XMConvertToRadians(360);
//	float				searchRange = 5.0f;
//	float				attackRange = 1.5f;
//	float				runTimer = 0.0f;
//
//	BehaviorTree*		aiTree = nullptr;
//	BehaviorData*		behaviorData = nullptr;
//	NodeBase*			activeNode = nullptr;
//};