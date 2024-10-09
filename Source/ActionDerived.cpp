#include "ActionDerived.h"
#include "PoisonZombie.h"
#include "Player.h"
#include "Mathf.h"
#include"Camera.h"
#include"CameraController.h"

// �ʏ�U���ړ�
ActionBase::State NormalActionMove::Run(float elapsedTime)
{
	float runTimer = owner->GetRunTimer();
	switch (step)
	{
	case 0:
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner->SetTargetPosition(Player::Instance().GetPosition());
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Walk), true);
		step++;
		break;
	case 1:
		runTimer -= elapsedTime;
		// �^�C�}�[�X�V
		owner->SetRunTimer(runTimer);
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner->SetTargetPosition(Player::Instance().GetPosition());
		// �ړI�n�_�ֈړ�
		owner->MoveToTarget(elapsedTime, 2.0f);

		// �v���C���[�Ƃ̋������v�Z
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();

		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		// �U���͈͂ɂ���Ƃ�
		if (dist < owner->GetAttackRange())
		{
			step = 0;
			// �ǐՐ�����Ԃ�
			return ActionBase::State::Complete;
		}
		// �s�����Ԃ��߂�����
		if (runTimer <= 0.0f)
		{
			step = 0;
			// �ǐՎ��s��Ԃ�
			return ActionBase::State::Failed;
		}
		break;
	}
	return ActionBase::State::Run;
}

// �U���s��
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
		// �C�ӂ̃A�j���[�V�����Đ���Ԃ̂ݏՓ˔��菈��������
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		owner->SetAttackCollisionFlg(1.3f, 1.35f, animationTime);
		if (owner->GetAttackCollisionFlg())
		{
			// �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
			owner->CollisionNodeVsPlayer("mixamorig:LeftHandThumb4", 0.2f);
		}
		runTimer -= elapsedTime;
		// �^�C�}�[�X�V
		owner->SetRunTimer(runTimer);
		// �A�j���[�V�������I�����Ă���Ƃ�
		if (!owner->GetModel()->IsPlayAnimation())
		{
			step = 0;
			// �U��������Ԃ�
			return ActionBase::State::Complete; 
		}
		break;
	}
	// �U������Ԃ�
	return ActionBase::State::Run;
}

// �e�U���ړ�
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



// �W�����v�U���ړ�
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
		// �ړI�n�_�ֈړ�
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

		// �v���C���[����������
		float radius = owner->GetRadius();
		if (!owner->GetModel()->IsPlayAnimation())
		{
			step = 0;
			// �p�j������Ԃ�
			return ActionBase::State::Complete;
		}
		break;
	}
	return ActionBase::State::Run;

}

// �W�����v�U��
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
		// �^�C�}�[�X�V
		owner->SetRunTimer(runTimer);
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
		float vx = targetPosition.x - position.x;
		float vz = targetPosition.z - position.z;
		float distSq = vx * vx + vz * vz;

		// �ړI�n�_�ֈړ�
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		if (animationTime >= 1.324f && animationTime <= 1.865f)
		{
			owner->JumpMoveToTarget(elapsedTime, 1.5f);

		}
		// �v���C���[����������
		float radius = owner->GetRadius();
		if (!owner->GetModel()->IsPlayAnimation() && runTimer <= 0.0f)
		{
			step = 0;
			// �p�j������Ԃ�
			return ActionBase::State::Complete;
		}
		break;
	}
	return ActionBase::State::Run;
}

// �ҋ@�s��
ActionBase::State IdleAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::IdleNormal), false);
		step++;
		break;
	case 1:
		// �v���C���[����������
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
		// �v���C���[����������
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
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner->SetTargetPosition(Player::Instance().GetPosition());
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		owner->GetModel()->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Walk), true);
		step++;
		break;
	case 1:
		runTimer -= elapsedTime;
		// �^�C�}�[�X�V
		owner->SetRunTimer(runTimer);
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner->SetTargetPosition(Player::Instance().GetPosition());
		// �ړI�n�_�ֈړ�
		owner->MoveToTarget(elapsedTime, 1.0);

		// �v���C���[�Ƃ̋������v�Z
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();

		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		// �U���͈͂ɂ���Ƃ�
		if (dist < owner->GetAttackRange())
		{
			step = 0;
			// �ǐՐ�����Ԃ�
			return ActionBase::State::Complete;
		}
		// �s�����Ԃ��߂�����
		if (runTimer <= 0.0f)
		{
			step = 0;
			// �ǐՎ��s��Ԃ�
			return ActionBase::State::Failed;
		}
		break;
	}
	return ActionBase::State::Run;
}