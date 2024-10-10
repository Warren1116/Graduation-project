//#pragma once
//
//#include "Graphics/Model.h"
//#include "Enemy.h"
//#include "Effect.h"
//#include "Object.h"
//
//// �X���C��
//class TutorialZombie : public Enemy
//{
//public:
//	TutorialZombie();
//	~TutorialZombie() override;
//
//	// �X�V����
//	void Update(float elapsedTime) override;
//
//	// �f�o�b�O�v���~�e�B�u�`��
//	void DrawDebugPrimitive() override;
//
//	void TransitionTitleState();
//	void UpdateTitleState(float elapsedTime);
//
//private:
//	// �^�[�Q�b�g�ʒu�������_���ɐݒ�
//	void SetRandomTargetPosition();
//
//	// �ړI�n�_�ֈړ�
//	void MoveToTarget(float elapsedTime, float speedRate);
//
//	// �v���C���[���G
//	bool SearchPlayer();
//
//	// �m�[�h�ƃv���C���[�̏Փˏ���
//	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);
//
//	// �p�j�X�e�[�g�֑J��
//	void TransitionWanderState();
//
//	// �p�j�X�e�[�g�X�V����
//	void UpdateWanderState(float elapsedTime);
//
//	// �ҋ@�X�e�[�g�֑J��
//	void TransitionIdleState();
//
//	// �ҋ@�X�e�[�g�X�V����
//	void UpdateIdleState(float elapsedTime);
//
//	// �ǐՃX�e�[�g�֑J��
//	void TransitionPursuitState();
//
//	// �ǐՃX�e�[�g�X�V����
//	void UpdatePursuitState(float elapsedTime);
//
//	// �U���X�e�[�g�֑J��
//	void TransitionAttackState();
//
//	// �U���X�e�[�g�X�V����
//	void UpdateAttackState(float elapsedTime);
//
//	// �퓬�ҋ@�X�e�[�g�֑J��
//	void TransitionIdleBattleState();
//
//	// �퓬�ҋ@�X�e�[�g�X�V����
//	void UpdateIdleBattleState(float elapsedTime);
//
//	// �_���[�W�X�e�[�g�֑J��
//	void TransitionDamageState();
//
//	// �_���[�W�X�e�[�g�X�V����
//	void UpdateDamageState(float elapsedTime);
//
//	// ���S�X�e�[�g�֑J��
//	void TransitionDeathState();
//
//	// ���S�X�e�[�g�X�V����
//	void UpdateDeathState(float elapsedTime);
//
//private:
//	// �X�e�[�g
//	enum class State
//	{
//		Wander,
//		Idle,
//		Pursuit,
//		Attack,
//		IdleBattle,
//		Damage,
//		Death,
//		Title,
//	};
//
//	// �A�j���[�V����
//	enum
//	{
//		Anim_Death,
//		Anim_Attack1,
//		Anim_RunFWD,
//		Anim_IdleNormal,
//	};
//
//protected:
//	// ���S�����Ƃ��ɌĂ΂��
//	void OnDamaged() override;
//
//	// ���S�����Ƃ��ɌĂ΂��
//	void OnDead() override;
//
//private:
//	State state = State::Wander;
//	DirectX::XMFLOAT3 targetPosition = { 0, 0, 0 };
//	DirectX::XMFLOAT3 territoryOrigin = { 0, 0, 0 };
//	float territoryRange = 10.0f;
//	float moveSpeed = 3.0f;
//	float turnSpeed = DirectX::XMConvertToRadians(360);
//	float stateTimer = 0.0f;
//	float searchRange = 5.0f;
//	float attackRange = 1.5f;
//	bool attackCollisionFlag = false;
//
//	float timer = 0;
//};