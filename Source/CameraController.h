#pragma once

#include <DirectXMath.h>


class CameraController
{
public:
	CameraController() {}
	~CameraController(){}

	static CameraController& Instance() 
	{ 
		static CameraController instance;
		return instance; 
	}


	// 更新処理
	void Update(float elapsedTime);

	// ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//const bool& GetMoveCursorFlag() const { return moveCursorFlag; }

	//bool& ChangeCursorLock() { return cursorLock = !cursorLock; }
	//const bool& GetCursorLock() const { return cursorLock; }

	//void SetCameraFlg(bool flg) { this->shakecamera = flg; }
	//bool GetCameraFlg() { return shakecamera; }


	//
	void ShakeCamera(float intensity, float duration);


public:
	DirectX::XMFLOAT3 angle = { 0,0,0 };
private:
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3	position = { 0,0,0 };
	float rollSpeed = DirectX::XMConvertToRadians(90);
	float range = 10.0f;

	float maxAngle = DirectX::XMConvertToRadians(45);
	float minAngle = DirectX::XMConvertToRadians(-45);
	float shakeTime = 0;
	float shakeIntensity = 0;

};

