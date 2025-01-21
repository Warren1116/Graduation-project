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
#include "EnemyThief.h"


// プレイヤー
class Player : public Character
{
private:
	struct CollisionMesh
	{
		struct Triangle
		{
			DirectX::XMFLOAT3	positions[3];
			DirectX::XMFLOAT3	normal;
		};
		std::vector<Triangle>	triangles;

		struct Area
		{
			DirectX::BoundingBox	boundingBox;
			std::vector<int>		triangleIndices;
		};
		std::vector<Area>		areas;
	};
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

	// Dodge入力処理
	bool InputDodge();

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

	bool GetFirstSwing() { return firstSwing; }

	//	記録されたスイングポイントを取得
	DirectX::XMFLOAT3 GetPreviousSwingPoint() { return previousSwingPoint; }

    //	攻撃予知フラグを取る
	bool GetAttackSoon() { return getAttacksoon; }
	void SetgetAttackSoon(bool getattack) { getAttacksoon = getattack; }
    //  攻撃予知フラグを取る(Shot)
    bool GetShotSoon() { return getShotsoon; }
    void SetgetShotSoon(bool getshot) { getShotsoon = getshot; }


	//　今攻撃するの敵を取得する	
	EnemyThief* GetAttackEnemy() { return attackEnemy; }
    void SetAttackEnemy(EnemyThief* enemy) { attackEnemy = enemy; }

    //    投げ技フラグを取る
	bool GetIsUseGrab() { return IsUseGrab; }
    //    スイングキックフラグを取る
    bool GetIsUseSwingKick() { return IsUseSwingKick; }

	std::unique_ptr<Model> model = nullptr;

	//  糸発射のタイマー
	float GetWebTimer() const { return webTimer; }

    void SetIdleState() { return TransitionIdleState(); }

    void SetVelocity(const DirectX::XMFLOAT3& velocity) { this->velocity = velocity; }


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
		ClimbTop,
		Grab,
		CrouchIdle,
		TitleIdle,
		SwingToKick,

		EventMode,
	};


	// ステート取得
	const State& GetState() const { return state; }
	const State& GetAttackState() const { return State::Attack; }

	float GetSkillTime() const { return skillTime; }

	struct Joint
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	oldPosition;
	};
	Joint		joints[5];

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

	// スイングステートへ遷移
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

	//	回避ステートへ遷移
	void TransitionDodgeState();

	//	回避ステートの更新
	void UpdateDodgeState(float elapsedTime);

	//	登るステートへ遷移
	void TransitionClimbTopState();

	//	登るステートの更新
	void UpdateClimbTopState(float elapsedTime);

	//	投げ技ステートへ遷移
	void TransitionGrabState();

	//	投げ技ステートの更新
	void UpdateGrabState(float elapsedTime);

	//	壁端っこの判定
	bool IsNearWallTop();

	//	スイングポイントを探す
	bool FindWallSwingPoint();

	bool ActiveGrabWeb(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos);


    //Titleステート用
    void TransitionTitleIdleState();
    void UpdateTitleIdleState(float elapsedTime);

    void TransitionCrouchIdleState();
    void UpdateCrouchIdleState(float elapsedTime);

    void TransitionSwingToKickState();
    void UpdateSwingToKickState(float elapsedTime);


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
		Anim_HoldInWall,
		Anim_Attack,
		Anim_Attack2,
		Anim_Kick,
		Anim_StartSwing,
		Anim_Swinging,
		Anim_Swinging2,
		Anim_Shoting,
		Anim_Dodge,
		Anim_ClimbUpWall,
		Anim_ClimbDown,
		Anim_SwingToLand,
		Anim_GrabAndDrop,
		Anim_CrouchIdle,
		Anim_TitleIdle,
		Anim_Crouch,
		Anim_SwingToKick,
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
	float jumpSpeed = 16.0f;
	//　ヒットエフェクト
	std::unique_ptr<Effect> hitEffect = nullptr;
	std::unique_ptr<AudioSource> punch = nullptr;
	std::unique_ptr<AudioSource> punch2 = nullptr;
	std::unique_ptr<AudioSource> kick = nullptr;


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
	//	攻撃予知
	bool getAttacksoon = false;
    bool getShotsoon = false;

    //	攻撃対象
    EnemyThief* attackEnemy = nullptr;

	//	投げ技
	bool IsUseGrab = false;

    //　スイングキック
    bool IsUseSwingKick = false;

    DirectX::XMFLOAT3 dodgeDirection;

	//カメラロック用
	LockonState			lockonState = LockonState::NotLocked;
	float				lockonTargetChangeTime = 0;
	float				lockonTargetChangeTimeMax = 8;
	Character*			lockonEnemy = nullptr;
	DirectX::XMFLOAT3	lockDirection;
	const float MAX_LOCKON_DISTANCE = 15.0f;

private:
	//壁当たり判定のチェック
	DirectX::XMFLOAT3 checkpos;
	DirectX::XMVECTOR checkDirection;

	//糸用
	//	スイングポイントの位置
	DirectX::XMFLOAT3 swingPoint;
	//	スイングポイントの方向
	DirectX::XMFLOAT3 swingwebDirection;
	DirectX::XMVECTOR SwingwebDirection;
	//	初回のスイングを判定
	bool firstSwing = true;
	float SwingCounter;
	//	前回のスイングポイントを記録
	DirectX::XMFLOAT3 previousSwingPoint;
	// 糸のタイマー
	float webTimer;
	float skillTime;

private:
	// メッセージキー
	uint64_t	EventModeIndex = -1;
	uint64_t	GameModeIndex = -1;

	uint64_t	SetPositionIndex = -1;
	uint64_t	MoveIndex = -1;
	struct
	{
		DirectX::XMFLOAT3 moveVec;
		float moveSpeed;
		float turnSpeed;
	} moveData;
	uint64_t	MotionIndex = -1;

};

