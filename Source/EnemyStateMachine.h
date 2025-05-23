#pragma once
#include <vector>
#include "StateBase.h"
class EnemyState;
class EnemyHierarchicalState;

class EnemyStateMachine
{
public:
    //  コンストラクタ
    EnemyStateMachine() {}
    //  デストラクタ
    ~EnemyStateMachine();
    //  更新処理
    void Update(float elapsedTime);
    //  ステートセット
    void SetState(int newState);
    //  ステート変更
    void ChangeState(int newState);
    //  現在のステート番号取得
    int GetStateIndex();

    //  ステート登録
    void RegisterState(EnemyHierarchicalState* state);
    //  2層目ステート変更
    void ChangeSubState(int newState);
    //  2層目ステート登録
    void RegisterSubState(int index, EnemyState* subState);
    EnemyHierarchicalState* GetState() { return currentState; }


private:
    //  現在のステート
    EnemyHierarchicalState* currentState = nullptr;
    //  各ステートを保持する配列
    std::vector<EnemyHierarchicalState*> statePool;


};
