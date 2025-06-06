#include "EnemyStateDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "MetaAI.h"
#include "SceneGame.h"



//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          親SearchState
//-----------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
EnemyStates::SearchState::~SearchState()
{
    for (EnemyState* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// サーチステートに入った時のメソッド
void EnemyStates::SearchState::Enter()
{
    SetSubState(static_cast<int>(EnemyThief::Search::Idle));
}

// サーチステートで実行するメソッド
void EnemyStates::SearchState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

// サーチステートから出ていくときのメソッド
void EnemyStates::SearchState::Exit()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子BattleState（SearchState）
//----------------------------------------------------------------------------------------------------------------------------------------
// 徘徊ステートに入った時のメソッド
void EnemyStates::WanderState::Enter()
{
    if (owner->GetHealth() <= 0)
        return;

    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);
}

// 徘徊ステートで実行するメソッド
void EnemyStates::WanderState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)
        return;

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
void EnemyStates::WanderState::Exit()
{
    //書かなくてよい
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　子idleState（SearchState）
//-----------------------------------------------------------------------------------------------------------------------------------------
// 待機ステートに入った時のメソッド
void EnemyStates::IdleState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    // タイマーをランダム設定
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Idle), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// 待機ステートで実行するメソッド
void EnemyStates::IdleState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    // タイマー処理
    owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

    // 待機時間が経過したとき徘徊ステートへ遷移
    if (owner->GetStateTimer() < 0.0f)
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Search::Wander));

    // プレイヤーが見つかったとき追跡ステートへ遷移
    if (owner->SearchPlayer())
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }
}

// 待機ステートから出ていくときのメソッド
void EnemyStates::IdleState::Exit()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          親BattleState
//-----------------------------------------------------------------------------------------------------------------------------------------
EnemyStates::BattleState::~BattleState()
{
    for (EnemyState* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

void EnemyStates::BattleState::Enter()
{
    if (owner->GetHealth() <= 0)
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Dead));
        return;
    }
    SetSubState(static_cast<int>(EnemyThief::Battle::Pursuit));
}

void EnemyStates::BattleState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

void EnemyStates::BattleState::Exit()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子PursuitState（BattleState）
//-----------------------------------------------------------------------------------------------------------------------------------------
// 追跡ステートに入った時のメソッド
void EnemyStates::PursuitState::Enter()
{
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

    // 敵を発見したんで仲間を呼ぶ
    // エネミーからメタAIへ MsgCallHelp を送信する。
    Meta::Instance().SendMessaging(
        owner->GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgCallHelp);

}

// 追跡ステートで実行するメソッド
void EnemyStates::PursuitState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Dead));
        return;
    }
    if (!owner->IsGetThrow())
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
    if (owner->GetAttackRange() * 0.5f > dist)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
    }
}

// 追跡ステートから出ていくときのメソッド
void EnemyStates::PursuitState::Exit()
{
    Player::Instance().SetgetAttackSoon(false);
    Player::Instance().SetgetShotSoon(false);
}
//----------------------------------------------------------------------------------------------------------------------------------------- 
//                                                          子AttackState（BattleState）
// -----------------------------------------------------------------------------------------------------------------------------------------
// 攻撃ステートに入った時のメソッド
void EnemyStates::AttackState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    // 攻撃タイプをランダムで決定
    // パンチ攻撃
    if (owner->randomType == EnemyThief::AttackType::Punch)
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);

    }
    // シュート攻撃
    if (owner->randomType == EnemyThief::AttackType::Shot)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Shot));
    }

}

// 攻撃ステートで実行するメソッド
void EnemyStates::AttackState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

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
void EnemyStates::AttackState::Exit()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子PunchState（BattleState）
//-----------------------------------------------------------------------------------------------------------------------------------------
// パンチステートに入った時のメソッド
void EnemyStates::PunchState::Enter()
{
    if (owner->GetHealth() <= 0)return;

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
        Player::Instance().SetgetAttackSoon(false);
        Player::Instance().ApplyDamage(12, 2.0f);
    }
}

// パンチステートで実行するメソッド
void EnemyStates::PunchState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

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
void EnemyStates::PunchState::Exit()
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
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子ShotState（BattleState）
//-----------------------------------------------------------------------------------------------------------------------------------------
// シュートステートに入った時のメソッド
void EnemyStates::ShotState::Enter()
{
    Fire = Audio::Instance().LoadAudioSource("Data/Audio/Fire.wav");

    if (owner->GetHealth() <= 0)return;

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

        // プレイヤーに向けて弾を発射
        DirectX::XMFLOAT3 playerPosition = Player::Instance().GetPosition();
        DirectX::XMFLOAT3 ownerPosition = owner->GetPosition();
        DirectX::XMVECTOR toPlayer = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPosition), DirectX::XMLoadFloat3(&ownerPosition));
        toPlayer = DirectX::XMVector3Normalize(toPlayer);

        // 新しい向きを計算
        float angleY = atan2f(DirectX::XMVectorGetX(toPlayer), DirectX::XMVectorGetZ(toPlayer));
        owner->SetAngle(DirectX::XMFLOAT3(0, angleY, 0));

        Fire->Play(false);
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackShot), false);
        Player::Instance().SetgetAttackSoon(false);
        Player::Instance().ApplyDamage(8, 2.0f);
    }
}

// シュートステートで実行するメソッド
void EnemyStates::ShotState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

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
void EnemyStates::ShotState::Exit()
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
        Player::Instance().SetgetShotSoon(false);

    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子StandbyState（BattleState）
// -----------------------------------------------------------------------------------------------------------------------------------------
// 戦闘待機ステートに入った時のメソッド
void EnemyStates::StandbyState::Enter()
{
    if (owner->GetHealth() <= 0)return;

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

    //　攻撃をランダムに設定する
    owner->randomType = (static_cast<EnemyThief::AttackType>(Mathf::RandomRange(1, 2)));

    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::HoldGun), false);
    attackCooldownTimer = Mathf::RandomRange(2.5f, 4.0f);

}

// 戦闘待機ステートで実行するメソッド
void EnemyStates::StandbyState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    // プレイヤーに向けて
    DirectX::XMFLOAT3 playerPosition = Player::Instance().GetPosition();
    DirectX::XMFLOAT3 ownerPosition = owner->GetPosition();
    DirectX::XMVECTOR toPlayer = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPosition), DirectX::XMLoadFloat3(&ownerPosition));
    toPlayer = DirectX::XMVector3Normalize(toPlayer);
    // 新しい向きを計算
    float angleY = atan2f(DirectX::XMVectorGetX(toPlayer), DirectX::XMVectorGetZ(toPlayer));
    owner->SetAngle(DirectX::XMFLOAT3(0, angleY, 0));

    // 攻撃権があるとき
    if (attackCooldownTimer <= attackWarningTime && !Player::Instance().GetAttackSoon())
    {
        Player::Instance().SetgetAttackSoon(true);
        if (owner->randomType == EnemyThief::AttackType::Shot)
        {
            Player::Instance().SetgetShotSoon(true);

        }

        Player::Instance().SetAttackEnemy(owner);
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
        // 攻撃範囲から出たら追跡ステートへ遷移
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Pursuit));
    }
}

// 戦闘待機ステートから出ていくときのメソッド
void EnemyStates::StandbyState::Exit()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子DamageState（BattleState）
//-----------------------------------------------------------------------------------------------------------------------------------------
// ダメージステートに入った時のメソッド
void EnemyStates::DamageState::Enter()
{
    //　PlayerのSwingKickに攻撃される時
    if (Player::Instance().GetIsUseSwingKick() && owner->GetIsLockedOn())
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::KickDown), false);
    }
    else
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::GetHit), false);
    }
}


// ダメージステートで実行するメソッド
void EnemyStates::DamageState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation() && owner->GetHealth() > 0)
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }

}
// ダメージステートから出ていくときのメソッド
void EnemyStates::DamageState::Exit()
{

}

//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          親RecieveState
//-----------------------------------------------------------------------------------------------------------------------------------------
// メタAIからメッセージを受信したときに呼ばれるステートを追加
// デストラクタ
EnemyStates::RecieveState::~RecieveState()
{
    for (EnemyState* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// データ受信したときのステート
void EnemyStates::RecieveState::Enter()
{
    // 初期ステートを設定
    SetSubState(static_cast<int>(EnemyThief::Recieve::Called));
}

// サーチステートで実行するメソッド
void EnemyStates::RecieveState::Execute(float elapsedTime)
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
void EnemyStates::RecieveState::Exit()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          子CalledState（RecieveState）
//-----------------------------------------------------------------------------------------------------------------------------------------
// 他のエネミーから呼ばれたときのステートを追加
void EnemyStates::CalledState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(5.0f);
}

// コールドステートで実行するメソッド
void EnemyStates::CalledState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)
        return;

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
void EnemyStates::CalledState::Exit()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------
