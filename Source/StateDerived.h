#pragma once
#include "StateBase.h"

// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public State
{
public:
    //  �R���X�g���N�^
    WanderState(EnemyThief* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~WanderState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class IdleState : public State
{
public:
    //  �R���X�g���N�^
    IdleState(EnemyThief* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~IdleState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class PursuitState : public State
{
public:
    //  �R���X�g���N�^
    PursuitState(EnemyThief* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~PursuitState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
private:
    float attackCooldownTimer;
    float attackWarningTime = 1.0f;

    enum class AttackType
    {
        Punch,
        Shot,
    };
};

class AttackState : public State
{
public:
    //  �R���X�g���N�^
    AttackState(EnemyThief* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~AttackState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class ShotState : public State
{
public:
    //  �R���X�g���N�^
    ShotState(EnemyThief* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~ShotState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class SearchState : public HierarchicalState
{
public:
    SearchState(EnemyThief* enemy) : HierarchicalState(enemy) {}
    //  �f�X�g���N�^
    ~SearchState();
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter();
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime);
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit();
};

class BattleState : public HierarchicalState
{
public:
    BattleState(EnemyThief* enemy) : HierarchicalState(enemy) {}
    //  �f�X�g���N�^
    ~BattleState();
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter();
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime);
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit();
};

// ���^AI���烁�b�Z�[�W����M�����Ƃ��ɌĂ΂��X�e�[�g��ǉ�
class RecieveState : public HierarchicalState
{
public:
    RecieveState(EnemyThief* enemy) : HierarchicalState(enemy) {}
    ~RecieveState();
    // �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter();
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime);
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit();
};

// ���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
class CalledState : public State
{
public:
    CalledState(EnemyThief* enemy) :State(enemy) {};
    ~CalledState() {}
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter() override;
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime) override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit() override;
};

//  �ҋ@�X�e�[�g�I�u�W�F�N�g
class StandbyState : public State
{
public:
    StandbyState(EnemyThief* enemy) : State(enemy) {};
    ~StandbyState() {}
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter() override;
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime) override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit() override;
private:
    float attackCooldownTimer;
    float attackWarningTime = 1.0f;

    enum class AttackType
    {
        Punch,
        Shot,
    };
};

