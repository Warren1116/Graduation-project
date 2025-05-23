#include "EnemyStateMachine.h"

EnemyStateMachine::~EnemyStateMachine()
{
    //  �o�^�����X�e�[�g���폜����
    for (EnemyState* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

//  �X�V����
void EnemyStateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

//  �X�e�[�g�̐ݒ�
void EnemyStateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);

    currentState->Enter();
}

//  �X�e�[�g�̕ύX
void EnemyStateMachine::ChangeState(int newState)
{
    if (newState >= 0 && newState < statePool.size())
    {
        if (currentState != nullptr)
        {
            if (currentState->GetSubState() != nullptr)
            {
                currentState->GetSubState()->Exit();
            }
            currentState->Exit();
        }
        currentState = statePool.at(newState);
        currentState->Enter();
    }
}

//  �X�e�[�g�̃C���f�b�N�X���擾
int EnemyStateMachine::GetStateIndex()
{
    int i = 0;
    for (EnemyState* state : statePool)
    {
        if (state == currentState)
        {
            //  i�ԍ��ڂ̃X�e�[�g�����^�[��
            return i;
        }
        ++i;
    }
    //  �X�e�[�g��������Ȃ�������
    return -1;
}

void EnemyStateMachine::RegisterState(EnemyHierarchicalState* state)
{
    statePool.emplace_back(state);
}

void EnemyStateMachine::ChangeSubState(int newState)
{
    currentState->ChangeSubState(newState);
}

void EnemyStateMachine::RegisterSubState(int index, EnemyState* subState)
{
    statePool.at(index)->RegisterSubState(subState);
}

