#pragma once
#include "PlayerStateBase.h"
#include "Graphics\LineRenderer.h"
#include "Audio/Audio.h"

namespace PlayerStates
{
    //待機ステート
    class IdleState : public PlayerState
    {
    public:
        IdleState(Player* player) : PlayerState(player) {};
        ~IdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //移動ステート
    class MoveState : public PlayerState
    {
    public:
        MoveState(Player* player) : PlayerState(player) {};
        ~MoveState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //ジャンプステート
    class JumpState : public PlayerState
    {
    public:
        JumpState(Player* player) : PlayerState(player) {};
        ~JumpState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //着地ステート
    class LandState : public PlayerState
    {
    public:
        LandState(Player* player) : PlayerState(player) {};
        ~LandState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //攻撃ステート
    class AttackState : public PlayerState
    {
    public:
        AttackState(Player* player) : PlayerState(player) {};
        ~AttackState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //ダメージステート
    class DamageState : public PlayerState
    {
    public:
        DamageState(Player* player) : PlayerState(player) {};
        ~DamageState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //死亡ステート
    class DeathState : public PlayerState
    {
    public:
        DeathState(Player* player) : PlayerState(player) {};
        ~DeathState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //回避ステート
    class DodgeState : public PlayerState
    {
    public:
        DodgeState(Player* player) : PlayerState(player) {};
        ~DodgeState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //クライミングステート
    class ClimbState : public PlayerState
    {
    public:
        ClimbState(Player* player) : PlayerState(player) {};
        ~ClimbState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //スイングステート
    class SwingState : public PlayerState
    {
    public:
        SwingState(Player* player) : PlayerState(player) {};
        ~SwingState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //ショットステート
    class ShotState : public PlayerState
    {
    public:
        ShotState(Player* player) : PlayerState(player) {};
        ~ShotState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //クライムトップステート
    class ClimbTopState : public PlayerState
    {
    public:
        ClimbTopState(Player* player) : PlayerState(player) {};
        ~ClimbTopState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //グラブステート
    class GrabState : public PlayerState
    {
    public:
        GrabState(Player* player) : PlayerState(player) {};
        ~GrabState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //待機用
    class CrouchIdleState : public PlayerState
    {
    public:
        CrouchIdleState(Player* player) : PlayerState(player) {};
        ~CrouchIdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //待機用
    class TitleIdleState : public PlayerState
    {
    public:
        TitleIdleState(Player* player) : PlayerState(player) {};
        ~TitleIdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //スイングキックステート
    class SwingToKickState : public PlayerState
    {
    public:
        SwingToKickState(Player* player) : PlayerState(player) {};
        ~SwingToKickState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //必殺技ステート
    class UltimateState : public PlayerState
    {
    public:
        UltimateState(Player* player) : PlayerState(player) {};
        ~UltimateState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    // バウンスステート
    class BounceState : public PlayerState
    {
    public:
        BounceState(Player* player) : PlayerState(player) {};
        ~BounceState() {}
        void Enter() override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    // フライパンチステート
    class FlyPunchState : public PlayerState
    {
    public:
        FlyPunchState(Player* player) : PlayerState(player) {};
        ~FlyPunchState() {}
        void Enter() override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };
}