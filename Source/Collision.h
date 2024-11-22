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


// コリジョン
class Collision
{
public:
    static bool IntersectSphereVsSphere2D(
        const DirectX::XMFLOAT2& positionA,
        float radiusA,
        const DirectX::XMFLOAT2& positionB,
        float radiusB
    );

    // 点と四角形の交差判定
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

    // 球と球の交差判定
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionA,
        DirectX::XMFLOAT3& outPositionB);

    // 円柱と円柱の交差判定
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA, float radiusA, float heightA,
        const DirectX::XMFLOAT3& positionB, float radiusB, float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // 球と円柱の交差判定
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
    
    // スフィアキャストVsモデル
    static bool Collision::IntersectSphereVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        float radius,
        const Model* model,
        HitResult& result);
        //SphereCastResult& result);

    // スフィアキャスト三角形
    static bool IntersectSphereVsTriangle(
        DirectX::XMVECTOR sphereStart,
        DirectX::XMVECTOR sphereDir,
        float radius,
        DirectX::XMVECTOR A,
        DirectX::XMVECTOR B,
        DirectX::XMVECTOR C,
        float& outDistance,
        DirectX::XMVECTOR& outContactPoint);

    // 三角形内にいるかどうかの判定
    static bool Collision::PointInTriangle(
        DirectX::XMVECTOR point,
        DirectX::XMVECTOR A,
        DirectX::XMVECTOR B,
        DirectX::XMVECTOR C);
};

