#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "Audio/Audio.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "CameraController.h"
#include "ProjectileManager.h"


// プレイヤー
class Player : public Character
{
public:
	Player(bool flag);
	~Player();
	static Player& Instance() { return *instance; }

	// 更新処理
	void Update(float elapsedTime);

	// デバッグ用GUI描画
	void DrawDebugGUI();

	// 移動入力処理
	bool InputMove(float elapsedTime);

	// ジャンプ入力処理
	bool InputJump();

	void OnLanding() override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//void SetHaveKey(const bool& haveKey) { this->haveKey = haveKey; }
	//bool HaveKey() const { return haveKey; }

	//void SetHaveAmmoNum(const int haveAmmoNum) { this->haveAmmoNum = haveAmmoNum; }
	//int GetHaveAmmoNum() const { return haveAmmoNum; }

	//bool NearStairs() const { return nearStairs; }

	ProjectileManager GetProjectileManager() { return projectileManager; }


	Model* model = nullptr;

	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Damage,
		Death,
		Revive,
		Dodge,
		Climb,
		Swing,
	};


	// ステート取得
	const State& GetState() const { return state; }
	const State& GetAttackState() const { return State::Attack; }

	//std::unique_ptr<AudioSource> null = nullptr;


protected:

	// 死亡したときに呼ばれる
	void OnDamaged() override;

	// 死亡したときに呼ばれる
	void OnDead() override;

private:
	// 攻撃入力処理
	bool InputAttack();
	// プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	void CollisionProjectileVsEnemies();

	// 待機ステートへ遷移
	void TransitionIdleState();

	// 待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	// 移動ステートへ遷移
	void TransitionMoveState();

	// 移動ステート更新処理
	void UpdateMoveState(float elapsedTime);

	// ジャンプステートへ遷移
	void TransitionJumpState();

	// ジャンプステート更新処理
	void UpdateJumpState(float elapsedTime);

	// クランプステートへ遷移
	void TransitionClimbWallState();

	// クランプステート更新処理
	void UpdateClimbWallState(float elapsedTime);

	// 攻撃ステートへ遷移
	void TransitionAttackState();

	// 攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);

	// ダメージステートへ遷移
	void TransitionDamageState();

	// ダメージステート更新処理
	void UpdateDamageState(float elapsedTime);

	// 死亡ステートへ遷移
	void TransitionDeathState();

	// 死亡ステート更新処理
	void UpdateDeathState(float elapsedTime);

	// 着地ステートへ遷移 
	void TransitionLandState();

	// 着地ステート更新処理
	void UpdateLandState(float elapsedTime);

	// Swingテートへ遷移
	void TransitionSwingState();

	// Swingステート更新処理
	void UpdateSwingState(float elapsedTime);

	// ノードとエネミーの衝突処理
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	void PlayAttackAnimation();

private:
	static Player* instance;

	ProjectileManager projectileManager;

	void InputProjectile();

private:
	//アニメーション
	enum Animation
	{
		Anim_Idle,
		Anim_Walking,
		Anim_Attack,
		Anim_Attack2,
		Anim_Kick,
		Anim_GetHit1,
		Anim_Death,
		Anim_Running,
		Anim_Climb,
		Anim_Landing,
		Anim_Jump,
		Anim_ClimbUpWall,
		Anim_ClimbDown,
		Anim_HoldInWall,

	};

private:
	DirectX::XMFLOAT3 GetMoveVec() const;

	float moveSpeed = 5.0f;

	float climbSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(10);
	float jumpSpeed = 20.0f;
	int jumpCount = 0;
	int jumpLimit = 1;
	//int haveAmmoNum = 50;
	Effect* hitEffect = nullptr;
	Effect* blowEffect = nullptr;
	std::unique_ptr<AudioSource> outOfBullets = nullptr;
	std::unique_ptr<AudioSource> walk = nullptr;
	
	State state = State::Idle;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	float maxAngleX = DirectX::XMConvertToRadians(35);
	float mixAngleX = DirectX::XMConvertToRadians(-35);


	std::vector<Model::NodePose> nodePoses;

	float HandRadius = 0.4f;

	//攻撃の回数
	int attackCount = 0;
	int attackLimit = 3;
	float attackTimer = 0;

	bool attacking = false;

	float swingTime = 0.0f;
	float swingAngle = 0.0f;

	DirectX::XMFLOAT3 swingPoint = { 0.0f, 10.0f, 0.0f };


};

