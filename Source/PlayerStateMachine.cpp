#include "PlayerStateMachine.h"

PlayerStateMachine::~PlayerStateMachine()
{
    for (PlayerState* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

void PlayerStateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

void PlayerStateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);
    currentState->Enter();
}

void PlayerStateMachine::ChangeState(int newState)
{
    if (newState >= 0 && newState < statePool.size())
    {
        if (currentState != nullptr)
        {
            currentState->Exit();
        }
        currentState = statePool.at(newState);
        currentState->Enter();
    }
}

int PlayerStateMachine::GetStateIndex()
{
    int i = 0;
    for (PlayerState* state : statePool)
    {
        if (state == currentState)
        {
            return i;
        }
        ++i;
    }
    return -1;
}

void PlayerStateMachine::RegisterState(PlayerState* state)
{
    statePool.emplace_back(state);
}
