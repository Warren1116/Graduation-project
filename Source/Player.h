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
#include "Enemy.h"
#include "EnemyThief.h"


// �v���C���[
class Player : public Character
{
private:
	struct CollisionMesh
	{
		struct Triangle
		{
			DirectX::XMFLOAT3	positions[3];
			DirectX::XMFLOAT3	normal;
		};
		std::vector<Triangle>	triangles;

		struct Area
		{
			DirectX::BoundingBox	boundingBox;
			std::vector<int>		triangleIndices;
		};
		std::vector<Area>		areas;
	};
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

	//�@�e�ۂ̓��͏���
	bool InputProjectile();

	// �U�����͏���
	bool InputAttack();

	// Dodge���͏���
	bool InputDodge();

	// ���b�N�I���X�e�[�g
	enum class LockonState
	{
		NotLocked,
		Locked,
	};

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//	�e�ۂ̃}�l�[�W���[�����
	ProjectileManager& GetProjectileManager() { return projectileManager; }
	//	�j�󂵂��e�ۂ̃}�l�[�W���[�����i���j
	BrokenProjectileManager& GetBrokenProjectileManager() { return brokenprojectileManager; }
	//	�v���C���[�̃��b�N�X�e�[�g�����
	LockonState GetlockonState() { return lockonState; }
	//�@���b�N���̓G���擾
	Enemy* GetLockonEnemy() const { return static_cast<Enemy*>(lockonEnemy); }
	//	�X�C���O�ʒu���擾�i���j
	DirectX::XMFLOAT3 GetswingPoint() { return swingPoint; }

	bool GetFirstSwing() { return firstSwing; }

	//	�L�^���ꂽ�X�C���O�|�C���g���擾
	DirectX::XMFLOAT3 GetPreviousSwingPoint() { return previousSwingPoint; }

    //	�U���\�m�t���O�����
	bool GetAttackSoon() { return getAttacksoon; }
	void SetgetAttackSoon(bool getattack) { getAttacksoon = getattack; }
    //  �U���\�m�t���O�����(Shot)
    bool GetShotSoon() { return getShotsoon; }
    void SetgetShotSoon(bool getshot) { getShotsoon = getshot; }


	//�@���U������̓G���擾����	
	EnemyThief* GetAttackEnemy() { return attackEnemy; }
    void SetAttackEnemy(EnemyThief* enemy) { attackEnemy = enemy; }

    //    �����Z�t���O�����
	bool GetIsUseGrab() { return IsUseGrab; }
    //    �X�C���O�L�b�N�t���O�����
    bool GetIsUseSwingKick() { return IsUseSwingKick; }

	std::unique_ptr<Model> model = nullptr;

	//  �����˂̃^�C�}�[
	float GetWebTimer() const { return webTimer; }

    void SetIdleState() { return TransitionIdleState(); }

    void SetVelocity(const DirectX::XMFLOAT3& velocity) { this->velocity = velocity; }


	// �X�e�[�g
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
		ClimbTop,
		Grab,
		CrouchIdle,
		TitleIdle,
		SwingToKick,

		EventMode,
	};


	// �X�e�[�g�擾
	const State& GetState() const { return state; }
	const State& GetAttackState() const { return State::Attack; }

	float GetSkillTime() const { return skillTime; }

	struct Joint
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	oldPosition;
	};
	Joint		joints[5];

protected:

	// �_���[�W���󂯂��Ƃ��ɌĂ΂��
	void OnDamaged() override;

	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

	//�@���n�̎��ɌĂ΂��
	void OnLanding() override;

private:
	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();
	//	�e�ۂƓG�Ƃ̏Փˏ���
	void CollisionProjectileVsEnemies();

	// �m�[�h�ƃG�l�~�[�̏Փˏ���
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

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

	// �X�C���O�X�e�[�g�֑J��
	void TransitionSwingState();

	// �X�C���O�X�e�[�g�X�V����
	void UpdateSwingState(float elapsedTime);

	// �J�����X�e�[�g�̍X�V
	void UpdateCameraState(float elapsedTime);

	//	���˃X�e�[�g���֑J��
	void TransitionShotingState();

	//	���˃X�e�[�g�X�V����
	void UpdateShotingState(float elapsedTime);

	//	�A���U���̃��[�V����
	void PlayAttackAnimation();

	//	����X�e�[�g�֑J��
	void TransitionDodgeState();

	//	����X�e�[�g�̍X�V
	void UpdateDodgeState(float elapsedTime);

	//	�o��X�e�[�g�֑J��
	void TransitionClimbTopState();

	//	�o��X�e�[�g�̍X�V
	void UpdateClimbTopState(float elapsedTime);

	//	�����Z�X�e�[�g�֑J��
	void TransitionGrabState();

	//	�����Z�X�e�[�g�̍X�V
	void UpdateGrabState(float elapsedTime);

	//	�ǒ[�����̔���
	bool IsNearWallTop();

	//	�X�C���O�|�C���g��T��
	bool FindWallSwingPoint();

	bool ActiveGrabWeb(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos);


    //Title�X�e�[�g�p
    void TransitionTitleIdleState();
    void UpdateTitleIdleState(float elapsedTime);

    void TransitionCrouchIdleState();
    void UpdateCrouchIdleState(float elapsedTime);

    void TransitionSwingToKickState();
    void UpdateSwingToKickState(float elapsedTime);


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
		Anim_HoldInWall,
		Anim_Attack,
		Anim_Attack2,
		Anim_Kick,
		Anim_StartSwing,
		Anim_Swinging,
		Anim_Swinging2,
		Anim_Shoting,
		Anim_Dodge,
		Anim_ClimbUpWall,
		Anim_ClimbDown,
		Anim_SwingToLand,
		Anim_GrabAndDrop,
		Anim_CrouchIdle,
		Anim_TitleIdle,
		Anim_Crouch,
		Anim_SwingToKick,
	};


private:
	// �B���instance
	static Player* instance;
	ProjectileManager projectileManager;
	BrokenProjectileManager brokenprojectileManager;

private:
	// �ړ��x�N�g���擾
	DirectX::XMFLOAT3 GetMoveVec() const;
	//�@�ړ��X�r�[�g
	float moveSpeed = 5.0f;
	//	�N���C�~���O�X�r�[�g
	float climbSpeed = 5.0f;
	//	��]�X�r�[�g
	float turnSpeed = DirectX::XMConvertToRadians(720);
	//�@�W�����u�X�r�[�g
	float jumpSpeed = 16.0f;
	//�@�q�b�g�G�t�F�N�g
	std::unique_ptr<Effect> hitEffect = nullptr;
	std::unique_ptr<AudioSource> punch = nullptr;
	std::unique_ptr<AudioSource> punch2 = nullptr;
	std::unique_ptr<AudioSource> kick = nullptr;


	// �X�e�[�g
	State state = State::Idle;
	bool attackCollisionFlag = false;
	float maxAngleX = DirectX::XMConvertToRadians(35);
	float mixAngleX = DirectX::XMConvertToRadians(-35);
	State lastState;

	//���f���m�[�h
	std::vector<Model::NodePose> nodePoses;

	// �U�����a
	float attackRadius = 0.4f;
	// �U���̉�
	int attackCount = 0;
	int attackLimit = 3;
	float attackTimer = 0;
	//	�U������
	bool attacking = false;
	//	�U���\�m
	bool getAttacksoon = false;
    bool getShotsoon = false;

    //	�U���Ώ�
    EnemyThief* attackEnemy = nullptr;

	//	�����Z
	bool IsUseGrab = false;

    //�@�X�C���O�L�b�N
    bool IsUseSwingKick = false;

    DirectX::XMFLOAT3 dodgeDirection;

	//�J�������b�N�p
	LockonState			lockonState = LockonState::NotLocked;
	float				lockonTargetChangeTime = 0;
	float				lockonTargetChangeTimeMax = 8;
	Character*			lockonEnemy = nullptr;
	DirectX::XMFLOAT3	lockDirection;
	const float MAX_LOCKON_DISTANCE = 15.0f;

private:
	//�Ǔ����蔻��̃`�F�b�N
	DirectX::XMFLOAT3 checkpos;
	DirectX::XMVECTOR checkDirection;

	//���p
	//	�X�C���O�|�C���g�̈ʒu
	DirectX::XMFLOAT3 swingPoint;
	//	�X�C���O�|�C���g�̕���
	DirectX::XMFLOAT3 swingwebDirection;
	DirectX::XMVECTOR SwingwebDirection;
	//	����̃X�C���O�𔻒�
	bool firstSwing = true;
	float SwingCounter;
	//	�O��̃X�C���O�|�C���g���L�^
	DirectX::XMFLOAT3 previousSwingPoint;
	// ���̃^�C�}�[
	float webTimer;
	float skillTime;

private:
	// ���b�Z�[�W�L�[
	uint64_t	EventModeIndex = -1;
	uint64_t	GameModeIndex = -1;

	uint64_t	SetPositionIndex = -1;
	uint64_t	MoveIndex = -1;
	struct
	{
		DirectX::XMFLOAT3 moveVec;
		float moveSpeed;
		float turnSpeed;
	} moveData;
	uint64_t	MotionIndex = -1;

};

