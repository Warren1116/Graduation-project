#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "Audio/Audio.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "CameraController.h"
#include "ProjectileManager.h"
#include "Graphics/geometric_primitive.h"
#include "BrokenProjectileManager.h"
#include <memory.h>


// �v���C���[
class Player : public Character
{
public:
	Player(bool flag);
	~Player();
	static Player& Instance() { return *instance; }

	// �X�V����
	void Update(float elapsedTime);

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �ړ����͏���
	bool InputMove(float elapsedTime);

	// �W�����v���͏���
	bool InputJump();

	void OnLanding() override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	ProjectileManager& GetProjectileManager() { return projectileManager; }
	BrokenProjectileManager& GetBrokenProjectileManager() { return brokenprojectileManager; }


	std::unique_ptr<Model> model = nullptr;

	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Damage,
		Death,
		Revive,
		Dodge,
		Climb,
		Swing,
		Shot,
	};


	// �X�e�[�g�擾
	const State& GetState() const { return state; }
	const State& GetAttackState() const { return State::Attack; }

	//std::unique_ptr<AudioSource> null = nullptr;


protected:

	// ���S�����Ƃ��ɌĂ΂��
	void OnDamaged() override;

	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

private:
	// �U�����͏���
	bool InputAttack();
	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

	void CollisionProjectileVsEnemies();

	// �ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	// �ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	// �ړ��X�e�[�g�֑J��
	void TransitionMoveState();

	// �ړ��X�e�[�g�X�V����
	void UpdateMoveState(float elapsedTime);

	// �W�����v�X�e�[�g�֑J��
	void TransitionJumpState();

	// �W�����v�X�e�[�g�X�V����
	void UpdateJumpState(float elapsedTime);

	// �N�����v�X�e�[�g�֑J��
	void TransitionClimbWallState();

	// �N�����v�X�e�[�g�X�V����
	void UpdateClimbWallState(float elapsedTime);

	// �U���X�e�[�g�֑J��
	void TransitionAttackState();

	// �U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);

	// �_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();

	// �_���[�W�X�e�[�g�X�V����
	void UpdateDamageState(float elapsedTime);

	// ���S�X�e�[�g�֑J��
	void TransitionDeathState();

	// ���S�X�e�[�g�X�V����
	void UpdateDeathState(float elapsedTime);

	// ���n�X�e�[�g�֑J�� 
	void TransitionLandState();

	// ���n�X�e�[�g�X�V����
	void UpdateLandState(float elapsedTime);

	// Swing�e�[�g�֑J��
	void TransitionSwingState();

	// Swing�X�e�[�g�X�V����
	void UpdateSwingState(float elapsedTime);

	// �m�[�h�ƃG�l�~�[�̏Փˏ���
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	void PlayAttackAnimation();

	// �J�����X�e�[�g�̍X�V
	void UpdateCameraState(float elapsedTime);

	void TransitionShotingState();

	void UpdateShotingState(float elapsedTime);



private:
	static Player* instance;

	ProjectileManager projectileManager;
	BrokenProjectileManager brokenprojectileManager;

	bool InputProjectile();

private:
	//�A�j���[�V����
	enum Animation
	{
		Anim_Idle,
		Anim_Walking,
		Anim_GetHit1,
		Anim_Death,
		Anim_Running,
		Anim_Climb,
		Anim_Landing,
		Anim_Jump,
		Anim_ClimbUpWall,
		Anim_ClimbDown,
		Anim_HoldInWall,
		Anim_Attack,
		Anim_Attack2,
		Anim_Kick,
		Anim_StartSwing,
		Anim_Swinging,
		Anim_Swinging2,
		Anim_Shoting,
	};

	// ���b�N�I���X�e�[�g
	enum class LockonState
	{
		NotLocked,
		Locked,
	};

private:
	DirectX::XMFLOAT3 GetMoveVec() const;

	float moveSpeed = 5.0f;

	float climbSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 20.0f;
	int jumpCount = 0;
	int jumpLimit = 1;
	std::unique_ptr<Effect> hitEffect = nullptr;

	std::unique_ptr<AudioSource> outOfBullets = nullptr;
	std::unique_ptr<AudioSource> walk = nullptr;
	
	State state = State::Idle;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	float maxAngleX = DirectX::XMConvertToRadians(35);
	float mixAngleX = DirectX::XMConvertToRadians(-35);


	std::vector<Model::NodePose> nodePoses;

	float HandRadius = 0.4f;

	//�U���̉�
	int attackCount = 0;
	int attackLimit = 3;
	float attackTimer = 0;

	bool attacking = false;


	LockonState			lockonState = LockonState::NotLocked;
	float				lockonTargetChangeTime = 0;
	float				lockonTargetChangeTimeMax = 8;
	Character*			lockonEnemy = nullptr;
	DirectX::XMFLOAT3	lockDirection;

private:
	//���p
	float swingTime = 0.0f;
	float swingAngle = 0.0f;
	DirectX::XMFLOAT3 swingPoint = { 0.0f, 10.0f, 0.0f };
	float angularVelocity;
	std::unique_ptr<Model> SwingWeb;
	float shotingTimer = 0.0f;

};

