#include "ActionDerived.h"
#include "PoisonZombie.h"
#include "Player.h"
#include "Mathf.h"
#include"Camera.h"
#include"CameraController.h"

// 通常攻撃移動
ActionBase::State NormalActionMove::Run(float elapsedTime)
{
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		// 目標地点をプレイヤー位置に設定
		owner->SetTargetPosition(Player::Instance().GetPosition());
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Walk), true);
		step++;
		break;
	case 1:
		runTimer -= elapsedTime;
		// タイマー更新
		owner->SetRunTimer(runTimer);
		// 目標地点をプレイヤー位置に設定
		owner->SetTargetPosition(Player::Instance().GetPosition());
		// 目的地点へ移動
		owner->MoveToTarget(elapsedTime, 2.0f);

		// プレイヤーとの距離を計算
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();

		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		// 攻撃範囲にいるとき
		if (dist < owner->GetAttackRange())
		{
			step = 0;
			// 追跡成功を返す
			return ActionBase::State::Complete;
		}
		// 行動時間が過ぎた時
		if (runTimer <= 0.0f)
		{
			step = 0;
			// 追跡失敗を返す
			return ActionBase::State::Failed;
		}
		break;
	}
	return ActionBase::State::Run;
}

// 攻撃行動
ActionBase::State NormalAction::Run(float elapsedTime)
{
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::HandAttack), false);
		step++;
		break;
	case 1:
		// 任意のアニメーション再生区間のみ衝突判定処理をする
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		owner->SetAttackCollisionFlg(1.3f, 1.35f, animationTime);
		if (owner->GetAttackCollisionFlg())
		{
			// 目玉ノードとプレイヤーの衝突処理
			owner->CollisionNodeVsPlayer("mixamorig:LeftHandThumb4", 0.2f);
		}
		runTimer -= elapsedTime;
		// タイマー更新
		owner->SetRunTimer(runTimer);
		// アニメーションが終了しているとき
		if (!owner->GetModel()->IsPlayAnimation())
		{
			step = 0;
			// 攻撃成功を返す
			return ActionBase::State::Complete; 
		}
		break;
	}
	// 攻撃中を返す
	return ActionBase::State::Run;
}

// 銃攻撃移動
ActionBase::State GunActionMove::Run(float elapsedTime)
{
	DirectX::XMFLOAT3 playerPos = Player::Instance().GetPosition();
	float vx = playerPos.x;
	float vy = playerPos.y;
	float vz = playerPos.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		owner->SetTargetPosition(Player::Instance().GetPosition());
		owner->SetRunTimer(5.4f);
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::AttackIdle), false);
		owner->timer += elapsedTime;
		koe = Audio::Instance().LoadAudioSource("Data/Audio/boss.wav");
		koe->Play(false);
		step++;
		break;
	case 1:
		Camera& camera=Camera::Instance();
		CameraController& cameraController = CameraController::Instance();
		owner->SetTargetPosition(Player::Instance().GetPosition());
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		if (animationTime >= 1.5f && animationTime <= 4.0f)
		{
			owner->SetCameraFlg(true);
			if (dist<owner->GetAttackGeroRange())
			{
				Player::Instance().ApplyDamage(5.0f, 1.0f);
			}
		}
		runTimer -= elapsedTime;
		owner->SetRunTimer(runTimer);
		if (runTimer <= 0.0f)
		{
			owner->SetCameraFlg(false);
			step = 0;
			return ActionBase::State::Complete;
		}
		break;
	}
	return ActionBase::State::Run;
}



// ジャンプ攻撃移動
ActionBase::State JumpActionMove::Run(float elapsedTime)
{
	
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		owner->SetRandomTargetPosition();
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::JumpAttack), false);
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		step++;
		break;
	case 1:
		// 目的地点へ移動
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
		DirectX::XMFLOAT3 jumposition = owner->Instance().jumpPosition;

		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		if (animationTime >= 1.324f && animationTime<=1.867f)
		{
			owner->MoveToTarget(elapsedTime, 1.0f);
		}

		float vx = targetPosition.x - position.x;
		float vz = targetPosition.z - position.z;
		float distSq = vx * vx + vz * vz;

		// プレイヤーを見つけた時
		float radius = owner->GetRadius();
		if (!owner->GetModel()->IsPlayAnimation())
		{
			step = 0;
			// 徘徊成功を返す
			return ActionBase::State::Complete;
		}
		break;
	}
	return ActionBase::State::Run;

}

// ジャンプ攻撃
ActionBase::State JumpAction::Run(float elapsedTime)
{
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		owner->SetTargetPosition(Player::Instance().GetPosition());
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::JumpAttack), false);
		step++;
		break;
	case 1:
		owner->SetTargetPosition(Player::Instance().GetPosition());
		runTimer -= elapsedTime;
		// タイマー更新
		owner->SetRunTimer(runTimer);
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
		float vx = targetPosition.x - position.x;
		float vz = targetPosition.z - position.z;
		float distSq = vx * vx + vz * vz;

		// 目的地点へ移動
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		if (animationTime >= 1.324f && animationTime <= 1.865f)
		{
			owner->JumpMoveToTarget(elapsedTime, 1.5f);

		}
		// プレイヤーを見つけた時
		float radius = owner->GetRadius();
		if (!owner->GetModel()->IsPlayAnimation() && runTimer <= 0.0f)
		{
			step = 0;
			// 徘徊成功を返す
			return ActionBase::State::Complete;
		}
		break;
	}
	return ActionBase::State::Run;
}

// 待機行動
ActionBase::State IdleAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::IdleNormal), false);
		step++;
		break;
	case 1:
		// プレイヤーを見つけた時
		if (!owner->GetModel()->IsPlayAnimation())
		{
			return ActionBase::State::Complete;
			step = 0;
		}
		break;
	}
	return ActionBase::State::Run;
}

ActionBase::State DieAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Die), false);
		step++;
		break;
	case 1:
		// プレイヤーを見つけた時
		if (!owner->GetModel()->IsPlayAnimation())
		{
			return ActionBase::State::Complete;
			step = 0;
		}
		break;
	}
	return ActionBase::State::Run;
}
ActionBase::State PursuitAction::Run(float elapsedTime)
{
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		// 目標地点をプレイヤー位置に設定
		owner->SetTargetPosition(Player::Instance().GetPosition());
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Walk), true);
		step++;
		break;
	case 1:
		runTimer -= elapsedTime;
		// タイマー更新
		owner->SetRunTimer(runTimer);
		// 目標地点をプレイヤー位置に設定
		owner->SetTargetPosition(Player::Instance().GetPosition());
		// 目的地点へ移動
		owner->MoveToTarget(elapsedTime, 1.0);

		// プレイヤーとの距離を計算
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();

		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		// 攻撃範囲にいるとき
		if (dist < owner->GetAttackRange())
		{
			step = 0;
			// 追跡成功を返す
			return ActionBase::State::Complete;
		}
		// 行動時間が過ぎた時
		if (runTimer <= 0.0f)
		{
			step = 0;
			// 追跡失敗を返す
			return ActionBase::State::Failed;
		}
		break;
	}
	return ActionBase::State::Run;
}