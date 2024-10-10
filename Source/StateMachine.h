#pragma once
#include <vector>
#include "StateBase.h"
#define USESTATEMACHINE3
class State;
class HierarchicalState;

class StateMachine
{
public:
    //  コンストラクタ
    StateMachine() {}
    //  デストラクタ
    ~StateMachine();
    //  更新処理
    void Update(float elapsedTime);
    //  ステートセット
    void SetState(int newState);
    //  ステート変更
    void ChangeState(int newState);
    //  現在のステート番号取得
    int GetStateIndex();

#ifdef USESTATEMACHINE3

    //  ステート登録
    void RegisterState(HierarchicalState* state);
    //  2層目ステート変更
    void ChangeSubState(int newState);
    //  2層目ステート登録
    void RegisterSubState(int index, State* subState);
    HierarchicalState* GetState() { return currentState; }

#endif // USESTATEMACHINE3

private:

#ifdef USESTATEMACHINE3

    //  現在のステート
    HierarchicalState* currentState = nullptr;
    //  各ステートを保持する配列
    std::vector<HierarchicalState*> statePool;

#endif // USESTATEMACHINE3

};
