#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "Telegram.h"

#define USESTATEMACHINE3

// �G�l�~�[�A�j���[�V����
enum class EnemyAnimation
{
	IdleNormal,
	IdleBattle,
	Attack01,
	Attack02,
	WalkFWD,
	WalkBWD,
	WalkLeft,
	WalkRight,
	RunFWD,
	SenseSomethingST,
	SenseSomethingRPT,
	Taunt,
	Victory,
	GetHit,
	Dizzy,
	Die,
};

class StateMachine;

class EnemyPeople :public Enemy
{
public:
	EnemyPeople();
	~EnemyPeople()override;

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

	//2�T��
	//�X�e�[�g�}�V���擾
	StateMachine* GetStateMachine() { return stateMachine; }
	//���f���擾
	Model* GetModel() { return model; }

	//	5�T�ځ@
	//TODO 05_02 ���b�Z�[�W��M�֐���ǉ�
	bool OnMessage(const Telegram& msg);

private:

	// �p�j�X�e�[�g�֑J��
	void TransitionWanderState();

	// �p�j�X�e�[�g�X�V����
	void UpdateWanderState(float elapsedTime);

	// �ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	// �ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	// �ǐՃX�e�[�g�֑J��
	void TransitionPursuitState();

	// �ǐՃX�e�[�g�X�V����
	void UpdatePursuitState(float elapsedTime);

	// �U���X�e�[�g�֑J��
	void TransitionAttackState();

	// �U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);
protected:
	void OnDead();

	// TODO 01 �K�v�ȃX�e�[�g��enum class�ŏ������Ȃ����B
	// �iWander�̂݋L�q�ς݁j
	//TODO 02_01 ���N���X����enum class State�ɃA�N�Z�X�ł���悤�ɏC���q��ύX
	//private:
public:
#ifdef USESTATEMACHINE3

	enum class State
	{
		Search,
		Battle,
		//	TODO 05_02 MetaAI���烁�b�Z�[�W����M�����Ƃ��̃X�e�[�g��ǉ�
		Recieve,
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
		//TODO 05_02 �퓬���ɍU�����������Ă��Ȃ��Ƃ��̏�����ǉ�
		Standby,
	};

	//TODO 05_02 MetaAI���o�R���đ��̃G�l�~�[����Ă΂ꂽ�Ƃ��̏���
	enum class Recieve
	{
		Called,
	};

#endif // USESTATEMACHINE3
	float				stateTimer = 0.0f;



private:
	Model* model = nullptr;
#ifdef USESTATEMACHINE2
	State				state = State::Wander;
#endif // USESTATEMACHINE2

#ifdef USESTATEMACHINE3
	State				state = State::Search;
#endif // USESTATEMACHINE2

	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				moveSpeed = 3.0f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);

	float				searchRange = 5.0f;
	float				attackRange = 1.5f;
	//2�T�ڒǉ�
	StateMachine* stateMachine = nullptr;
};