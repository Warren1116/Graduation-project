#include "PoisonZombie.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentDerived.h"
#include "ActionDerived.h"
#include "Collision.h"

static PoisonZombie* instance = nullptr;

// �C���X�^���X�擾
PoisonZombie& PoisonZombie::Instance()
{
	return *instance;
}



// �R���X�g���N�^
PoisonZombie::PoisonZombie()
{
	health = 5;

	model = new Model("Data/Model/PoisonZombie/PoisonZombie.mdl");


	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;


	radius = 0.5f;
	height = 1.0f;

	// �r�w�C�r�A�c���[�ݒ�
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);

	// BehaviorTree�}�����BehaviorTree���\�z
	aiTree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
	aiTree->AddNode("Root", "Scout", 1, BehaviorTree::SelectRule::Priority, new WnderJudgment(this), nullptr);
	aiTree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Random, new BattleJudgment(this), nullptr);
	aiTree->AddNode("Root", "Die", 3, BehaviorTree::SelectRule::Non, new DieJudgment(this), new DieAction(this));

	//aiTree->AddNode("Scout", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
	aiTree->AddNode("Scout", "Wander", 0, BehaviorTree::SelectRule::Non, nullptr, new PursuitAction(this));

	aiTree->AddNode("Battle", "BattleMoveTree", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
	aiTree->AddNode("Battle", "GunMoveTree", 1, BehaviorTree::SelectRule::Non, nullptr,new GunActionMove(this));

	aiTree->AddNode("BattleMoveTree", "BattleMove", 0, BehaviorTree::SelectRule::Non, new HandAttackMoveJudgment(this), new NormalActionMove(this));
	aiTree->AddNode("BattleMoveTree", "HandAttack", 1, BehaviorTree::SelectRule::Non, new HandAttackJudgment(this), new NormalAction(this));

	SetPosition(DirectX::XMFLOAT3(5.0f, 0.0f, 25.0f));
	SetTerritory(GetPosition(), 10.0f);
	SetRandomTargetPosition();
	SetGunRandomTargetPosition();
}

// �f�X�g���N�^
PoisonZombie::~PoisonZombie()
{
	if (behaviorData != nullptr)
	{
		delete behaviorData;
		behaviorData = nullptr;
	}
	if (aiTree != nullptr)
	{
		delete aiTree;
		aiTree = nullptr;
	}
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void PoisonZombie::Update(float elapsedTime)
{
	if (!isAlive)
		Destroy();

	// ���ݎ��s����Ă���m�[�h���������
	if (activeNode == nullptr)
	{
		// ���Ɏ��s����m�[�h�𐄘_����B
		activeNode = aiTree->ActiveNodeInference(behaviorData);
	}
	// ���ݎ��s����m�[�h�������
	if (activeNode != nullptr)
	{
		// �r�w�C�r�A�c���[����m�[�h�����s�B
		activeNode = aiTree->Run(activeNode, behaviorData, elapsedTime);
	}

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

float PoisonZombie::PlayerToBoss()
{
	float length;
	DirectX::XMStoreFloat(&length,
		DirectX::XMVector3Length(DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&Player::Instance().GetPosition()))));

	return length;
}

// ���S�������ɌĂ΂��
void PoisonZombie::OnDead()
{
	model->PlayAnimation(static_cast<int>(PoisonZombie::EnemyAnimation::Die), false);
	if (!model->IsPlayAnimation())
	{
		isAlive = false;
	}
}

//void PoisonZombie::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}


void PoisonZombie::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �꒣��͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	debugRenderer->DrawCylinder(territoryOrigin, attackRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));

	// �^�[�Q�b�g�ʒu���f�o�b�O���`��
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// ���G�͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}



// �꒣��ݒ�
void PoisonZombie::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

void PoisonZombie::GunSetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryGunRange = range;
}

// �^�[�Q�b�g�ʒu�������_���ݒ�
void PoisonZombie::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(attackRange, jumpAttackRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
	jumpPosition = { targetPosition.x,targetPosition.y,targetPosition.z };

}

void PoisonZombie::SetGunRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(jumpAttackRange, territoryGunRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}


// �ړI�n�_�ֈړ�
void PoisonZombie::MoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	Move(vx, vz, speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void PoisonZombie::JumpMoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x-0.8f;
	float vz = targetPosition.z - position.z-0.8f;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	position.x += vx;
	position.z += vz;
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool PoisonZombie::SearchPlayer()
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
void PoisonZombie::DrawDebugGUI()
{
	std::string str = "";
	if (activeNode != nullptr)
	{
		str = activeNode->GetName();
	}
	//�g�����X�t�H�[��
	if (ImGui::CollapsingHeader("PoisonZombie", ImGuiTreeNodeFlags_DefaultOpen))
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

		ImGui::Text(u8"Behavior�@%s", str.c_str());
	}
}

void PoisonZombie::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
			}
		}
	}
}