#pragma once

#include <DirectXMath.h>
#include <vector>

#include "MessageData.h"
#include "Messenger.h"

class CameraController
{
	// ���[�h
	enum class Mode
	{
		FreeCamera,		// �t���[�J����
		LockonCamera,	// ���b�N�I���J����
		MotionCamera,	// ���[�V�����J����
	};

public:
	CameraController();
	~CameraController();

	static CameraController& Instance() 
	{ 
		static CameraController instance;
		return instance; 
	}


	// �X�V����
	void Update(float elapsedTime);

	// �^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    //  �J�����̑J��
	void SetTransition(bool Active) { this->isTransition = Active; }

private:
	// �t���[�J�����X�V����
	void FreeCamera(float elapsedTime);

	// ���b�N�I���J�����X�V����
	void LockonCamera(float elapsedTime);

	// ���[�V�����J�����X�V����
	void MotionCamera(float elapsedTime);
private:
	// �t���[�J����
	void OnFreeMode(void* data);

	// ���b�N�I���J����
	void OnLockonMode(void* data);

	// ���[�V�����J����
	void OnMotionMode(void* data);

	// �J�����h��
	void OnShake(void* data);

	// �����̃Y�������Z�o
	float CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);

private:
	Mode mode = Mode::FreeCamera;
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	float rollSpeed = DirectX::XMConvertToRadians(30);
	float range = 10.0f;

	float maxAngle = DirectX::XMConvertToRadians(45);
	float minAngle = DirectX::XMConvertToRadians(5);

	// ���b�N�I������
	DirectX::XMFLOAT3	newPosition = { 0, 0, 0 };
	DirectX::XMFLOAT3	newTarget = { 0, 0, 0 };
	DirectX::XMFLOAT3	targetWork[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : ���W, 1 : �����_
	float				lengthLimit[2] = { 5, 7 };
	float				sideValue = 1;
	// ���[�V�����J����
	float				motionTimer = 0;
	std::vector<CameraMotionData>	motionData;
	//�J�����V�F�C�N
	float				shakeTimer = 0;
	float				shakePower = 1;

    bool isTransition = false;

	// ���b�Z�[�W�L�[
	uint64_t			CAMERACHANGEFREEMODEKEY;
	uint64_t			CAMERACHANGELOCKONMODEKEY;
	uint64_t			CAMERACHANGEMOTIONMODEKEY;
	uint64_t			CAMERASHAKEKEY;
};

