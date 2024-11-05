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
#include "Graphics/geometric_primitive.h"
#include "BrokenProjectileManager.h"
#include <memory.h>
#include "Enemy.h"


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

	//　弾丸の入力処理
	bool InputProjectile();

	// 攻撃入力処理
	bool InputAttack();

	// ロックオンステート
	enum class LockonState
	{
		NotLocked,
		Locked,
	};

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//	弾丸のマネージャーを取る
	ProjectileManager& GetProjectileManager() { return projectileManager; }
	//	破壊した弾丸のマネージャーを取る（仮）
	BrokenProjectileManager& GetBrokenProjectileManager() { return brokenprojectileManager; }
	//	プレイヤーのロックステートを取る
	LockonState GetlockonState() { return lockonState; }
	//　ロック中の敵を取得
	Enemy* GetLockonEnemy() const { return static_cast<Enemy*>(lockonEnemy); }
	//	スイング位置を取得（仮）
	DirectX::XMFLOAT3 GetswingPoint() { return swingPoint; }

	std::unique_ptr<Model> model = nullptr;

	// ステート
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
		Shot,
	};


	// ステート取得
	const State& GetState() const { return state; }
	const State& GetAttackState() const { return State::Attack; }


protected:

	// ダメージを受けたときに呼ばれる
	void OnDamaged() override;

	// 死亡したときに呼ばれる
	void OnDead() override;

	//　着地の時に呼ばれる
	void OnLanding() override;

private:
	// プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();
	//	弾丸と敵との衝突処理
	void CollisionProjectileVsEnemies();

	// ノードとエネミーの衝突処理
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

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

	// スイングステートをへ遷移
	void TransitionSwingState();

	// スイングステート更新処理
	void UpdateSwingState(float elapsedTime);

	// カメラステートの更新
	void UpdateCameraState(float elapsedTime);

	//	発射ステートをへ遷移
	void TransitionShotingState();

	//	発射ステート更新処理
	void UpdateShotingState(float elapsedTime);

	//	連撃攻撃のモーション
	void PlayAttackAnimation();


private:
	//アニメーション
	enum Animation
	{
		Anim_Idle,
		Anim_Walking,
		Anim_GetHit1,
		Anim_Death,
		Anim_Running,
		Anim_Climb,
		Anim_Landing,
		Anim_Jump,
		Anim_ClimbUpWall,
		Anim_ClimbDown,
		Anim_HoldInWall,
		Anim_Attack,
		Anim_Attack2,
		Anim_Kick,
		Anim_StartSwing,
		Anim_Swinging,
		Anim_Swinging2,
		Anim_Shoting,
	};


private:
	// 唯一のinstance
	static Player* instance;
	ProjectileManager projectileManager;
	BrokenProjectileManager brokenprojectileManager;

private:
	// 移動ベクトル取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	//　移動スビート
	float moveSpeed = 5.0f;
	//	クライミングスビート
	float climbSpeed = 5.0f;
	//	回転スビート
	float turnSpeed = DirectX::XMConvertToRadians(720);
	//　ジャンブスビート
	float jumpSpeed = 20.0f;
	//　ヒットエフェクト
	std::unique_ptr<Effect> hitEffect = nullptr;

	// ステート
	State state = State::Idle;
	bool attackCollisionFlag = false;
	float maxAngleX = DirectX::XMConvertToRadians(35);
	float mixAngleX = DirectX::XMConvertToRadians(-35);
	State lastState;

	//モデルノード
	std::vector<Model::NodePose> nodePoses;

	// 攻撃半径
	float attackRadius = 0.4f;
	// 攻撃の回数
	int attackCount = 0;
	int attackLimit = 3;
	float attackTimer = 0;
	//	攻撃判定
	bool attacking = false;

	//カメラロック用
	LockonState			lockonState = LockonState::NotLocked;
	float				lockonTargetChangeTime = 0;
	float				lockonTargetChangeTimeMax = 8;
	Character*			lockonEnemy = nullptr;
	DirectX::XMFLOAT3	lockDirection;

private:
	//糸用
	float swingTime = 0.0f;
	float swingAngle = 0.0f;
	DirectX::XMFLOAT3 swingPoint;
	DirectX::XMVECTOR swingwebDirection;
	DirectX::XMFLOAT3 highestSwingPoint;
};

