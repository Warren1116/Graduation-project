#include "StateMachine.h"

StateMachine::~StateMachine()
{
    //  登録したステートを削除する
    for (State* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

//  更新処理
void StateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

//  ステートの設定
void StateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);

    currentState->Enter();
}

//  ステートの変更
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

//  ステートのインデックスを取得
int StateMachine::GetStateIndex()
{
    int i = 0;
    for (State* state : statePool)
    {
        if (state == currentState)
        {
            //  i番号目のステートをリターン
            return i;
        }
        ++i;
    }
    //  ステートが見つからなかった時
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

