#pragma once
#include "StateBase.h"
#include "Graphics\LineRenderer.h"
#include "Audio/Audio.h"


// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public EnemyState
{
public:
    //  �R���X�g���N�^
    WanderState(EnemyThief* enemy) : EnemyState(enemy) {};
    //  �f�X�g���N�^
    ~WanderState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class IdleState : public EnemyState
{
public:
    //  �R���X�g���N�^
    IdleState(EnemyThief* enemy) : EnemyState(enemy) {};
    //  �f�X�g���N�^
    ~IdleState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class PursuitState : public EnemyState
{
public:
    //  �R���X�g���N�^
    PursuitState(EnemyThief* enemy) : EnemyState(enemy) {};
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


};

class AttackState : public EnemyState
{
public:
    //  �R���X�g���N�^
    AttackState(EnemyThief* enemy) : EnemyState(enemy) {};
    //  �f�X�g���N�^
    ~AttackState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
private:
};

class PunchState : public EnemyState
{
public:
    //  �R���X�g���N�^
    PunchState(EnemyThief* enemy) : EnemyState(enemy) {};
    //  �f�X�g���N�^
    ~PunchState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class ShotState : public EnemyState
{
public:
    //  �R���X�g���N�^
    ShotState(EnemyThief* enemy) : EnemyState(enemy) {};
    //  �f�X�g���N�^
    ~ShotState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
    std::unique_ptr<AudioSource> Fire = nullptr;


};

class SearchState : public EnemyHierarchicalState
{
public:
    SearchState(EnemyThief* enemy) : EnemyHierarchicalState(enemy) {}
    //  �f�X�g���N�^
    ~SearchState();
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter();
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime);
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit();
};

class BattleState : public EnemyHierarchicalState
{
public:
    BattleState(EnemyThief* enemy) : EnemyHierarchicalState(enemy) {}
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
class RecieveState : public EnemyHierarchicalState
{
public:
    RecieveState(EnemyThief* enemy) : EnemyHierarchicalState(enemy) {}
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
class CalledState : public EnemyState
{
public:
    CalledState(EnemyThief* enemy) :EnemyState(enemy) {};
    ~CalledState() {}
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter() override;
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime) override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit() override;
};

//  �ҋ@�X�e�[�g�I�u�W�F�N�g
class StandbyState : public EnemyState
{
public:

    StandbyState(EnemyThief* enemy) : EnemyState(enemy) {};
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
};

// �_���[�W�X�e�[�g�I�u�W�F�N�g
class  DamageState : public EnemyState
{
public:
    DamageState(EnemyThief* enemy) : EnemyState(enemy) {};
    ~DamageState() {}
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter() override;
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime) override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit() override;
};

// ���S�X�e�[�g�I�u�W�F�N�g
class  DeadState : public EnemyState
{
public:
    DeadState(EnemyThief* enemy) : EnemyState(enemy) {};
    ~DeadState() {}
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter() override;
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime) override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit() override;
};
