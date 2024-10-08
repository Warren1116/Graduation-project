#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"

// TODO 02_03 Stateを基底クラスとして各種Stateクラスを用意する。
// Wanderはサンプルとしてすでに記述済み
// 各種Enter関数の内容は各Transition○○State関数を参考に
// 各種Execute関数の内容は各Update○○State関数を参考に

//-------------//
// WanderState //
//-------------//
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
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Idle));
	}
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
	// プレイヤー索敵
	if (owner->SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Pursuit));
	}

}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{

}

//-----------//
// IdleState //
//-----------//
// 待機ステートに入った時のメソッド
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleNormal), true);

	// タイマーをランダムで設定
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// 待機ステートで実行するメソッド
void IdleState::Execute(float elapsedTime)
{
	float stateTimer = owner->GetStateTimer();
	stateTimer -= elapsedTime;
	owner->SetStateTimer(stateTimer);

	// 待機時間が経過したら徘徊ステートへ遷移
	if (owner->GetStateTimer() <= 0.0f)
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Wander));
	}

	// プレイヤーが見つかったら追跡ステートへ遷移
	if (owner->SearchPlayer())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Pursuit));
	}
}

// 待機ステートから出ていくときのメソッド
void IdleState::Exit()
{

}

//--------------//
// PursuitState //
//--------------//
// 追跡ステートに入った時のメソッド
void PursuitState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);

	// 数秒間追跡するタイマーをランダム設定
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// 追跡ステートで実行するメソッド
void PursuitState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
	owner->SetTargetPosition(Player::Instance().GetPosition());

	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 1.0);

	// タイマー処理
	float stateTimer = owner->GetStateTimer();
	stateTimer -= elapsedTime;
	owner->SetStateTimer(stateTimer);

	// 追跡時間が経過したとき待機ステートへ遷移
	if (owner->GetStateTimer() < 0.0f)
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Idle));
	}

	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// 攻撃範囲に入ったとき攻撃ステートへ遷移
	if (dist < owner->GetAttackRange())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::HandAttack));
	}
}

// 追跡メソッドから出ていくときのメソッド
void PursuitState::Exit()
{
	
}

//AttackIdle
//戦闘待機ステートに入った時のメソッド
//
void AttackIdleState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackIdle), false);

}

// 戦闘待機ステートで実行するメソッド
void AttackIdleState::Execute(float elapsedTime)
{
	// 離れ過ぎたら追跡ステート

	// 一定範囲内なら毒攻撃
	// 更に近ければジャンプ攻撃か追跡ステート
}

// 戦闘待機ステートから出ていくときのメソッド
void AttackIdleState::Exit()
{
}


//
// 毒攻撃に入った時のメソッド 
//
void GunState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::GunAttack), false);

}

// 毒攻撃ステートで実行するメソッド
void GunState::Execute(float elapsedTime)
{
	// モーションが終了したとき追跡ステートへ移行
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::AttackIdle));

	}
}

// 毒攻撃から出ていくときのメソッド
void GunState::Exit()
{
	
}

//
// 手刀攻撃に入った時のメソッド
//
void HandState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackIdle), false);

}

// 手刀攻撃ステートで実行するメソッド
void HandState::Execute(float elapsedTime)
{
	// モーションが終了したとき追跡ステートへ移行
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::AttackIdle));

	}
}

// 手刀攻撃から出ていくときのメソッド
void HandState::Exit()
{
}

//
// ジャンプ攻撃ステートに入った時のメソッド
//
void JumpState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::JumpAttack), false);
}

// ジャンプ攻撃で実行するメソッド
void JumpState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
	owner->SetTargetPosition(Player::Instance().GetPosition());

	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 1.0);


	// モーションが終了したとき追跡ステートへ移行
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::AttackIdle));

	}
}

// ジャンプ攻撃から出ていくときのメソッド
void JumpState::Exit()
{
}

