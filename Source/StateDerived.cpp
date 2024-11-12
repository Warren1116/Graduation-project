#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "MetaAI.h"

// Stateを基底クラスとして各種Stateクラスを用意する。
// Wanderはサンプルとしてすでに記述済み
// 各種Enter関数の内容は各Transition○○State関数を参考に
// 各種Execute関数の内容は各Update○○State関数を参考に

// 徘徊ステートに入った時のメソッド
void WanderState::Enter()
{
    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::WalkFWD), true);
}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
    // 目的地点までのXZ平面での距離判定
    DirectX::XMFLOAT3 position = owner->GetPosition();
    DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;

    // 目的地へ着いた
    float radius = owner->GetRadius();
    if (distSq < radius * radius)
    {
        // 待機ステートへ遷移
        // ChangeStateクラスで二層目のStateを切り替える
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Search::Idle));
    }
    // 目的地点へ移動
    owner->MoveToTarget(elapsedTime, 0.5f);
    // プレイヤー索敵
    if (owner->SearchPlayer())
    {
        // 見つかったら追跡ステートへ遷移
        // ChangeStateクラスで一層目のStateを切り替える
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }

}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{
    //書かなくてよい
}


// 待機ステートに入った時のメソッド
void IdleState::Enter()
{
    // 各種Enter関数の内容は各Transition○○State関数を参考に
    // タイマーをランダム設定
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleNormal), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// 待機ステートで実行するメソッド
void IdleState::Execute(float elapsedTime)
{
    // 各種Execute関数の内容は各Update○○State関数を参考に
    // タイマー処理
    owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

    // 待機時間が経過したとき徘徊ステートへ遷移
    if (owner->GetStateTimer() < 0.0f)
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Search::Wander));

    // プレイヤーが見つかったとき追跡ステートへ遷移
    if (owner->SearchPlayer())
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
}

// 待機ステートから出ていくときのメソッド
void IdleState::Exit()
{
    //書かなくてよい
}


// 追跡ステートに入った時のメソッド
void PursuitState::Enter()
{
    // 各種Enter関数の内容は各Transition○○State関数を参考に
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::WalkFWD), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

    // TODO 05_06 敵を発見したんで仲間を呼ぶ
    // エネミーからメタAIへ MsgCallHelp を送信する。
    Meta::Instance().SendMessaging(
        owner->GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgCallHelp);
    attackCooldownTimer = 1.0f;

}

// 追跡ステートで実行するメソッド
void PursuitState::Execute(float elapsedTime)
{
    // 各種Execute関数の内容は各Update○○State関数を参考に
    // 目標地点をプレイヤー位置に設定
    owner->SetTargetPosition(Player::Instance().GetPosition());

    // 目的地点へ移動
    owner->MoveToTarget(elapsedTime, 1.0);

    // タイマー処理
    owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

    // 追跡時間が経過したとき待機ステートへ遷移
    if (owner->GetStateTimer() < 0.0f)
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Search));

    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    //// 攻撃範囲に入ったとき攻撃ステートへ遷移
    if (owner->GetAttackRange() > dist)
    {
        attackCooldownTimer -= elapsedTime;
        if (attackCooldownTimer <= attackCooldownTimer)
        {
            Player::Instance().SetgetAttackSoon(true);
        }
        if (attackCooldownTimer <= 0)
        {
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Attack));
        }
    }
}

// 追跡ステートから出ていくときのメソッド
void PursuitState::Exit()
{
    //書かなくてよい
    Player::Instance().SetgetAttackSoon(false);
}

// 攻撃ステートに入った時のメソッド
void AttackState::Enter()
{
    // 攻撃権がなければ
    if (!owner->GetAttackFlg())
    {
        // 攻撃権をメタAIから求める
        // エネミーからメタAIへ MsgAskAttackRight を送信する
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgAskAttackRight);
    }
    // 攻撃権があればモーション再生開始
    if (owner->GetAttackFlg())
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Attack01), false);
        Player::Instance().ApplyDamage(1, 2.0f);
        CameraController::Instance().ShakeCamera(0.1f, 0.3f);
    }
}

// 攻撃ステートで実行するメソッド
void AttackState::Execute(float elapsedTime)
{
    // 攻撃権があるとき
    if (owner->GetAttackFlg())
    {
        // 攻撃モーションが終わっていれば追跡へ遷移
        if (!owner->GetModel()->IsPlayAnimation())
        {
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
        }
    }
    else
    {
        // 攻撃権がないときステート変更
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
    }
}

// 攻撃ステートから出ていくときのメソッド
void AttackState::Exit()
{
    if (owner->GetAttackFlg())
    {
        // TODO 05_07 攻撃が終わったとき攻撃権の放棄
        // 攻撃フラグをfalseに設定
        // エネミーからメタAIへ MsgChangeAttackRight を送信する
        owner->SetAttackFlg(false);
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgChangeAttackRight);
            Player::Instance().SetgetAttackSoon(false);
    }
}

// デストラクタ
SearchState::~SearchState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// サーチステートに入った時のメソッド
void SearchState::Enter()
{
    SetSubState(static_cast<int>(EnemyThief::Search::Idle));
}

// サーチステートで実行するメソッド
void SearchState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

// サーチステートから出ていくときのメソッド
void SearchState::Exit()
{

}

BattleState::~BattleState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

void BattleState::Enter()
{
    SetSubState(static_cast<int>(EnemyThief::Search::Wander));
}

void BattleState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

void BattleState::Exit()
{
}

// TODO 05_03 メタAIからメッセージを受信したときに呼ばれるステートを追加
// デストラクタ
RecieveState::~RecieveState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// データ受信したときのステート
void RecieveState::Enter()
{
    // 初期ステートを設定
    SetSubState(static_cast<int>(EnemyThief::Recieve::Called));
}

// サーチステートで実行するメソッド
void RecieveState::Execute(float elapsedTime)
{
    // 子ステート実行
    subState->Execute(elapsedTime);
    // 
    if (owner->SearchPlayer())
    {
        // Battleステートへ遷移
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }
}

// サーチステートから出ていくときのメソッド
void RecieveState::Exit()
{
}

// TODO 05_03 他のエネミーから呼ばれたときのステートを追加
void CalledState::Enter()
{
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::WalkBWD), true);
    owner->SetStateTimer(5.0f);
}

// コールドステートで実行するメソッド
void CalledState::Execute(float elapsedTime)
{
    // タイマー処理
    float timer = owner->GetStateTimer();
    timer -= elapsedTime;
    owner->SetStateTimer(timer);
    if (timer < 0.0f)
    {
        // 徘徊ステートへ遷移
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Search));
    }
    // 対象をプレイヤー地点に設定
    owner->SetTargetPosition(Player::Instance().GetPosition());
    owner->MoveToTarget(elapsedTime, 1.0f);
}

// コールドステートから出ていくときのメソッド
void CalledState::Exit()
{
}

// 戦闘待機ステートに入った時のメソッド
void StandbyState::Enter()
{
    Player::Instance().SetgetAttackSoon(false);
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleBattle), false);
    attackCooldownTimer = Mathf::RandomRange(3.5f, 5.0f);

}

// 戦闘待機ステートで実行するメソッド
void StandbyState::Execute(float elapsedTime)
{

    if (attackCooldownTimer <= attackWarningTime && !Player::Instance().GetAttackSoon()) {
        Player::Instance().SetgetAttackSoon(true);
    }
    // 攻撃権があるとき
    if (owner->GetAttackFlg())
    {
        attackCooldownTimer -= elapsedTime;
        if (attackCooldownTimer <= 0)
        {
            // 攻撃権があるときステート変更
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Attack));
        }
    }
    // 目標地点をプレイヤー位置に設定
    owner->SetTargetPosition(Player::Instance().GetPosition());
    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist > owner->GetAttackRange())
    {
        Player::Instance().SetgetAttackSoon(false);
        // 攻撃範囲から出たら追跡ステートへ遷移
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Pursuit));
    }
}

// 戦闘待機ステートから出ていくときのメソッド
void StandbyState::Exit()
{
    Player::Instance().SetgetAttackSoon(false);
}

