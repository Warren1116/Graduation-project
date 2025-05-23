#pragma once
//#include "PlayerStateBase.h"
#include "Graphics\LineRenderer.h"
#include "Audio/Audio.h"


#include "Player.h"
class Player;

class PlayerState
{
public:
    PlayerState(Player* player) : owner(player) {}
    virtual ~PlayerState() {}
    virtual void Enter() = 0;
    virtual void Execute(float elapsedTime) = 0;
    virtual void Exit() = 0;
protected:
    Player* owner;
};


namespace PlayerStates
{
    class IdleState : public PlayerState
    {
    public:
        IdleState(Player* player) : PlayerState(player) {};
        ~IdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class MoveState : public PlayerState
    {
    public:
        MoveState(Player* player) : PlayerState(player) {};
        ~MoveState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class JumpState : public PlayerState
    {
    public:
        JumpState(Player* player) : PlayerState(player) {};
        ~JumpState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class LandState : public PlayerState
    {
    public:
        LandState(Player* player) : PlayerState(player) {};
        ~LandState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class AttackState : public PlayerState
    {
    public:
        AttackState(Player* player) : PlayerState(player) {};
        ~AttackState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class DamageState : public PlayerState
    {
    public:
        DamageState(Player* player) : PlayerState(player) {};
        ~DamageState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class DeathState : public PlayerState
    {
    public:
        DeathState(Player* player) : PlayerState(player) {};
        ~DeathState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class DodgeState : public PlayerState
    {
    public:
        DodgeState(Player* player) : PlayerState(player) {};
        ~DodgeState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class ClimbState : public PlayerState
    {
    public:
        ClimbState(Player* player) : PlayerState(player) {};
        ~ClimbState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class SwingState : public PlayerState
    {
    public:
        SwingState(Player* player) : PlayerState(player) {};
        ~SwingState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class ShotState : public PlayerState
    {
    public:
        ShotState(Player* player) : PlayerState(player) {};
        ~ShotState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class ClimbTopState : public PlayerState
    {
    public:
        ClimbTopState(Player* player) : PlayerState(player) {};
        ~ClimbTopState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class GrabState : public PlayerState
    {
    public:
        GrabState(Player* player) : PlayerState(player) {};
        ~GrabState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class CrouchIdleState : public PlayerState
    {
    public:
        CrouchIdleState(Player* player) : PlayerState(player) {};
        ~CrouchIdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class TitleIdleState : public PlayerState
    {
    public:
        TitleIdleState(Player* player) : PlayerState(player) {};
        ~TitleIdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class SwingToKickState : public PlayerState
    {
    public:
        SwingToKickState(Player* player) : PlayerState(player) {};
        ~SwingToKickState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    class UltimateState : public PlayerState
    {
    public:
        UltimateState(Player* player) : PlayerState(player) {};
        ~UltimateState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };
}