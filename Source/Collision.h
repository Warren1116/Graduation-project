#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 normal = { 0, 0, 0 };
    float distance = 0.0f;
    int materialIndex = -1;
    DirectX::XMFLOAT3 rotation = { 0, 0 , 0 };
};


// ÉRÉäÉWÉáÉì
class Collision
{
public:
    static bool IntersectSphereVsSphere2D(
        const DirectX::XMFLOAT2& positionA,
        float radiusA,
        const DirectX::XMFLOAT2& positionB,
        float radiusB
    );

    // ì_Ç∆éläpå`ÇÃåç∑îªíË
    static bool IntersectPointVsRectXZ(
        const float pointPositionX,
        const float pointPositionZ,
        const float rectPositionX,
        const float rectPositionZ,
        float top,
        float bottom,
        float left,
        float right
    );

    // ãÖÇ∆ãÖÇÃåç∑îªíË
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionA,
        DirectX::XMFLOAT3& outPositionB);

    // â~íåÇ∆â~íåÇÃåç∑îªíË
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA, float radiusA, float heightA,
        const DirectX::XMFLOAT3& positionB, float radiusB, float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // ãÖÇ∆â~íåÇÃåç∑îªíË
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};

