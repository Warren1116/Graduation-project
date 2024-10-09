#include "RunZombie.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include "SceneManager.h"

// �R���X�g���N�^
RunZombie::RunZombie()
{
	model = std::make_unique<Model>("Data/Model/RunZombie/Zombie Agonizing.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.35f;


	TransitionWanderState();
}

//�@�f�X�g���N�^
RunZombie::~RunZombie()
{
}

// �X�V����
void RunZombie::Update(float elapsedTime)
{
	Player& player = Player::Instance();

	switch (state)
	{
		break;
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Pursuit:
		UpdatePursuitState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	case State::IdleBattle:
		UpdateIdleBattleState(elapsedTime);
		break;
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	}

	// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���x�����X�V
	UpdateVelocity(elapsedTime);

	// ���f���A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

//// �`�揈��
//void RunZombie::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}

// �f�o�b�O�v���~�e�B�u�`��
void RunZombie::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �꒣��͈͂��f�o�b�O�~���`��


	// �^�[�Q�b�g�ʒu���f�o�b�O���`��
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));


	// �U��������f�o�b�O���`��
	Model::Node* node = model->FindNode("EyeBall");
	float animationTime = model->GetCurrentAnimationSeconds();

	if (node != nullptr && attackCollisionFlag)
	{
		// �m�[�h�ʒu�擾
		DirectX::XMFLOAT3 nodePosition;
		nodePosition.x = node->worldTransform._41;
		nodePosition.y = node->worldTransform._42;
		nodePosition.z = node->worldTransform._43;
		debugRenderer->DrawSphere(nodePosition, 0.2f, DirectX::XMFLOAT4(0, 0, 1, 1));
	}

}

// �꒣��ݒ�
void RunZombie::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;

}

// �^�[�Q�b�g�ʒu�������_���ݒ�
void RunZombie::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);

	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = 0.0f;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// �ڕW�n�_�ֈړ�
void RunZombie::MoveToTarget(float elapsedTime, float speedRate)
{
	//�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

// �v���C���[���G
bool RunZombie::SearchPlayer()
{
	// �v���C���[�Ƃ̍��፷���l������3D�ł̋������������
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		// �P�ʃx�N�g����
		vx /= distXZ;
		vz /= distXZ;
		// �O���x�N�g��
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// �Q�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// �m�[�h�ƃv���C���[�̏Փˏ���
void RunZombie::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
	// �m�[�h�擾
	Model::Node* node = model->FindNode(nodeName);
	if (node != nullptr)
	{
		// �m�[�h�ʒu�擾
		DirectX::XMFLOAT3 nodePosition;
		nodePosition.x = node->worldTransform._41;
		nodePosition.y = node->worldTransform._42;
		nodePosition.z = node->worldTransform._43;


		// �v���C���[�Ƃ̂����蔻��
		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;

		if (Collision::IntersectSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{
			// �_���[�W��^����
			if (player.ApplyDamage(1, 0.5f))
			{
				// �G�𐁂���΂��x�N�g�����Z�o
				DirectX::XMFLOAT3 vec;
				vec.x = outPosition.x - nodePosition.x;
				vec.z = outPosition.z - nodePosition.z;
				float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
				vec.x /= length;
				vec.z /= length;

				// XZ���ʂɐ�����΂��͂�������
				float power = 10.0f;
				vec.x *= power;
				vec.z *= power;
				// Y�����ɂ��͂�������
				vec.y = 5.0f;

				// ������΂�
				player.AddImpulse(vec);
			}
		}
	}
}

// �p�j�X�e�[�g�֑J��
void RunZombie::TransitionWanderState()
{
	state = State::Wander;

	// �ڕW�n�_�ݒ�
	SetRandomTargetPosition();

	// ���[�V�����Đ�
	model->PlayAnimation(Anim_RunFWD, true);
}

// �p�j�X�e�[�g�X�V����
void RunZombie::UpdateWanderState(float elapsedTime)
{
	// �ڕW�n�_�܂�XZ���ʂł̋�������
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �ڕW�n�_�ֈړ�
	MoveToTarget(elapsedTime, 0.5f);

	if (SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		TransitionPursuitState();
	}
}

// �ҋ@�X�e�[�g�֑J��
void RunZombie::TransitionIdleState()
{
	state = State::Idle;

	// �^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	// �ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_IdleNormal, true);
}

// �ҋ@�X�e�[�g�X�V����
void RunZombie::UpdateIdleState(float elapsedTime)
{
	// �^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		// �p�j�X�e�[�g�֑J��
		TransitionWanderState();
	}

	if (SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		TransitionPursuitState();
	}
}

// �ǐՃX�e�[�g�֑J��
void RunZombie::TransitionPursuitState()
{
	state = State::Pursuit;

	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	// ���胂�[�V�����Đ�
	model->PlayAnimation(Anim_RunFWD, true);
}

// �ǐՃX�e�[�g�X�V����
void RunZombie::UpdatePursuitState(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	targetPosition = Player::Instance().GetPosition();

	// �ڕW�n�_�ֈړ�
	MoveToTarget(elapsedTime, 1.0f);

	// �^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �v���C���[�ɋ߂Â��ƍU���X�e�[�g�֑J��
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < attackRange)
	{
		// �U���X�e�[�g�֑J��
		TransitionAttackState();
	}
}

// �U���X�e�[�g�֑J��
void RunZombie::TransitionAttackState()
{
	state = State::Attack;


	// �U���A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Attack1, false);
}

// �U���X�e�[�g�X�V����
void RunZombie::UpdateAttackState(float elapsedTime)
{
	// �C�ӂ̃A�j���[�V�����Đ���Ԃ̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = animationTime >= 0.1f && animationTime <= 0.35f;

	if (attackCollisionFlag)
	{
		// �v���C���[�̏Փˏ���
		CollisionNodeVsPlayer("EyeBall", 0.2f);
	}

	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// �퓬�ҋ@�X�e�[�g�֑J��
void RunZombie::TransitionIdleBattleState()
{
	state = State::IdleBattle;

	// ���b�ԑҋ@����^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(2.0f, 3.0f);

	// �ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_IdleNormal, true);
}

// �퓬�ҋ@�X�e�[�g�X�V����
void RunZombie::UpdateIdleBattleState(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	targetPosition = Player::Instance().GetPosition();

	// �^�C�}�[�ݒ�
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		// �v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		if (dist < attackRange)
		{
			// �U���X�e�[�g�֑J��
			TransitionAttackState();
		}
		else
		{
			// �p�j�X�e�[�g�֑J��
			TransitionWanderState();
		}
	}

	MoveToTarget(elapsedTime, 0.0f);
}

// �_���[�W�X�e�[�g�֑J��
void RunZombie::TransitionDamageState()
{
	state = State::Damage;

	// �_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_IdleNormal, false);
}

// �_���[�W�X�e�[�g�X�V����
void RunZombie::UpdateDamageState(float elapsedTime)
{
	// �_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// ���S�X�e�[�g�֑J��
void RunZombie::TransitionDeathState()
{
	state = State::Death;

	// ���S�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Death, false);
}

// ���S�X�e�[�g�X�V����
void RunZombie::UpdateDeathState(float elapsedTime)
{
	// ���S�A�j���[�V�������I������玩����j��
	if (!model->IsPlayAnimation())
	{
		Destroy();
	}
}

// ���S�����Ƃ��ɌĂ΂��
void RunZombie::OnDamaged()
{
	//TransitionDamageState();
}

// ���S�����Ƃ��ɌĂ΂��
void RunZombie::OnDead()
{
	TransitionDeathState();
}
