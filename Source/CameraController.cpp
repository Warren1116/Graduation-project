#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "Player.h"
#include <imgui.h>
#include "EnemyManager.h"
#include <string>
#include "StageManager.h"


CameraController::CameraController()
{
    position = Camera::Instance().GetEye();
    newPosition = Camera::Instance().GetEye();
    CAMERACHANGEFREEMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEFREEMODE, [&](void* data) { OnFreeMode(data); });
    CAMERACHANGELOCKONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGELOCKONMODE, [&](void* data) { OnLockonMode(data); });
    CAMERACHANGEMOTIONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEMOTIONMODE, [&](void* data) { OnMotionMode(data); });
    CAMERASHAKEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERASHAKE, [&](void* data) { OnShake(data); });

}

CameraController::~CameraController()
{
    Messenger::Instance().RemoveReceiver(CAMERACHANGEFREEMODEKEY);
    Messenger::Instance().RemoveReceiver(CAMERACHANGELOCKONMODEKEY);
    Messenger::Instance().RemoveReceiver(CAMERACHANGEMOTIONMODEKEY);
    Messenger::Instance().RemoveReceiver(CAMERASHAKEKEY);
}

void CameraController::Update(float elapsedTime)
{
    // 各モードでの処理
    switch (mode)
    {
    case	Mode::FreeCamera:	FreeCamera(elapsedTime);	break;
    case	Mode::LockonCamera:	LockonCamera(elapsedTime);	break;
    case	Mode::MotionCamera:	MotionCamera(elapsedTime);	break;
    }

    // カメラ揺れ
    if (shakeTimer > 0)
    {
        newPosition.x += (rand() % 3 - 1) * shakePower;
        newPosition.y += (rand() % 3 - 1) * shakePower;
        newPosition.z += (rand() % 3 - 1) * shakePower;
        shakeTimer -= elapsedTime;
    }

    // 地形との当たり判定を行う
    HitResult	hitResult;
    if (StageManager::Instance().RayCast(newTarget, newPosition, hitResult))
    {
        DirectX::XMVECTOR	p = DirectX::XMLoadFloat3(&hitResult.position);
        DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
        p = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(4), cuv, p);
        DirectX::XMStoreFloat3(&newPosition, p);
    }
    static	constexpr	float	Speed = 1.0f / 8.0f;
    position.x += (newPosition.x - position.x) * Speed;
    position.y += (newPosition.y - position.y) * Speed;
    position.z += (newPosition.z - position.z) * Speed;
    target.x += (newTarget.x - target.x) * Speed;
    target.y += (newTarget.y - target.y) * Speed;
    target.z += (newTarget.z - target.z) * Speed;


    // カメラに視点を注視点を設定
    Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));


}

void CameraController::FreeCamera(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = (mouse.GetPositionX() - mouse.GetOldPositionX());
    float ay = (mouse.GetPositionY() - mouse.GetOldPositionY());

    float stickX = gamePad.GetAxisRX(); 
    float stickY = -gamePad.GetAxisRY(); 

    float stickSensitivity = 0.008f; 
    ax += stickX * stickSensitivity * Graphics::Instance().GetScreenWidth();
    ay += stickY * stickSensitivity * Graphics::Instance().GetScreenHeight();

    float speed = rollSpeed * elapsedTime;


    {
        angle.x += ay * speed;
        angle.y += ax * speed;
    }

    if (angle.x > maxAngle) angle.x = maxAngle;
    if (angle.x < minAngle) angle.x = minAngle;
    if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
    if (angle.y > DirectX::XM_PI) angle.y -= DirectX::XM_2PI;

    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    newPosition.x = target.x - front.x * range;
    newPosition.y = target.y - front.y * range;
    newPosition.z = target.z - front.z * range;
}

void CameraController::LockonCamera(float elapsedTime)
{
    //	後方斜に移動させる
    DirectX::XMVECTOR	t0 = DirectX::XMVectorSet(targetWork[0].x, 0.5f, targetWork[0].z, 0);
    DirectX::XMVECTOR	t1 = DirectX::XMVectorSet(targetWork[1].x, 0.5f, targetWork[1].z, 0);
    DirectX::XMVECTOR	crv = DirectX::XMLoadFloat3(&Camera::Instance().GetRight());
    DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
    DirectX::XMVECTOR	v = DirectX::XMVectorSubtract(t1, t0);
    DirectX::XMVECTOR	l = DirectX::XMVector3Length(v);

    t0 = DirectX::XMLoadFloat3(&targetWork[0]);
    t1 = DirectX::XMLoadFloat3(&targetWork[1]);

    //	新しい注視点を算出
    DirectX::XMStoreFloat3(&newTarget, DirectX::XMVectorMultiplyAdd(v, DirectX::XMVectorReplicate(0.5f), t0));

    //	新しい座標を算出
    l = DirectX::XMVectorClamp(l
        , DirectX::XMVectorReplicate(lengthLimit[0])
        , DirectX::XMVectorReplicate(lengthLimit[1]));
    t0 = DirectX::XMVectorMultiplyAdd(l, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), t0);
    t0 = DirectX::XMVectorMultiplyAdd(crv, DirectX::XMVectorReplicate(sideValue * 3.0f), t0);
    t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(3.0f), t0);
    DirectX::XMStoreFloat3(&newPosition, t0);

}

void CameraController::MotionCamera(float elapsedTime)
{
    if (motionData.empty())
        return;

    motionTimer += elapsedTime * 60;
    if (motionData.size() == 1)
    {
        if (motionTimer >= motionData[0].time)
        {
            newPosition = motionData[0].position;
            newTarget = motionData[0].target;
            position = newPosition;
            target = newTarget;
        }
    }
    else
    {
        bool set = false;
        for (int i = 0; i < motionData.size() - 1; ++i)
        {
            if (motionData[i].time <= motionTimer && motionTimer < motionData[i + 1].time)
            {
                set = true;
                float	value = motionData[i + 1].time - motionData[i].time;
                value = (motionTimer - motionData[i].time) / value;
                newPosition = motionData[i].position;
                newPosition.x += (motionData[i + 1].position.x - motionData[i].position.x) * value;
                newPosition.y += (motionData[i + 1].position.y - motionData[i].position.y) * value;
                newPosition.z += (motionData[i + 1].position.z - motionData[i].position.z) * value;
                newTarget = motionData[i].target;
                newTarget.x += (motionData[i + 1].target.x - motionData[i].target.x) * value;
                newTarget.y += (motionData[i + 1].target.y - motionData[i].target.y) * value * 0.5f;
                newTarget.z += (motionData[i + 1].target.z - motionData[i].target.z) * value;
                position = newPosition;
                target = newTarget;
                break;
            }
        }
        if (!set)
        {
            if (motionTimer >= motionData[motionData.size() - 1].time)
            {
                newPosition = motionData[motionData.size() - 1].position;
                newTarget = motionData[motionData.size() - 1].target;
                position = newPosition;
                target = newTarget;
            }
        }
    }

}


void CameraController::OnFreeMode(void* data)
{
    MessageData::CAMERACHANGEFREEMODEDATA* p = static_cast<MessageData::CAMERACHANGEFREEMODEDATA*>(data);
    if (this->mode != Mode::FreeCamera)
    {
        // 角度算出
        DirectX::XMFLOAT3	v;
        v.x = newPosition.x - newTarget.x;
        v.y = newPosition.y - newTarget.y;
        v.z = newPosition.z - newTarget.z;
        angle.y = atan2f(v.x, v.z) + DirectX::XM_PI;
        angle.x = atan2f(v.y, v.z);
        //	角度の正規化
        angle.y = atan2f(sinf(angle.y), cosf(angle.y));
        angle.x = atan2f(sinf(angle.x), cosf(angle.x));
    }
    this->mode = Mode::FreeCamera;
    this->newTarget = p->target;
    this->newTarget.y += 0.01f;

}

void CameraController::OnLockonMode(void* data)
{
    MessageData::CAMERACHANGELOCKONMODEDATA* p = static_cast<MessageData::CAMERACHANGELOCKONMODEDATA*>(data);
    if (this->mode != Mode::LockonCamera)
        sideValue = CalcSide(p->start, p->target);

    this->mode = Mode::LockonCamera;
    targetWork[0] = p->start;
    targetWork[1] = p->target;
    targetWork[0].y += 0.01f;
    targetWork[1].y += 0.01f;
}

void CameraController::OnMotionMode(void* data)
{
    MessageData::CAMERACHANGEMOTIONMODEDATA* p = static_cast<MessageData::CAMERACHANGEMOTIONMODEDATA*>(data);
    if (this->mode != Mode::MotionCamera)
        motionTimer = 0;
    this->mode = Mode::MotionCamera;
    motionData.clear();
    motionData = p->data;
}

void CameraController::OnShake(void* data)
{
    MessageData::CAMERASHAKEDATA* p = static_cast<MessageData::CAMERASHAKEDATA*>(data);
    shakeTimer = p->shakeTimer;
    shakePower = p->shakePower;
}


float CameraController::CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
    // 外積を用いて横軸のズレ方向算出
    DirectX::XMFLOAT2	v;
    v.x = position.x - target.x;
    v.y = position.z - target.z;
    float	l = sqrtf(v.x * v.x + v.y * v.y);
    v.y /= l;
    v.y /= l;
    DirectX::XMFLOAT2	n;
    n.x = p1.x - p2.x;
    n.y = p1.z - p2.z;
    l = sqrtf(n.x * n.x + n.y * n.y);
    n.x /= l;
    n.y /= l;
    return	((v.x * n.y) - (v.y * n.x) < 0) ? +1.0f : -1.0f;

}
