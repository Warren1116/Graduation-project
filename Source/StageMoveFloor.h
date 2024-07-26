#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class StageMoveFloor : public Stage
{
public:
    StageMoveFloor();
    ~StageMoveFloor();

    void Update(float elapsedTime) override;

    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

    void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

    void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }
private:
    void UpdateTransform();
private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

    DirectX::XMFLOAT3 torque = { 0,0,0 };
    DirectX::XMFLOAT3 start = { 0,0,0 };
    DirectX::XMFLOAT3 goal = { 0,0,0 };
    
    float moveSpeed = 2.0f;
    float moveRate = 0.0f;

    DirectX::XMFLOAT4X4 oldTransform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
};
