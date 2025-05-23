#pragma once
#include "EnemyThief.h"
class EnemyThief;

class EnemyState
{
public:
    // コンストラクタ
    EnemyState(EnemyThief* enemy) : owner(enemy) {}
    virtual ~EnemyState() {}
    //全て継承先で実装させる必要があるため純粋仮想関数で実装
    //ステートに入った時のメソッド
    virtual void Enter() = 0;
    //ステートで実行するメソッド
    virtual void Execute(float elapsedTime) = 0;
    //ステートから出ていくときのメソッド
    virtual void Exit() = 0;

protected:
    EnemyThief* owner;

};

//  1層目のステートクラスの作成(StateBase.cppに関数定義をする)
#include <vector>
class EnemyHierarchicalState : public EnemyState
{
public:
    EnemyHierarchicalState(EnemyThief* enemy) : EnemyState(enemy) {}
    virtual ~EnemyHierarchicalState() {}
    //  全て継承先で実装させる必要があるため純粋仮想関数で実装
    //  ステートに入った時のメソッド
    virtual void Enter() = 0;
    //  ステートで実行するメソッド
    virtual void Execute(float elapsedTime) = 0;
    //  ステートから出ていくときのメソッド
    virtual void Exit() = 0;
    //  サブステートセット
    virtual void SetSubState(int newState);
    //  サブステート変更
    virtual void ChangeSubState(int newState);
    //  サブステート登録
    virtual void RegisterSubState(EnemyState* state);
    //  サブステート取得
    virtual EnemyState* GetSubState() { return subState; }
    //  サブステート番号取得
    virtual int GetSubStateIndex();

protected:
    EnemyState* subState = nullptr;
    std::vector<EnemyState*> subStatePool;
};
