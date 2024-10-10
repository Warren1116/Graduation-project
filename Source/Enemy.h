#pragma once
#include "Graphics/Shader.h"
#include "Character.h"

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

	virtual float PlayerToBoss() { return 0; }

	void SetCameraFlg(bool flg) { camerashake = flg; }
	float GetCameraFlg() { return camerashake; }

	Model* GetModel() const { return model; }

protected:
	Model* model = nullptr;
	bool		camerashake = false;
	EnemyManager* manager = nullptr;
};

