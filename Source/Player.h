#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "Audio/Audio.h"
#include "Object.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "CameraController.h"
#include "ProjectileManager.h"




// �v���C���[
class Player : public Character
{
public:
	Player(bool flag);
	~Player();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc, ModelShader* shader);

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �ړ����͏���
	bool InputMove(float elapsedTime);

	// �W�����v���͏���
	bool InputJump();

	void OnLanding() override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	void SetHaveKey(const bool& haveKey) { this->haveKey = haveKey; }
	bool HaveKey() const { return haveKey; }

	void SetHaveAmmoNum(const int haveAmmoNum) { this->haveAmmoNum = haveAmmoNum; }
	int GetHaveAmmoNum() const { return haveAmmoNum; }

	bool NearStairs() const { return nearStairs; }

	// �C���X�^���X�擾
	static Player& Instance();

	Model* model = nullptr;

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
	};


	// �X�e�[�g�擾
	const State& GetState() const { return state; }
	const State& GetAttackState() const { return State::Attack; }

	std::unique_ptr<AudioSource> key = nullptr;
	std::unique_ptr<AudioSource> ammo = nullptr;

protected:

	// ���S�����Ƃ��ɌĂ΂��
	void OnDamaged() override;

	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

private:
	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

	// �ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	// �ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	// �ړ��X�e�[�g�֑J��
	void TransitionMoveState();

	// �ړ��X�e�[�g�X�V����
	void UpdateMoveState(float elapsedTime);

	void TransitionJumpState();

	void UpdateJumpState(float elapsedTime);

	void TransitionClimbWallState();

	void UpdateClimbWallState(float elapsedTime);

	// �U�����͏���
	bool InputAttack();

	// �U���X�e�[�g�֑J��
	void TransitionAttackState();

	// �U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);

	// �����[�h�X�e�[�g�֑J��
	void TransitionReloadState();

	// �����[�h�X�e�[�g�X�V����
	void UpdateReloadState(float elapsedTime);

	// �_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();

	// �_���[�W�X�e�[�g�X�V����
	void UpdateDamageState(float elapsedTime);

	// ���S�X�e�[�g�֑J��
	void TransitionDeathState();

	// ���S�X�e�[�g�X�V����
	void UpdateDeathState(float elapsedTime);

	void TransitionLandState();

	void UpdateLandState(float elapsedTime);


	// �m�[�h�ƃG�l�~�[�̏Փˏ���
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	void PlayAttackAnimation();

private:
	ProjectileManager projectileManager;

	void InputProjectile();

private:
	//�A�j���[�V����
	enum Animation
	{
		Anim_Idle,
		Anim_Walking,
		Anim_Attack,
		Anim_Attack2,
		Anim_Kick,
		Anim_GetHit1,
		Anim_Death,
		Anim_Running,
		Anim_Climb,
		Anim_Landing,
		Anim_Jump,

	};

private:
	DirectX::XMFLOAT3 GetMoveVec() const;

	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(10);
	float jumpSpeed = 20.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	int haveAmmoNum = 50;
	Effect* hitEffect = nullptr;
	Effect* blowEffect = nullptr;
	std::unique_ptr<AudioSource> outOfBullets = nullptr;
	std::unique_ptr<AudioSource> walk = nullptr;
	
	State state = State::Idle;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	float maxAngleX = DirectX::XMConvertToRadians(35);
	float mixAngleX = DirectX::XMConvertToRadians(-35);
	bool moveCursorFlag = true;
	bool haveKey = false;
	bool nearStairs = false;

	Goal* goal;

	std::vector<Model::NodePose> nodePoses;

	float HandRadius = 0.4f;

	//�U���̉�
	int attackCount = 0;
	int attackLimit = 3;
	float attackTimer = 0;

	bool attacking = false;

};

