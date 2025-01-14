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
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);
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
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Idle), true);
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
    if (owner->GetHealth() <= 0)return;
    // 各種Enter関数の内容は各Transition○○State関数を参考に
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

    // TODO 05_06 敵を発見したんで仲間を呼ぶ
    // エネミーからメタAIへ MsgCallHelp を送信する。
    Meta::Instance().SendMessaging(
        owner->GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgCallHelp);

}

// 追跡ステートで実行するメソッド
void PursuitState::Execute(float elapsedTime)
{

    if (!owner->IsThrown())
    {
        // 各種Execute関数の内容は各Update○○State関数を参考に
        // 目標地点をプレイヤー位置に設定
        owner->SetTargetPosition(Player::Instance().GetPosition());

    }

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
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
    }
}

// 追跡ステートから出ていくときのメソッド
void PursuitState::Exit()
{
    Player::Instance().SetgetAttackSoon(false);
}

// 攻撃ステートに入った時のメソッド
void AttackState::Enter()
{
    randomType = static_cast<AttackType>(Mathf::RandomRange(1, 2));

    if (randomType == AttackType::Punch)
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);

    }

    if (randomType == AttackType::Shot)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Shot));
    }
}

// 攻撃ステートで実行するメソッド
void AttackState::Execute(float elapsedTime)
{
    owner->SetTargetPosition(Player::Instance().GetPosition());

    // 目的地点へ移動
    owner->MoveToTarget(elapsedTime, 1.0);

    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    //// 攻撃範囲に入ったとき攻撃ステートへ遷移
    if (owner->GetPunchRange() > dist)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Punch));
        return;
    }


}

// 攻撃ステートから出ていくときのメソッド
void AttackState::Exit()
{

}


// パンチステートに入った時のメソッド
void PunchState::Enter()
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
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackPunch), false);
        Player::Instance().ApplyDamage(1, 2.0f);
        //MessageData::CAMERASHAKEDATA	p = { 0.1f, 2.0f };
        //Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
    }
}

// パンチステートで実行するメソッド
void PunchState::Execute(float elapsedTime)
{
    owner->SetTargetPosition(Player::Instance().GetPosition());

    // 目的地点へ移動
    owner->MoveToTarget(elapsedTime, 1.0);

    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    // 攻撃権があるとき
    if (owner->GetAttackFlg())
    {
        if (!owner->GetModel()->IsPlayAnimation())
        {
            // 攻撃モーションが終わっていれば追跡へ遷移
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
            return;
        }
    }
    else
    {
        // 攻撃権がないときステート変更
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
        return;
    }

}
// パンチステートから出ていくときのメソッド
void PunchState::Exit()
{
    if (owner->GetAttackFlg())
    {
        // 攻撃が終わったとき攻撃権の放棄
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



// シュートステートに入った時のメソッド
void ShotState::Enter()
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
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackShot), false);
        Player::Instance().ApplyDamage(1, 2.0f);
        /*CameraController::Instance().ShakeCamera(0.1f, 0.3f);*/
    }
}

// シュートステートで実行するメソッド
void ShotState::Execute(float elapsedTime)
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

// シュートステートから出ていくときのメソッド
void ShotState::Exit()
{
    if (owner->GetAttackFlg())
    {
        // 攻撃が終わったとき攻撃権の放棄
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
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
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

    Player::Instance().SetgetAttackSoon(false);
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::HoldGun), false);
    attackCooldownTimer = Mathf::RandomRange(2.5f, 4.0f);

}

// 戦闘待機ステートで実行するメソッド
void StandbyState::Execute(float elapsedTime)
{
    //AttackType randomType = static_cast<AttackType>(Mathf::RandomRange(1, 2));
    //randomType = static_cast<AttackType>(2);

    if (attackCooldownTimer <= attackWarningTime && !Player::Instance().GetAttackSoon()) {
        Player::Instance().SetgetAttackSoon(true);
    }

    // 攻撃権があるとき
    if (owner->GetAttackFlg())
    {
        attackCooldownTimer -= elapsedTime;
        if (attackCooldownTimer <= 0)
        {
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

