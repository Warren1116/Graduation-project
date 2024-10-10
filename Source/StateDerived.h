#pragma once
#include "StateBase.h"

// TODO 02_02 �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public State
{
public:
    //  �R���X�g���N�^
    WanderState(EnemyPeople* enemy) : State(enemy) {};
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
    IdleState(EnemyPeople* enemy) : State(enemy) {};
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
    PursuitState(EnemyPeople* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~PursuitState() {}
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

class AttackState : public State
{
public:
    //  �R���X�g���N�^
    AttackState(EnemyPeople* enemy) : State(enemy) {};
    //  �f�X�g���N�^
    ~AttackState() {}
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
    SearchState(EnemyPeople* enemy) : HierarchicalState(enemy) {}
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
    BattleState(EnemyPeople* enemy) : HierarchicalState(enemy) {}
    //  �f�X�g���N�^
    ~BattleState();
    //  �X�e�[�g�ɓ��������̃��\�b�h
    void Enter();
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime);
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit();
};

// TODO_05_03 ���^AI���烁�b�Z�[�W����M�����Ƃ��ɌĂ΂��X�e�[�g��ǉ�
class RecieveState : public HierarchicalState
{
public:
    RecieveState(EnemyPeople* enemy) : HierarchicalState(enemy) {}
    ~RecieveState();
    // �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter();
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime);
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit();
};

// TODO_05_03 ���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
class CalledState : public State
{
public:
    CalledState(EnemyPeople* enemy) :State(enemy) {};
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
    StandbyState(EnemyPeople* enemy) : State(enemy) {};
    ~StandbyState() {}
    // �X�e�[�g�ɓ������Ƃ��̃��\�b�h
    void Enter() override;
    // �X�e�[�g�Ŏ��s����̃��\�b�h
    void Execute(float elapsedTime) override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit() override;
};