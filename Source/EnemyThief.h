#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "Telegram.h"

#define USESTATEMACHINE3



// �G�l�~�[�A�j���[�V����
enum class EnemyAnimation
{
	Idle,
	Walk,
	Run,
	AttackShot,
	Die,
	GetHit,
	AttackPunch,
	GetThrow,
	HoldGun,
	KickDown,
};

class StateMachine;

class EnemyThief :public Enemy
{
public:
	EnemyThief();
	~EnemyThief()override;

    // �U���^�C�v
	enum class AttackType
	{
		Null,
		Punch,
		Shot,
	};
	AttackType randomType = AttackType::Null;


	// �X�V����
	void Update(float elapsedTime)override;

	//// �`�揈��
	//void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI();
	// �^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();

	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);

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
	float GetPunchRange() { return punchRange; }

	//2�T��
	//�X�e�[�g�}�V���擾
	StateMachine* GetStateMachine() { return stateMachine; }
	//���f���擾
	Model* GetModel() { return model.get(); }

	//	5�T�ځ@
	//TODO 05_02 ���b�Z�[�W��M�֐���ǉ�
	bool OnMessage(const Telegram& msg);

	void SetThrown(bool thrown) { isThrown = thrown; }
	bool IsThrown() const { return isThrown; }

private:

	// ���S�X�e�[�g�֑J��
	void TransitionDeathState();

	// ���S�X�e�[�g�X�V����
	void UpdateDeathState(float elapsedTime);

	// �_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();
    // �_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);


protected:
	void OnDead();


public:
	enum class State
	{
		Search,
		Battle,
		// MetaAI���烁�b�Z�[�W����M�����Ƃ��̃X�e�[�g��ǉ�
		Recieve,
		Dead,

		Damage,
	};

	enum class Search
	{
		Wander,
		Idle,
	};

	enum class Battle
	{
		Pursuit,
		Attack,
		Punch,
		Shot,
		// �퓬���ɍU�����������Ă��Ȃ��Ƃ��̏�����ǉ�
		Standby,
		Dead,
	};

	//TODO ���o�R���đ��̃G�l�~�[����Ă΂ꂽ�Ƃ��̏���
	enum class Recieve
	{
		Called,
	};

	float				stateTimer = 0.0f;


private:
	std::unique_ptr<Model> model;
	State				state = State::Search;

	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				moveSpeed = 3.5f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);

	float				searchRange = 7.0f;
	float				attackRange = 10.0f;
	float				punchRange = 1.5f;
	StateMachine* stateMachine = nullptr;


	float timeElapsed = 0.0f;
	float totalFlightTime;
	float webTimer;
	bool ThrowFlag;


};