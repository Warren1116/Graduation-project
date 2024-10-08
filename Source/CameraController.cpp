#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = (mouse.GetPositionX() - mouse.GetOldPositionX());
    float ay = (mouse.GetPositionY() - mouse.GetOldPositionY());

    //�J�����̉�]���x
    float speed = rollSpeed * elapsedTime;



    //�����V�F�C�N�̎��Ԃ��I����ĂȂ��Ȃ�A�V�F�C�N���s��
    if (shakeTime > 0.0f)
    {
        float shakeX = (rand() % 100 - 50) / 50.0f * shakeIntensity;
        float shakeY = (rand() % 100 - 50) / 50.0f * shakeIntensity;

        //�J�����ɉ��p
        angle.x += shakeX;
        angle.y += shakeY;

        //�V�F�C�N�̎��ԍX�V
        shakeTime -= elapsedTime;

        //�V�F�C�N�̋��������炷
        shakeIntensity *= 0.9f;
    }
    else
    {
        //�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
        angle.x += ay * speed;
        angle.y += ax * speed;
    }


    //�J������]����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
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

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::ShakeCamera(float intensity, float duration)
{
    shakeTime = duration;
    shakeIntensity = intensity;
}
