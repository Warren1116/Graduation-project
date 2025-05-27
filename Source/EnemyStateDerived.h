#pragma once
#include "EnemyStateBase.h"
#include "Graphics\LineRenderer.h"
#include "Audio/Audio.h"

namespace EnemyStates
{
    // 徘徊ステートオブジェクト
    class WanderState : public EnemyState
    {
    public:
        //  コンストラクタ
        WanderState(EnemyThief* enemy) : EnemyState(enemy) {};
        //  デストラクタ
        ~WanderState() {}
        //  ステートに入った時のメソッド
        void Enter()override;
        //ステートで実行するメソッド
        void Execute(float elapsedTime)override;
        //ステートから出ていくときのメソッド
        void Exit()override;
    };

    // 待機ステートオブジェクト
    class IdleState : public EnemyState
    {
    public:
        //  コンストラクタ
        IdleState(EnemyThief* enemy) : EnemyState(enemy) {};
        //  デストラクタ
        ~IdleState() {}
        //  ステートに入った時のメソッド
        void Enter()override;
        //ステートで実行するメソッド
        void Execute(float elapsedTime)override;
        //ステートから出ていくときのメソッド
        void Exit()override;
    };

    // 追跡ステートオブジェクト
    class PursuitState : public EnemyState
    {
    public:
        //  コンストラクタ
        PursuitState(EnemyThief* enemy) : EnemyState(enemy) {};
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


    };

    // 攻撃ステートオブジェクト
    class AttackState : public EnemyState
    {
    public:
        //  コンストラクタ
        AttackState(EnemyThief* enemy) : EnemyState(enemy) {};
        //  デストラクタ
        ~AttackState() {}
        //  ステートに入った時のメソッド
        void Enter()override;
        //ステートで実行するメソッド
        void Execute(float elapsedTime)override;
        //ステートから出ていくときのメソッド
        void Exit()override;
    private:
    };

    // 攻撃ステートの子オブジェクト（パンチ）
    class PunchState : public EnemyState
    {
    public:
        //  コンストラクタ
        PunchState(EnemyThief* enemy) : EnemyState(enemy) {};
        //  デストラクタ
        ~PunchState() {}
        //  ステートに入った時のメソッド
        void Enter()override;
        //ステートで実行するメソッド
        void Execute(float elapsedTime)override;
        //ステートから出ていくときのメソッド
        void Exit()override;
    };

    // 攻撃ステートの子オブジェクト（シュート）
    class ShotState : public EnemyState
    {
    public:
        //  コンストラクタ
        ShotState(EnemyThief* enemy) : EnemyState(enemy) {};
        //  デストラクタ
        ~ShotState() {}
        //  ステートに入った時のメソッド
        void Enter()override;
        //ステートで実行するメソッド
        void Execute(float elapsedTime)override;
        //ステートから出ていくときのメソッド
        void Exit()override;
        std::unique_ptr<AudioSource> Fire = nullptr;


    };

    // 階層型ステートマシンの親ステートクラス
    // 索敵ステート
    class SearchState : public EnemyHierarchicalState
    {
    public:
        SearchState(EnemyThief* enemy) : EnemyHierarchicalState(enemy) {}
        //  デストラクタ
        ~SearchState();
        //  ステートに入った時のメソッド
        void Enter();
        //ステートで実行するメソッド
        void Execute(float elapsedTime);
        //ステートから出ていくときのメソッド
        void Exit();
    };

    // 戦闘ステート
    class BattleState : public EnemyHierarchicalState
    {
    public:
        BattleState(EnemyThief* enemy) : EnemyHierarchicalState(enemy) {}
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
    class RecieveState : public EnemyHierarchicalState
    {
    public:
        RecieveState(EnemyThief* enemy) : EnemyHierarchicalState(enemy) {}
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
    class CalledState : public EnemyState
    {
    public:
        CalledState(EnemyThief* enemy) :EnemyState(enemy) {};
        ~CalledState() {}
        // ステートに入ったときのメソッド
        void Enter() override;
        // ステートで実行するのメソッド
        void Execute(float elapsedTime) override;
        // ステートから出ていくときのメソッド
        void Exit() override;
    };

    //  待機ステートオブジェクト
    class StandbyState : public EnemyState
    {
    public:

        StandbyState(EnemyThief* enemy) : EnemyState(enemy) {};
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
    };

    // ダメージステートオブジェクト
    class  DamageState : public EnemyState
    {
    public:
        DamageState(EnemyThief* enemy) : EnemyState(enemy) {};
        ~DamageState() {}
        // ステートに入ったときのメソッド
        void Enter() override;
        // ステートで実行するのメソッド
        void Execute(float elapsedTime) override;
        // ステートから出ていくときのメソッド
        void Exit() override;
    };

    // 死亡ステートオブジェクト
    class  DeadState : public EnemyState
    {
    public:
        DeadState(EnemyThief* enemy) : EnemyState(enemy) {};
        ~DeadState() {}
        // ステートに入ったときのメソッド
        void Enter() override;
        // ステートで実行するのメソッド
        void Execute(float elapsedTime) override;
        // ステートから出ていくときのメソッド
        void Exit() override;
    };
}
