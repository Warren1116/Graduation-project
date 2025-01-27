#include "StateMachine.h"

StateMachine::~StateMachine()
{
    //  �o�^�����X�e�[�g���폜����
    for (State* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

//  �X�V����
void StateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

//  �X�e�[�g�̐ݒ�
void StateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);

    currentState->Enter();
}

//  �X�e�[�g�̕ύX
void StateMachine::ChangeState(int newState)
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
int StateMachine::GetStateIndex()
{
    int i = 0;
    for (State* state : statePool)
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

void StateMachine::RegisterState(HierarchicalState* state)
{
    statePool.emplace_back(state);
}

void StateMachine::ChangeSubState(int newState)
{
    currentState->ChangeSubState(newState);
}

void StateMachine::RegisterSubState(int index, State* subState)
{
    statePool.at(index)->RegisterSubState(subState);
}

