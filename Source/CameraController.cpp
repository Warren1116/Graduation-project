#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = (mouse.GetPositionX() - mouse.GetOldPositionX());
    float ay = (mouse.GetPositionY() - mouse.GetOldPositionY());

    //カメラの回転速度
    float speed = rollSpeed * elapsedTime;



    //もしシェイクの時間が終わってないなら、シェイクを行う
    if (shakeTime > 0.0f)
    {
        float shakeX = (rand() % 100 - 50) / 50.0f * shakeIntensity;
        float shakeY = (rand() % 100 - 50) / 50.0f * shakeIntensity;

        //カメラに応用
        angle.x += shakeX;
        angle.y += shakeY;

        //シェイクの時間更新
        shakeTime -= elapsedTime;

        //シェイクの強さを減らす
        shakeIntensity *= 0.9f;
    }
    else
    {
        //スティックの入力値に合わせてX軸とY軸を回転
        angle.x += ay * speed;
        angle.y += ax * speed;
    }


    //カメラ回転を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    if (angle.x > maxAngle)
    {
        angle.x = maxAngle;
    }
    if (angle.x < minAngle)
    {
        angle.x = minAngle;
    }
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::ShakeCamera(float intensity, float duration)
{
    shakeTime = duration;
    shakeIntensity = intensity;
}
