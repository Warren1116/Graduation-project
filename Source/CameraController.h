#pragma once

#include <DirectXMath.h>
class CameraController
{
public:
    CameraController(){}
    ~CameraController(){}

    //唯一のインスタンス取得
    static CameraController& Instance()
    {
        static CameraController cameraController;
        return cameraController;
    }

    //更新処理
    void Update(float elapsedTime);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    //
    void ShakeCamera(float intensity, float duration);

private:
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float range = 10.0f;

    float maxAngle = DirectX::XMConvertToRadians(45);
    float minAngle = DirectX::XMConvertToRadians(-45);

    float shakeTime = 0;
    float shakeIntensity = 0;

};
