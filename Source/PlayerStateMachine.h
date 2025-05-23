#pragma once
#include <vector>
#include "PlayerStateDerived.h"
class PlayerState;

class PlayerStateMachine
{
public:
    PlayerStateMachine() {}
    ~PlayerStateMachine();

    void Update(float elapsedTime);
    void SetState(int newState);
    void ChangeState(int newState);
    int GetStateIndex();
    //  �X�e�[�g�o�^
    void RegisterState(PlayerState* state);

private:
    PlayerState* currentState = nullptr;
    std::vector<PlayerState*> statePool;
};
