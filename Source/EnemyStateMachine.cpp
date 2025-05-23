#include "EnemyStateMachine.h"

EnemyStateMachine::~EnemyStateMachine()
{
    //  登録したステートを削除する
    for (EnemyState* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

//  更新処理
void EnemyStateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

//  ステートの設定
void EnemyStateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);

    currentState->Enter();
}

//  ステートの変更
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

//  ステートのインデックスを取得
int EnemyStateMachine::GetStateIndex()
{
    int i = 0;
    for (EnemyState* state : statePool)
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

