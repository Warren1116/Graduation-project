#pragma once
#include "PlayerStateBase.h"
#include "Graphics\LineRenderer.h"
#include "Audio/Audio.h"

namespace PlayerStates
{
    //�ҋ@�X�e�[�g
    class IdleState : public PlayerState
    {
    public:
        IdleState(Player* player) : PlayerState(player) {};
        ~IdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�ړ��X�e�[�g
    class MoveState : public PlayerState
    {
    public:
        MoveState(Player* player) : PlayerState(player) {};
        ~MoveState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�W�����v�X�e�[�g
    class JumpState : public PlayerState
    {
    public:
        JumpState(Player* player) : PlayerState(player) {};
        ~JumpState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //���n�X�e�[�g
    class LandState : public PlayerState
    {
    public:
        LandState(Player* player) : PlayerState(player) {};
        ~LandState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�U���X�e�[�g
    class AttackState : public PlayerState
    {
    public:
        AttackState(Player* player) : PlayerState(player) {};
        ~AttackState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�_���[�W�X�e�[�g
    class DamageState : public PlayerState
    {
    public:
        DamageState(Player* player) : PlayerState(player) {};
        ~DamageState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //���S�X�e�[�g
    class DeathState : public PlayerState
    {
    public:
        DeathState(Player* player) : PlayerState(player) {};
        ~DeathState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //����X�e�[�g
    class DodgeState : public PlayerState
    {
    public:
        DodgeState(Player* player) : PlayerState(player) {};
        ~DodgeState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�N���C�~���O�X�e�[�g
    class ClimbState : public PlayerState
    {
    public:
        ClimbState(Player* player) : PlayerState(player) {};
        ~ClimbState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�X�C���O�X�e�[�g
    class SwingState : public PlayerState
    {
    public:
        SwingState(Player* player) : PlayerState(player) {};
        ~SwingState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�V���b�g�X�e�[�g
    class ShotState : public PlayerState
    {
    public:
        ShotState(Player* player) : PlayerState(player) {};
        ~ShotState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�N���C���g�b�v�X�e�[�g
    class ClimbTopState : public PlayerState
    {
    public:
        ClimbTopState(Player* player) : PlayerState(player) {};
        ~ClimbTopState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�O���u�X�e�[�g
    class GrabState : public PlayerState
    {
    public:
        GrabState(Player* player) : PlayerState(player) {};
        ~GrabState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�ҋ@�p
    class CrouchIdleState : public PlayerState
    {
    public:
        CrouchIdleState(Player* player) : PlayerState(player) {};
        ~CrouchIdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�ҋ@�p
    class TitleIdleState : public PlayerState
    {
    public:
        TitleIdleState(Player* player) : PlayerState(player) {};
        ~TitleIdleState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�X�C���O�L�b�N�X�e�[�g
    class SwingToKickState : public PlayerState
    {
    public:
        SwingToKickState(Player* player) : PlayerState(player) {};
        ~SwingToKickState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    //�K�E�Z�X�e�[�g
    class UltimateState : public PlayerState
    {
    public:
        UltimateState(Player* player) : PlayerState(player) {};
        ~UltimateState() {}
        void Enter()override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    // �o�E���X�X�e�[�g
    class BounceState : public PlayerState
    {
    public:
        BounceState(Player* player) : PlayerState(player) {};
        ~BounceState() {}
        void Enter() override;
        void Execute(float elapsedTime)override;
        void Exit()override;
    };

    // �t���C�p���`�X�e�[�g
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