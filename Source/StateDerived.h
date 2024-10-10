#pragma once
#include "StateBase.h"

// TODO 02_02 徘徊ステートオブジェクト
class WanderState : public State
{
public:
    //  コンストラクタ
    WanderState(EnemyPeople* enemy) : State(enemy) {};
    //  デストラクタ
    ~WanderState() {}
    //  ステートに入った時のメソッド
    void Enter()override;
    //ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    //ステートから出ていくときのメソッド
    void Exit()override;
};

class IdleState : public State
{
public:
    //  コンストラクタ
    IdleState(EnemyPeople* enemy) : State(enemy) {};
    //  デストラクタ
    ~IdleState() {}
    //  ステートに入った時のメソッド
    void Enter()override;
    //ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    //ステートから出ていくときのメソッド
    void Exit()override;
};

class PursuitState : public State
{
public:
    //  コンストラクタ
    PursuitState(EnemyPeople* enemy) : State(enemy) {};
    //  デストラクタ
    ~PursuitState() {}
    //  ステートに入った時のメソッド
    void Enter()override;
    //ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    //ステートから出ていくときのメソッド
    void Exit()override;
};

class AttackState : public State
{
public:
    //  コンストラクタ
    AttackState(EnemyPeople* enemy) : State(enemy) {};
    //  デストラクタ
    ~AttackState() {}
    //  ステートに入った時のメソッド
    void Enter()override;
    //ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    //ステートから出ていくときのメソッド
    void Exit()override;
};

class SearchState : public HierarchicalState
{
public:
    SearchState(EnemyPeople* enemy) : HierarchicalState(enemy) {}
    //  デストラクタ
    ~SearchState();
    //  ステートに入った時のメソッド
    void Enter();
    //ステートで実行するメソッド
    void Execute(float elapsedTime);
    //ステートから出ていくときのメソッド
    void Exit();
};

class BattleState : public HierarchicalState
{
public:
    BattleState(EnemyPeople* enemy) : HierarchicalState(enemy) {}
    //  デストラクタ
    ~BattleState();
    //  ステートに入った時のメソッド
    void Enter();
    //ステートで実行するメソッド
    void Execute(float elapsedTime);
    //ステートから出ていくときのメソッド
    void Exit();
};

// TODO_05_03 メタAIからメッセージを受信したときに呼ばれるステートを追加
class RecieveState : public HierarchicalState
{
public:
    RecieveState(EnemyPeople* enemy) : HierarchicalState(enemy) {}
    ~RecieveState();
    // 全て継承先で実装させる必要があるため純粋仮想関数で実装
    // ステートに入ったときのメソッド
    void Enter();
    // ステートで実行するのメソッド
    void Execute(float elapsedTime);
    // ステートから出ていくときのメソッド
    void Exit();
};

// TODO_05_03 他のエネミーから呼ばれたときのステートを追加
class CalledState : public State
{
public:
    CalledState(EnemyPeople* enemy) :State(enemy) {};
    ~CalledState() {}
    // ステートに入ったときのメソッド
    void Enter() override;
    // ステートで実行するのメソッド
    void Execute(float elapsedTime) override;
    // ステートから出ていくときのメソッド
    void Exit() override;
};

//  待機ステートオブジェクト
class StandbyState : public State
{
public:
    StandbyState(EnemyPeople* enemy) : State(enemy) {};
    ~StandbyState() {}
    // ステートに入ったときのメソッド
    void Enter() override;
    // ステートで実行するのメソッド
    void Execute(float elapsedTime) override;
    // ステートから出ていくときのメソッド
    void Exit() override;
};