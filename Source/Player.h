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
#include "BrokenProjectileManager.h"
#include <memory.h>
#include "Enemy.h"
#include "EnemyThief.h"
#include "PlayerStateMachine.h"
#include "PlayerStateDerived.h"


//アニメーション
enum class PlayerAnimation
{
    Anim_Idle, Anim_Walking, Anim_GetHit1, Anim_Death, Anim_Running,
    Anim_Climb, Anim_Landing, Anim_Jump, Anim_HoldInWall, Anim_Attack,
    Anim_Attack2, Anim_Kick, Anim_StartSwing, Anim_Swinging, Anim_Swinging2,
    Anim_Shoting, Anim_Dodge, Anim_ClimbUpWall, Anim_ClimbDown, Anim_SwingToLand,
    Anim_GrabAndDrop, Anim_CrouchIdle, Anim_TitleIdle, Anim_Crouch,
    Anim_SwingToKick, Anim_Ultimate
};

//　ステートマシン
class PlayerStateMachine;

// プレイヤーの状態を管理するクラス
namespace PlayerStates
{
    class IdleState; class MoveState; class JumpState; class ClimbState;
    class AttackState; class DamageState; class SwingState; class SwingToKickState;
    class LandState; class ShotState; class GrabState; class DodgeState;
    class ClimbTopState; class UltimateState; class DeathState;
    class CrouchIdleState; class TitleIdleState; class BounceState;
}


// プレイヤー
class Player : public Character
{
    //　Friendクラス宣言
    friend class PlayerStates::IdleState;
    friend class PlayerStates::MoveState;
    friend class PlayerStates::JumpState;
    friend class PlayerStates::ClimbState;
    friend class PlayerStates::AttackState;
    friend class PlayerStates::DamageState;
    friend class PlayerStates::SwingState;
    friend class PlayerStates::SwingToKickState;
    friend class PlayerStates::LandState;
    friend class PlayerStates::ShotState;
    friend class PlayerStates::GrabState;
    friend class PlayerStates::DodgeState;
    friend class PlayerStates::ClimbTopState;
    friend class PlayerStates::UltimateState;
    friend class PlayerStates::DeathState;
    friend class PlayerStates::CrouchIdleState;
    friend class PlayerStates::TitleIdleState;
    friend class PlayerStates::BounceState;

public:
    // ステート
    enum class State
    {
        Idle, Move, Jump, Land, Attack, Damage, Death, Dodge,
        Climb, Swing, Shot, ClimbTop, Grab, CrouchIdle, TitleIdle,
        SwingToKick, Ultimate, Bounce, EventMode
    };

    // ロックオンステート
    enum class LockonState
    {
        NotLocked,
        Locked,
    };

    Player(bool flag);
    ~Player();
    static Player& Instance() { return *instance; }

    // ステートマシン
    PlayerStateMachine* stateMachine = nullptr;
    //　モデル
    std::unique_ptr<Model> model = nullptr;

    // 関数
    void Update(float elapsedTime);     // 更新処理
    void DrawDebugGUI();                // デバッグ用GUI描画
    bool InputMove(float elapsedTime);  // 移動入力処理
    bool InputJump();                   // ジャンプ入力処理
    bool InputProjectile();             //　弾丸の入力処理
    bool InputAttack();                 // 攻撃入力処理
    bool InputDodge();                  // Dodge入力処理
    bool InputHealing();                //　回復入力処理
    void DrawDebugPrimitive();          // デバッグプリミティブ描画

    ///////////////////////////////////////////////////////////////////////////////////
    //                              ゲッター・セッター                               //
    ///////////////////////////////////////////////////////////////////////////////////
    //弾丸
    ProjectileManager& GetProjectileManager() { return projectileManager; }    //	弾丸のマネージャーを取る
    BrokenProjectileManager& GetBrokenProjectileManager() { return brokenprojectileManager; }    //	破壊した弾丸のマネージャーを取る

    //敵
    Enemy* GetLockonEnemy() const { return static_cast<Enemy*>(lockonEnemy); }    //　ロック中の敵を取得
    EnemyThief* GetAttackEnemy() { return attackEnemy; }                          //　今攻撃するの敵を取得する
    void SetAttackEnemy(EnemyThief* enemy) { attackEnemy = enemy; }
    bool GetAttackSoon() { return getAttacksoon; }                                //　攻撃予知フラグを取る
    void SetgetAttackSoon(bool getattack) { getAttacksoon = getattack; }
    bool GetShotSoon() { return getShotsoon; }                                    //  攻撃予知フラグを取る(Shot)
    void SetgetShotSoon(bool getshot) { getShotsoon = getshot; }

    //スイング
    DirectX::XMFLOAT3 GetswingPoint() { return swingPoint; }    //	スイング位置を取得
    DirectX::XMFLOAT3 GetPreviousSwingPoint() { return previousSwingPoint; }    //	記録されたスイングポイントを取得

    //　スキル
    bool GetIsUseGrab() { return IsUseGrab; }               //    投げ技フラグを取る
    bool GetIsUseSwingKick() { return IsUseSwingKick; }    //    スイングキックフラグを取る
    float GetWebTimer() const { return webTimer; }    //  糸発射のタイマー
    float GetSkillTime() const { return skillTime; }    //  スキルの回数を取る

    // ステート
    const State& GetState() const { return state; }
    State SetState(State state) { return this->state = state; }
    State ChangeState(State state) { return this->state = state; }
    LockonState GetlockonState() { return lockonState; }    //	プレイヤーのロックステートを取る

protected:
    // ダメージを受けたときに呼ばれる
    void OnDamaged() override;
    // 死亡したときに呼ばれる
    void OnDead() override;
    //　着地の時に呼ばれる
    void OnLanding() override;

private:
    // コリジョンメッシュ
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

    // 唯一のinstance
    static Player* instance;
    //モデルノード
    std::vector<Model::NodePose> nodePoses;

    ///////////////////////////////////////////////////////////////////////////////////
    //                                   プレイヤー                                  //
    ///////////////////////////////////////////////////////////////////////////////////
    // ステート
    State state = State::Idle;
    State lastState;

    // 戦闘
    bool attacking = false;    // 攻撃判定
    float attackTimer = 0;
    int attackCount = 0;    // 攻撃の回数
    const int attackLimit = 3;
    float attackRadius = 0.4f;    // 攻撃半径
    bool attackCollisionFlag = false;
    float maxAngleX = DirectX::XMConvertToRadians(35);
    float mixAngleX = DirectX::XMConvertToRadians(-35);
    bool getAttacksoon = false;    //	攻撃予知
    bool getShotsoon = false;    //	攻撃予知

    // スイング
    DirectX::XMFLOAT3 previousSwingPoint;        //	前回のスイングポイントを記録
    DirectX::XMFLOAT3 swingPoint;    //	スイングポイントの位置
    DirectX::XMFLOAT3 swingwebDirection;    //	スイングポイントの方向
    DirectX::XMVECTOR SwingwebDirection;
    bool firstSwing = true;    //	初回のスイングを判定
    float swingCooldown = 0.3f;    //連続スイングを避けるため
    float swingCooldownTimer = 0.0f;
    bool canSwing = true;

    // バウンス
    bool ropeAttached = false;
    float bounceTimer = 0.0f;
    const float ropeCoolDown = 1.0f;

    // カメラロック用
    LockonState			lockonState = LockonState::NotLocked;
    float				lockonTargetChangeTime = 0;
    float				lockonTargetChangeTimeMax = 8;
    Character* lockonEnemy = nullptr;
    DirectX::XMFLOAT3	lockDirection;
    const float MAX_LOCKON_DISTANCE = 15.0f;

    // 投げ技・必殺技用
    bool IsUseGrab = false;         // 投げ技
    float webTimer;                 // 糸のタイマー
    bool IsUseSwingKick = false;    // スイングキック
    float ultimateAttackRadius = 5.0f;// 攻撃範囲の半径
    float skillTime;                // 
    float skillTimeMax = 5.0f;      // 

    // 音・エフェクト
    std::unique_ptr<Effect> hitEffect = nullptr;        //　ヒットエフェクト
    std::unique_ptr<AudioSource> punch = nullptr;       //	攻撃音
    std::unique_ptr<AudioSource> punch2 = nullptr;      //	攻撃音
    std::unique_ptr<AudioSource> kick = nullptr;        //	攻撃音
    std::unique_ptr<AudioSource> Fall = nullptr;        //  落とす音
    bool fallSoundPlayed = false;
    std::unique_ptr<AudioSource> spiderSense = nullptr; //  スパイダーセンス
    bool spiderSensePlayed = false;
    std::unique_ptr<AudioSource> FirstSwing = nullptr;  //　シュート音
    std::unique_ptr<AudioSource> Swing = nullptr;       //　シュート音
    std::unique_ptr<AudioSource> ShotWeb = nullptr;     //　シュート音
    bool shotWebPlayed = false;
    std::unique_ptr<AudioSource> Healing = nullptr;     //  回復音
    std::unique_ptr<AudioSource> Damage = nullptr;      //	ダメージ音

    // 弾丸
    ProjectileManager projectileManager;
    BrokenProjectileManager brokenprojectileManager;

    // 壁当たり判定のチェック
    DirectX::XMFLOAT3 checkpos;
    DirectX::XMVECTOR checkDirection;

    // 移動
    float moveSpeed = 5.0f;                                //　移動スビート
    float climbSpeed = 5.0f;                               //　クライミングスビート
    float turnSpeed = DirectX::XMConvertToRadians(720);    //　回転スビート
    float jumpSpeed = 16.0f;                               //　ジャンブスビート
    DirectX::XMFLOAT3 dodgeDirection;                      //  回避の方向

    // 敵
    EnemyThief* attackEnemy = nullptr;                     //　攻撃対象

    // 関数
    void CollisionPlayerVsEnemies();    // プレイヤーとエネミーとの衝突処理
    void CollisionProjectileVsEnemies();    //	弾丸と敵との衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);    // ノードとエネミーの衝突処理
    void UpdateCameraState(float elapsedTime);    // カメラステートの更新
    void PlayAttackAnimation();    //	連撃攻撃のモーション
    bool IsNearWallTop();    //	壁端っこの判定
    bool FindWallSwingPoint();    //	スイングポイントを探す
    void SwingCollision(float elapsedTime);    //	スイングの当たり判定
    // スイングの計算
    void HandleSwingPhysics(float elapsedTime, float ropeLength, float gravityStrength, float dragCoefficient);
    //　投げ技Active
    bool ActiveGrabWeb(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos);
    DirectX::XMFLOAT3 GetMoveVec() const;    // 移動ベクトル取得
};

