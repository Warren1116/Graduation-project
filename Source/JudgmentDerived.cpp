#include "JudgmentDerived.h"
#include "Player.h"
#include "Mathf.h"


// BattleNode�ɑJ�ڂł��邩����
bool BattleJudgment::Judgment()
{
	if (owner->GetHealth() > 0)
	{
		// �v���C���[�������邩
		if (owner->SearchPlayer())
		{
		return true;
		}
	}
	return true;
}

// HandAttackNode�ɑJ�ڂł��邩����
bool HandAttackMoveJudgment::Judgment()
{

	// �ΏۂƂ̋������Z�o
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist >= owner->GetAttackRange())
	{
		// HandNode�֑J�ڂł���
		return true;
	}
	return false;
}

bool HandAttackJudgment::Judgment()
{
	// �ΏۂƂ̋������Z�o
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		// HandNode�֑J�ڂł���
		return true;
	}
	return false;
}


// Jump�m�[�m�ɑJ�ڂł��邩
bool JumpAttackMoveJudgment::Judgment()
{
		// �ΏۂƂ̋������Z�o
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);

		if (dist >= owner->GetJumpAttackRange() && dist <= owner->GetAttackRange())
		{
			// JumpNode�֑J�ڂł���
			return true;
		}
		return false;
}

bool JumpAttackJudgment::Judgment()
{
	// �ΏۂƂ̋������Z�o
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist <= owner->GetJumpAttackRange() && dist >= owner->GetAttackRange())
	{
		// JumpNode�֑J�ڂł���
		return true;
	}
	return false;
}

bool DieJudgment::Judgment()
{
	if (owner->GetHealth() <= 0)
	{
		return true;
	}
	return false;
}

bool WnderJudgment::Judgment()
{
	if (!owner->SearchPlayer())
	{
		return true;
	}
	return false;
	
}
