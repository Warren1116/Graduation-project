#pragma once
#include "StateBase.h"

// 徘徊ステートオブジェクト
class WanderState : public State
{
public:
    //  コンストラクタ
    WanderState(EnemyThief* enemy) : State(enemy) {};
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
    IdleState(EnemyThief* enemy) : State(enemy) {};
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
    PursuitState(EnemyThief* enemy) : State(enemy) {};
    //  デストラクタ
    ~PursuitState() {}
    //  ステートに入った時のメソッド
    void Enter()override;
    //ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    //ステートから出ていくときのメソッド
    void Exit()override;
private:
    float attackCooldownTimer;
    float attackWarningTime = 1.0f;

    enum class AttackType
    {
        Punch,
        Shot,
    };
};

class AttackState : public State
{
public:
    //  コンストラクタ
    AttackState(EnemyThief* enemy) : State(enemy) {};
    //  デストラクタ
    ~AttackState() {}
    //  ステートに入った時のメソッド
    void Enter()override;
    //ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    //ステートから出ていくときのメソッド
    void Exit()override;
};

class ShotState : public State
{
public:
    //  コンストラクタ
    ShotState(EnemyThief* enemy) : State(enemy) {};
    //  デストラクタ
    ~ShotState() {}
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
    SearchState(EnemyThief* enemy) : HierarchicalState(enemy) {}
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
    BattleState(EnemyThief* enemy) : HierarchicalState(enemy) {}
    //  デストラクタ
    ~BattleState();
    //  ステートに入った時のメソッド
    void Enter();
    //ステートで実行するメソッド
    void Execute(float elapsedTime);
    //ステートから出ていくときのメソッド
    void Exit();
};

// メタAIからメッセージを受信したときに呼ばれるステートを追加
class RecieveState : public HierarchicalState
{
public:
    RecieveState(EnemyThief* enemy) : HierarchicalState(enemy) {}
    ~RecieveState();
    // 全て継承先で実装させる必要があるため純粋仮想関数で実装
    // ステートに入ったときのメソッド
    void Enter();
    // ステートで実行するのメソッド
    void Execute(float elapsedTime);
    // ステートから出ていくときのメソッド
    void Exit();
};

// 他のエネミーから呼ばれたときのステートを追加
class CalledState : public State
{
public:
    CalledState(EnemyThief* enemy) :State(enemy) {};
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
    StandbyState(EnemyThief* enemy) : State(enemy) {};
    ~StandbyState() {}
    // ステートに入ったときのメソッド
    void Enter() override;
    // ステートで実行するのメソッド
    void Execute(float elapsedTime) override;
    // ステートから出ていくときのメソッド
    void Exit() override;
private:
    float attackCooldownTimer;
    float attackWarningTime = 1.0f;

    enum class AttackType
    {
        Punch,
        Shot,
    };
};

