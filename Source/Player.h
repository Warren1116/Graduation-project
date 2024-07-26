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

    bool InputDodge();  //回避

    void TransitionDodgeState();

    void UpdateDodgeState(float elapsedTime);

    void TransitionDamageState();

    void UpdateDamageState(float elapsedTime);

    void TransitionDeathState();

    void UpdateDeathState(float elapsedTime);

    void TransitionReviveState();

    void UpdateReviveState(float elapsedTime);



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

    float leftHandRadius = 0.4f;

    bool attackCollisionFlag = false;

    //アニメーション
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking,
        Anim_Dodge,
    };
protected:
    void OnDamaged() override;
    void OnDead() override;

};
