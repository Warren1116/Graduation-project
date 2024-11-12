#pragma once

#include <DirectXMath.h>
#include "Mathf.h"
#include <vector>

class Character
{
public:
	Character() {};
	virtual ~Character() {};
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// �`�揈��
	//virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
	// �f�o�b�O�pGUI�`��
	virtual	void DrawDebugGUI() {}

	// �f�o�b�O�v���~�e�B�u�`��
	virtual	void DrawDebugPrimitive() {}

	// �s��X�V����
	void UpdateTransform();

	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	void SetTrianglePosition(const DirectX::XMFLOAT3& position, int index) { this->position = position; }

	const DirectX::XMFLOAT3& GetRight() const { return { transform._11, transform._12, transform._13 }; }
	const DirectX::XMFLOAT3& GetUp() const { return { transform._21, transform._22, transform._23 }; }
	const DirectX::XMFLOAT3& GetFront() const { return { transform._31, transform._32, transform._33 }; }
	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// ��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	// ��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	// �X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// ���a�擾
	float GetRadius() const { return radius; }

	// �����擾
	float GetHeight() const { return height; }

	DirectX::XMFLOAT3 GetVelocity() const { return velocity; }

	// �n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincibleTime);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// ���N��Ԃ��擾
	int GetHealth() const { return health; }

	// �ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return maxHealth; }

	// ��������t���O
	bool GetAlive() { return isAlive; }

	// �X�C���O���Ă�t���O
	bool GetOnSwing() { return onSwing; }
private:

	// �������͍X�V�����@
	void UpdateVerticalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);

	// �������͍X�V�����@
	void UpdateHorizontalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);

protected:
	// �ړ�����
	void Move(float vx, float vy, float vz, float speed);
	void Move(float vx, float vz, float speed);

	// ���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// �W�����v����
	void Jump(float speed);

	// ���x�����X�V
	void UpdateVelocity(float elapsedTime);

	// ���G���ԍX�V
	void UpdateInvincibleTimer(float elapsedTime);

	// ���n�����Ƃ��ɌĂ΂��
	virtual void OnLanding() {}

	// �_���[�W���󂯂��Ƃ��ɌĂ΂��
	virtual void OnDamaged() {}

	// ���S�����Ƃ��ɌĂ΂��
	virtual void OnDead() {}


protected:

protected:
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1,
	};
	DirectX::XMFLOAT4X4 rotation = {

	};
	float radius = 1.2f;
	float height = 1.9f;
	float gravity = -1.0f;
	DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
	bool isGround = false;
	int health = 15;
	int maxHealth = 5;
	float stepOffset = 1.5f;
	float invincibleTimer = 0.0f;
	float friction = 0.5f;
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float airControl = 0.3f;
	float slopeRate = 1.0f;

	float skinWidth = 0.01f;
	float slopeLimit = 45.0f;

	bool isAlive = true;

	//�@�v���C���[�p����
	bool hitWall = false;
	bool onClimb = false;
	bool CanClimb = false;
	bool onSwing = false;

	float fallStartHeight = 0.0f;
	float landingHeightThreshold = 1.0f;
};

