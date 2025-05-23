#pragma once
#include "EnemyThief.h"
class EnemyThief;

class EnemyState
{
public:
    // �R���X�g���N�^
    EnemyState(EnemyThief* enemy) : owner(enemy) {}
    virtual ~EnemyState() {}
    //�S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    //�X�e�[�g�ɓ��������̃��\�b�h
    virtual void Enter() = 0;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    virtual void Execute(float elapsedTime) = 0;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    virtual void Exit() = 0;

protected:
    EnemyThief* owner;

};

//  1�w�ڂ̃X�e�[�g�N���X�̍쐬(StateBase.cpp�Ɋ֐���`������)
#include <vector>
class EnemyHierarchicalState : public EnemyState
{
public:
    EnemyHierarchicalState(EnemyThief* enemy) : EnemyState(enemy) {}
    virtual ~EnemyHierarchicalState() {}
    //  �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    //  �X�e�[�g�ɓ��������̃��\�b�h
    virtual void Enter() = 0;
    //  �X�e�[�g�Ŏ��s���郁�\�b�h
    virtual void Execute(float elapsedTime) = 0;
    //  �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    virtual void Exit() = 0;
    //  �T�u�X�e�[�g�Z�b�g
    virtual void SetSubState(int newState);
    //  �T�u�X�e�[�g�ύX
    virtual void ChangeSubState(int newState);
    //  �T�u�X�e�[�g�o�^
    virtual void RegisterSubState(EnemyState* state);
    //  �T�u�X�e�[�g�擾
    virtual EnemyState* GetSubState() { return subState; }
    //  �T�u�X�e�[�g�ԍ��擾
    virtual int GetSubStateIndex();

protected:
    EnemyState* subState = nullptr;
    std::vector<EnemyState*> subStatePool;
};
