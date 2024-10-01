#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"


class Player : public Character
{
public:

    static Player& Instance();

    Player();
    ~Player() override;


    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();


    DirectX::XMFLOAT3 GetMoveVec() const;

    bool InputMove(float elapsedTime);


    void CollisionPlayerVsEnemies();

    bool InputJump();

    void OnLanding() override;

    void CollisionProjectileVsEnemies();

    void TransitionIdleState();

    void UpdateIdleState(float elapsedTime);

    void TransitionMoveState();

    void UpdateMoveState(float elapsedTime);

    void TransitionJumpState();

    void UpdateJumpState(float elapsedTime);

    void TransitionLandState();

    void UpdateLandState(float elapsedTime);

    bool InputAttack();

    void TransitionAttackState();

    void UpdateAttackState(float elapsedTime);

    //bool InputDodge();  //回避

    //void TransitionDodgeState();

    //void UpdateDodgeState(float elapsedTime);

    void TransitionDamageState();

    void UpdateDamageState(float elapsedTime);

    void TransitionDeathState();

    void UpdateDeathState(float elapsedTime);

    void TransitionClimbWallState();

    void UpdateClimbWallState(float elapsedTime);

    ////モーション更新
    //void UpdateMotion(float elapsedTime);

    void PlayAttackAnimation();


private:
    ProjectileManager projectileManager;

    void InputProjectile();

    //ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

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
    };


private:
    Model* model = nullptr;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float moveSpeed = 5.0f;

    float jumpSpeed = 20.0f;

    //float gravity = -1.0f;

    //DirectX::XMFLOAT3 velocity = { 0,0,0 };

    int jumpCount = 0;

    int jumpLimit = 2;

    Effect* hitEffect = nullptr;

    State   state = State::Idle;

    float HandRadius = 0.4f;

    bool attackCollisionFlag = false;

    int animationIndex = -1;

    float animationSeconds = 0;

    float oldAnimationSeconds = 0;

    std::vector<Model::NodePose> nodePoses;

    DirectX::XMFLOAT4X4	worldTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

    bool animationLoop = false;
    bool bakeTranslationY = false;

    //攻撃の回数
    int attackCount = 0;
    int attackLimit = 3;
    float attackTimer = 0;

    bool attacking = false;


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

    };
protected:
    void OnDamaged() override;
    void OnDead() override;

};
