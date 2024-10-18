#include "EnemyPeople.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "StateDerived.h"
#include "SceneGame.h"


#define USESTATEMACHINE3

// �R���X�g���N�^
EnemyPeople::EnemyPeople()
{
	model = std::make_unique<Model>("Data/Model/Slime/Slime.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.5f;
	height = 1.0f;

	// StateMachine�𐶐����A�K�w�^�X�e�[�g�}�V���ɑΉ�����悤�ɓo�^�X�e�[�g��ύX���Ă����B
	stateMachine = new StateMachine();

	stateMachine->RegisterState(new SearchState(this));
	stateMachine->RegisterState(new BattleState(this));

	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new WanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new IdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new PursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new AttackState(this));

	// �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̈�w�ڂ̃X�e�[�g��ǉ��o�^
	stateMachine->RegisterState(new RecieveState(this));

	// �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̃T�u�X�e�[�g��ǉ��o�^
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Recieve), new CalledState(this));

	// �퓬���U�����̎����Ă��Ȃ���Ԃł̑ҋ@�X�e�[�g��ǉ��o�^
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new StandbyState(this));

	stateMachine->SetState(static_cast<int>(State::Search));

	// �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̂P�w�ڂ̃X�e�[�g��ǉ��o�^
	stateMachine->RegisterState(new RecieveState(this));

	// �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^(WanderState�ȊO��2�w�ڂ̃X�e�[�g�����l�̕��@�Ŋe���ǉ����Ă��������B)
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new IdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new WanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new PursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new AttackState(this));

	// �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̃T�u�X�e�[�g��ǉ��o�^
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Recieve), new CalledState(this));

	// �퓬���U�����̎����Ă��Ȃ���Ԃł̑ҋ@�X�e�[�g��ǉ��o�^
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new StandbyState(this));

	// �X�e�[�g���Z�b�g
	stateMachine->SetState(static_cast<int>(State::Search));

}

// �f�X�g���N�^
EnemyPeople::~EnemyPeople()
{
	delete stateMachine;
}

void EnemyPeople::Update(float elapsedTime)
{
	stateMachine->Update(elapsedTime);

	// ���͏����X�V
	UpdateVelocity(elapsedTime);

	// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s��X�V
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

// ���S�������ɌĂ΂��
void EnemyPeople::OnDead()
{
	// ���S
	Destroy();
	SceneGame::Instance().UnregisterRenderModel(model.get());
}


void EnemyPeople::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �꒣��͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// �^�[�Q�b�g�ʒu���f�o�b�O���`��
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// ���G�͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}

// �꒣��ݒ�
void EnemyPeople::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}
// �^�[�Q�b�g�ʒu�������_���ݒ�
void EnemyPeople::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// �ړI�n�_�ֈړ�
void EnemyPeople::MoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemyPeople::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MsgCallHelp:
		if (!SearchPlayer())
		{	// �v���C���[�������Ă��Ȃ��Ƃ��Ɉ�w�ڃX�e�[�g��Receive�ɕύX����
			stateMachine->ChangeState(static_cast<int>(EnemyPeople::State::Recieve));
		}
		return true;
		//	���^AI����U������^����ꂽ�Ƃ�
	case MESSAGE_TYPE::MsgGiveAttackRight:
		// �U���t���O��true�ɐݒ�
		SetAttackFlg(true);
		return true;
	}
	return false;
}


bool EnemyPeople::SearchPlayer()
{
	// �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
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

		// �����x�N�g����
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}


// �f�o�b�O�G�l�~�[���\��
void EnemyPeople::DrawDebugGUI()
{

	std::string str = "";
	std::string subStr = "";

	switch (static_cast<State>(stateMachine->GetStateIndex()))
	{
	case State::Search:
		str = "Search";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Search::Wander))
		{
			subStr = "Wander";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Search::Idle))
		{
			subStr = "Idle";
		}
		break;
	case State::Battle:
		str = "Battle";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Battle::Pursuit))
		{
			subStr = "Pursuit";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Battle::Attack))
		{
			subStr = "Attack";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Battle::Standby))
		{
			subStr = "Standby";
		}
		break;
	case State::Recieve:
		if (stateMachine->GetStateIndex() == static_cast<int>(EnemyPeople::Recieve::Called))
		{
			subStr = "Called";
		}
		break;

	}


	//�g�����X�t�H�[��
	if (ImGui::CollapsingHeader("EnemyPeople", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �ʒu
		ImGui::InputFloat3("Position", &position.x);
		// ��]
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// �X�P�[��
		ImGui::InputFloat3("Scale", &scale.x);

		ImGui::Text(u8"State�@%s", str.c_str());
		ImGui::Text(u8"SubState �@%s", subStr.c_str());
	}
}
