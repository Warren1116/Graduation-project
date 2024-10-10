//#include "ActionDerived.h"
//#include "EnemyBlueSlime.h"
//#include "Player.h"
//#include "Mathf.h"
//
//
//// 攻撃行動
//ActionBase::State AttackAction::Run(float elapsedTime)
//{
//	switch (step)
//	{
//	case 0:
//		// 目標地点をプレイヤー位置に設定
//		owner->SetTargetPosition(Player::Instance().GetPosition());
//		// アニメーション再生
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Attack01), false);
//		step++;
//		break;
//	case 1:
//
//		// アニメーションが終了しているとき
//		if (!owner->GetModel()->IsPlayAnimation())
//		{
//			step = 0;
//			// 攻撃成功を返す
//			return ActionBase::State::Complete;
//		}
//		break;
//	}
//	// スキル中を返す
//	return ActionBase::State::Run;
//}
//// スキル攻撃行動
//ActionBase::State SkillAction::Run(float elapsedTime)
//{
//	switch (step)
//	{
//	case 0:
//		// 目標地点をプレイヤー位置に設定
//		owner->SetTargetPosition(Player::Instance().GetPosition());
//		// アニメーション再生
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Attack02), false);
//		step++;
//	case 1:
//		// アニメーションが終了しているとき
//		if (!owner->GetModel()->IsPlayAnimation())
//		{
//			step = 0;
//			// 攻撃成功を返す
//			return ActionBase::State::Complete;
//		}
//		break;
//	}
//	// スキル中を返す
//	return ActionBase::State::Run;
//}
//
//// 徘徊行動
//ActionBase::State WanderAction::Run(float elapsedTime)
//{
//	switch (step)
//	{
//	case 0:
//		// 徘徊モーション設定
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::WalkFWD), true);
//		step++;
//		break;
//	case 1:
//		// 目的地点までのXZ平面での距離判定
//		DirectX::XMFLOAT3 position = owner->GetPosition();
//		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
//		float vx = targetPosition.x - position.x;
//		float vz = targetPosition.z - position.z;
//		float distSq = vx * vx + vz * vz;
//
//		// 目的地へ着いた
//		float radius = owner->GetRadius();
//		if (distSq < radius * radius)
//		{
//			step = 0;
//			// 徘徊成功を返す
//			return ActionBase::State::Complete;
//		}
//
//		// 目的地点へ移動
//		owner->MoveToTarget(elapsedTime, 0.5f);
//
//		// プレイヤー索敵成功したら
//		if (owner->SearchPlayer())
//		{
//			step = 0;
//			// 徘徊成功を返す
//			return ActionBase::State::Complete;
//		}
//		break;
//	}
//	// 実行中
//	return ActionBase::State::Run;
//}
//
//// 追跡行動
//ActionBase::State PursuitAction::Run(float elapsedTime)
//{
//	float runTimer = owner->GetRunTimer();
//	switch (step)
//	{
//	case 0:
//		// 目標地点をプレイヤー位置に設定
//		owner->SetTargetPosition(Player::Instance().GetPosition());
//		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);
//		step++;
//		break;
//	case 1:
//		runTimer -= elapsedTime;
//		// タイマー更新
//		owner->SetRunTimer(runTimer);
//		// 目標地点をプレイヤー位置に設定
//		owner->SetTargetPosition(Player::Instance().GetPosition());
//		// 目的地点へ移動
//		owner->MoveToTarget(elapsedTime, 1.0);
//
//		// プレイヤーとの距離を計算
//		DirectX::XMFLOAT3 position = owner->GetPosition();
//		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
//
//		float vx = targetPosition.x - position.x;
//		float vy = targetPosition.y - position.y;
//		float vz = targetPosition.z - position.z;
//		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
//		// 攻撃範囲にいるとき
//		if (dist < owner->GetAttackRange())
//		{
//			step = 0;
//			// 追跡成功を返す
//			return ActionBase::State::Complete;
//		}
//		// 行動時間が過ぎた時
//		if (runTimer <= 0.0f)
//		{
//			step = 0;
//			// 追跡失敗を返す
//			return ActionBase::State::Failed;
//		}
//		break;
//	}
//	return ActionBase::State::Run;
//}
//
//// 待機行動
//ActionBase::State IdleAction::Run(float elapsedTime)
//{
//	float runTimer = owner->GetRunTimer();
//	switch (step)
//	{
//	case 0:
//		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::IdleNormal), true);
//		step++;
//		break;
//	case 1:
//		runTimer -= elapsedTime;
//		// タイマー更新
//		owner->SetRunTimer(runTimer);
//
//		// 待機時間が過ぎた時
//		if (runTimer <= 0.0f)
//		{
//			owner->SetRandomTargetPosition();
//			step = 0;
//			return ActionBase::State::Complete;
//		}
//
//		// プレイヤーを見つけた時
//		if (owner->SearchPlayer())
//		{
//			step = 0;
//			return ActionBase::State::Complete;
//		}
//		break;
//	}
//	return ActionBase::State::Run;
//}
//
//// 逃走行動
//ActionBase::State LeaveAction::Run(float elapsedTime)
//{
//	DirectX::XMFLOAT3 targetPosition;
//	switch (step)
//	{
//	case 0:
//		// 目標地点をプレイヤーと正反対のベクトル×5の位置に指定
//		DirectX::XMVECTOR startPosition = DirectX::XMLoadFloat3(&Player::Instance().GetPosition());
//		DirectX::XMVECTOR endPosition = DirectX::XMLoadFloat3(&owner->GetPosition());
//
//		DirectX::XMVECTOR TargetPosition = DirectX::XMVectorSubtract(endPosition, startPosition);
//		TargetPosition = DirectX::XMVector3Normalize(TargetPosition);
//		TargetPosition = DirectX::XMVectorScale(TargetPosition, 5.0f);
//
//
//		DirectX::XMStoreFloat3(&targetPosition, TargetPosition);
//		targetPosition.x += owner->GetPosition().x;
//		targetPosition.y += owner->GetPosition().y;
//		targetPosition.z += owner->GetPosition().z;
//		owner->SetTargetPosition(targetPosition);
//
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);
//		step++;
//		break;
//	case 1:
//
//		targetPosition = owner->GetTargetPosition();
//		// 目的地点へ移動
//		owner->MoveToTarget(elapsedTime, 1.0);
//
//		DirectX::XMFLOAT3 position = owner->GetPosition();
//		targetPosition = owner->GetTargetPosition();
//
//		float vx = targetPosition.x - position.x;
//		float vz = targetPosition.z - position.z;
//		float distSq = vx * vx + vz * vz;
//
//		// 目的地へ着いた
//		float radius = owner->GetRadius();
//		if (distSq < radius * radius)
//		{
//			step = 0;
//			return ActionBase::State::Complete;
//		}
//
//		break;
//	}
//
//	return ActionBase::State::Run;
//}
//// 回復行動
//ActionBase::State RecoverAction::Run(float elapsedTime)
//{
//	switch (step)
//	{
//	case 0:
//		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Taunt), false);
//		step++;
//		break;
//	case 1:
//		if (!owner->GetModel()->IsPlayAnimation())
//		{
//			owner->SetHealth(owner->GetMaxHealth());
//			step = 0;
//			return ActionBase::State::Complete;
//		}
//		break;
//	}
//
//	return ActionBase::State::Run;
//
//}