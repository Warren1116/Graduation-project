#pragma once
#include <vector>
#include "StateBase.h"
class EnemyState;
class EnemyHierarchicalState;

class EnemyStateMachine
{
public:
    //  �R���X�g���N�^
    EnemyStateMachine() {}
    //  �f�X�g���N�^
    ~EnemyStateMachine();
    //  �X�V����
    void Update(float elapsedTime);
    //  �X�e�[�g�Z�b�g
    void SetState(int newState);
    //  �X�e�[�g�ύX
    void ChangeState(int newState);
    //  ���݂̃X�e�[�g�ԍ��擾
    int GetStateIndex();

    //  �X�e�[�g�o�^
    void RegisterState(EnemyHierarchicalState* state);
    //  2�w�ڃX�e�[�g�ύX
    void ChangeSubState(int newState);
    //  2�w�ڃX�e�[�g�o�^
    void RegisterSubState(int index, EnemyState* subState);
    EnemyHierarchicalState* GetState() { return currentState; }


private:
    //  ���݂̃X�e�[�g
    EnemyHierarchicalState* currentState = nullptr;
    //  �e�X�e�[�g��ێ�����z��
    std::vector<EnemyHierarchicalState*> statePool;


};
