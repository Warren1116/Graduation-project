#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Telegram.h"

// �O���錾
class EnemyManager;

// �G�l�~�[
class Enemy : public Character
{
public:
	Enemy() {}
	~Enemy() override {}
	
	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �j��
	void Destroy();

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	virtual void DrawDebugGUI() {}

	//virtual float PlayerToBoss() { return 0; }

	//void SetCameraFlg(bool flg) { camerashake = flg; }
	//float GetCameraFlg() { return camerashake; }

	// �U���t���O�擾
	virtual bool	GetAttackFlg() { return attackFlg; }
	// �U���t���O�Z�b�g
	virtual void	SetAttackFlg(bool flg) { attackFlg = flg; };
	virtual void	SetId(int id) { this->id = id; }
	virtual int		GetId() { return id; }
	// �U���͈͎擾
	virtual float GetAttackRange() { return attackRange; }

	// ���b�Z�[�W��M�����Ƃ��̏���
	virtual bool OnMessage(const Telegram& msg);

	////���b�N�I������Ă��邩�ǂ���
	//virtual bool GetIsLockedOn() const { return IsLockedOn; }
	//virtual void SetLockedOn(bool lockStatus) { IsLockedOn = lockStatus;}

	Model* GetModel() const { return model; }

protected:
	Model* model = nullptr;
	bool		camerashake = false;
	EnemyManager* manager = nullptr;

	int	state = 0;
	bool attackFlg = false;
	int id = 0;
	float searchRange = 0.0f;
	float attackRange = 0.0f;

	//bool IsLockedOn = false;
};

