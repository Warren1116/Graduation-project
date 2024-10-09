#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"

// TODO 02_03 State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

//-------------//
// WanderState //
//-------------//
// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
	owner->SetRandomTargetPosition();
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Pursuit));
	}

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

//-----------//
// IdleState //
//-----------//
// �ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleNormal), true);

	// �^�C�}�[�������_���Őݒ�
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void IdleState::Execute(float elapsedTime)
{
	float stateTimer = owner->GetStateTimer();
	stateTimer -= elapsedTime;
	owner->SetStateTimer(stateTimer);

	// �ҋ@���Ԃ��o�߂�����p�j�X�e�[�g�֑J��
	if (owner->GetStateTimer() <= 0.0f)
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Wander));
	}

	// �v���C���[������������ǐՃX�e�[�g�֑J��
	if (owner->SearchPlayer())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Pursuit));
	}
}

// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void IdleState::Exit()
{

}

//--------------//
// PursuitState //
//--------------//
// �ǐՃX�e�[�g�ɓ��������̃��\�b�h
void PursuitState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);

	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// �ǐՃX�e�[�g�Ŏ��s���郁�\�b�h
void PursuitState::Execute(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	owner->SetTargetPosition(Player::Instance().GetPosition());

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 1.0);

	// �^�C�}�[����
	float stateTimer = owner->GetStateTimer();
	stateTimer -= elapsedTime;
	owner->SetStateTimer(stateTimer);

	// �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J��
	if (owner->GetStateTimer() < 0.0f)
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::Idle));
	}

	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J��
	if (dist < owner->GetAttackRange())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::HandAttack));
	}
}

// �ǐՃ��\�b�h����o�Ă����Ƃ��̃��\�b�h
void PursuitState::Exit()
{
	
}

//AttackIdle
//�퓬�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
//
void AttackIdleState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackIdle), false);

}

// �퓬�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void AttackIdleState::Execute(float elapsedTime)
{
	// ����߂�����ǐՃX�e�[�g

	// ���͈͓��Ȃ�ōU��
	// �X�ɋ߂���΃W�����v�U�����ǐՃX�e�[�g
}

// �퓬�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackIdleState::Exit()
{
}


//
// �ōU���ɓ��������̃��\�b�h 
//
void GunState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::GunAttack), false);

}

// �ōU���X�e�[�g�Ŏ��s���郁�\�b�h
void GunState::Execute(float elapsedTime)
{
	// ���[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::AttackIdle));

	}
}

// �ōU������o�Ă����Ƃ��̃��\�b�h
void GunState::Exit()
{
	
}

//
// �蓁�U���ɓ��������̃��\�b�h
//
void HandState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackIdle), false);

}

// �蓁�U���X�e�[�g�Ŏ��s���郁�\�b�h
void HandState::Execute(float elapsedTime)
{
	// ���[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::AttackIdle));

	}
}

// �蓁�U������o�Ă����Ƃ��̃��\�b�h
void HandState::Exit()
{
}

//
// �W�����v�U���X�e�[�g�ɓ��������̃��\�b�h
//
void JumpState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::JumpAttack), false);
}

// �W�����v�U���Ŏ��s���郁�\�b�h
void JumpState::Execute(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	owner->SetTargetPosition(Player::Instance().GetPosition());

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 1.0);


	// ���[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachin()->ChangeState(static_cast<int>(PoisonZombie::State::AttackIdle));

	}
}

// �W�����v�U������o�Ă����Ƃ��̃��\�b�h
void JumpState::Exit()
{
}

