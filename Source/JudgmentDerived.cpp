#include "JudgmentDerived.h"
#include "Player.h"
#include "Mathf.h"


// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
bool BattleJudgment::Judgment()
{
	if (owner->GetHealth() > 0)
	{
		// ƒvƒŒƒCƒ„[‚ªŒ©‚Â‚©‚é‚©
		if (owner->SearchPlayer())
		{
		return true;
		}
	}
	return true;
}

// HandAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
bool HandAttackMoveJudgment::Judgment()
{

	// ‘ÎÛ‚Æ‚Ì‹——£‚ðŽZo
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist >= owner->GetAttackRange())
	{
		// HandNode‚Ö‘JˆÚ‚Å‚«‚é
		return true;
	}
	return false;
}

bool HandAttackJudgment::Judgment()
{
	// ‘ÎÛ‚Æ‚Ì‹——£‚ðŽZo
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		// HandNode‚Ö‘JˆÚ‚Å‚«‚é
		return true;
	}
	return false;
}


// Jumpƒm[ƒm‚É‘JˆÚ‚Å‚«‚é‚©
bool JumpAttackMoveJudgment::Judgment()
{
		// ‘ÎÛ‚Æ‚Ì‹——£‚ðŽZo
		DirectX::XMFLOAT3 position = owner->GetPosition();
		DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);

		if (dist >= owner->GetJumpAttackRange() && dist <= owner->GetAttackRange())
		{
			// JumpNode‚Ö‘JˆÚ‚Å‚«‚é
			return true;
		}
		return false;
}

bool JumpAttackJudgment::Judgment()
{
	// ‘ÎÛ‚Æ‚Ì‹——£‚ðŽZo
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist <= owner->GetJumpAttackRange() && dist >= owner->GetAttackRange())
	{
		// JumpNode‚Ö‘JˆÚ‚Å‚«‚é
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
