#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include"Effect.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;


class PoisonZombie :public Enemy
{
public:
	PoisonZombie();
	~PoisonZombie()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	void GunSetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI() override;

	// �W�����v�^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();

	// �e�U���^�[�Q�b�g�͈͂������_���ݒ�
	void SetGunRandomTargetPosition();

	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);

	void JumpMoveToTarget(float elapsedTime, float speedRate);

	// �v���C���[���G
	bool SearchPlayer();

	// �^�[�Q�b�g�|�W�V�����ݒ�
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// �^�[�Q�b�g�|�W�V�����擾
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	// �|�W�V�����擾
	DirectX::XMFLOAT3 GetPosition() { return position; }

	// �X�e�[�g�^�C�}�[�ݒ�
	void SetStateTimer(float timer) {
		stateTimer = timer;
	}
	// �X�e�[�g�^�C�}�[�擾
	float GetStateTimer() { return stateTimer; }

	// �U���͈͎擾
	float GetAttackRange() { return attackRange; }
	float GetAttackGeroRange() { return geroattackRange; }

	// �W�����v�U���͈͎擾
	float GetJumpAttackRange() { return jumpAttackRange; }

	// �e�U���͈͎擾
	float GetGunAttackRange() { return gunAttackRange; }

	// ���f���擾
	Model* GetModel() { return model; }

	static PoisonZombie& Instance();

	// ���s�^�C�}�[�ݒ�
	void SetRunTimer(float timer) { runTimer = timer; }

	// ���s�^�C�}�[�擾
	float GetRunTimer() { return runTimer; }

	void SetHealth(int setHealth) { health = setHealth; }

	float PlayerToBoss() override;

protected:
	void OnDead();
public:
	// TODO 01 �K�v�ȃX�e�[�g��enum class�ŏ������Ȃ����B
	// �iWander�̂݋L�q�ς݁j
	enum class State
	{
		Wander,
		Idle,
		Pursuit,
		AttackIdle,
		GunAttack,
		HandAttack,
		JumpAttack
	};
	// �G�l�~�[�A�j���[�V����
	enum class EnemyAnimation
	{
		IdleNormal,
		Walk,
		GunAttack,
		HandAttack,
		Die,
		AttackIdle,
		JumpAttack,
	};


	// �e�̃t���O�Q�b�^�[�Z�b�^�[
	void SetGunFlg(bool flg) { gunflg = flg; }
	float GetGunFlg() { return gunflg; }

	void SetAttackFlg(bool flg) { attackFlag = flg; }
	float GetAttackFlg() { return attackFlag; }

	void SetAttackCollisionFlg(float min, float max, float animationTime) 
	{
		attackCollisionFlag = animationTime >= min && animationTime <= max;
	}
	bool GetAttackCollisionFlg() { return attackCollisionFlag; }
	
	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

public:
	DirectX::XMFLOAT3 jumpPosition;
	Effect* gero = nullptr;
	int timer = 0.0f;
private:

	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				territoryGunRange = 20.0f;
	float				moveSpeed = 10.0f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				stateTimer = 0.0f;
	float				searchRange = 10.0f;
	float				attackRange = 3.0f;
	float				geroattackRange = 5.0f;
	float				jumpAttackRange = 10.0f;
	float				gunAttackRange = 150.0f;
	bool attackCollisionFlag = false;


	float				runTimer = 0.0f;
	bool				gunflg = false;
	bool attackFlag = false;

	BehaviorTree* aiTree = nullptr;
	BehaviorData* behaviorData = nullptr;
	NodeBase* activeNode = nullptr;
};