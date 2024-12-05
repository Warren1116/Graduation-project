#include "Collision.h"
#include "StageMain.h"

bool Collision::IntersectSphereVsSphere2D(const DirectX::XMFLOAT2& positionA, float radiusA, const DirectX::XMFLOAT2& positionB, float radiusB)
{
    float vx = positionB.x - positionA.x;
    float vz = positionB.y - positionA.y;
    float range = radiusA + radiusB;
    float distXY = sqrtf(vx * vx + vz * vz);

    if (distXY <= range)
    {
        return true;
    }
}

bool Collision::IntersectPointVsRectXZ(const float pointPositionX, const float pointPositionZ, const float rectPositionX, const float rectPositionZ, float top, float bottom, float left, float right)
{
    if (rectPositionX + right > pointPositionX &&
        rectPositionX - left < pointPositionX &&
        rectPositionZ + top > pointPositionZ &&
        rectPositionZ - bottom < pointPositionZ)
    {
        return true;
    }

    return false;
}

// ���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionA,
    DirectX::XMFLOAT3& outPositionB)
{
    // B��A�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ��������
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    Vec = DirectX::XMVector3Normalize(Vec);
    // �����蔻��Ɏ��ʓ���t�^���ďd�ݔ�����s���Əd����\�����₷��
    Vec = DirectX::XMVectorScale(Vec, (range - lengthSq) * 0.5f);
    PositionA = DirectX::XMVectorSubtract(PositionA, Vec);
    PositionB = DirectX::XMVectorAdd(PositionB, Vec);
    DirectX::XMStoreFloat3(&outPositionA, PositionA);
    DirectX::XMStoreFloat3(&outPositionB, PositionB);
    return true;
}

bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }

    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    float range = radiusA + radiusB;
    float distXZ = sqrtf(vx * vx + vz * vz);

    if (distXZ > range)
    {
        return false;
    }

    // A��B�������o��
    vx /= distXZ;
    vz /= distXZ;

    outPositionB.x = { positionA.x + (vx * range) };
    outPositionB.y = { positionB.y };
    outPositionB.z = { positionA.z + (vz * range) };

    return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
    {
        return false;
    }

    if (spherePosition.y + sphereRadius < cylinderPosition.y)
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float range = cylinderRadius + sphereRadius;
    float distXZ = sqrtf(vx * vx + vz * vz);

    if (distXZ > range)
    {
        return false;
    }

    // A��B�������o��
    vx /= distXZ;
    vz /= distXZ;

    outCylinderPosition.x = { spherePosition.x + (vx * range) };
    outCylinderPosition.y = { cylinderPosition.y };
    outCylinderPosition.z = { spherePosition.z + (vz * range) };

    return true;
}


bool Collision::IntersectRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
{
    using namespace DirectX;

    // ������
    XMVECTOR WorldStart = XMLoadFloat3(&start);
    XMVECTOR WorldEnd = XMLoadFloat3(&end);
    XMVECTOR WorldRayVec = XMVectorSubtract(WorldEnd, WorldStart);
    float distance = XMVectorGetX(XMVector3Length(WorldRayVec));
    XMVECTOR Direction = XMVector3Normalize(WorldRayVec);
    result.distance = distance;
    bool hit = false;

    // 1. ��ԕ����iAABB�̈�̌����j
    const auto& collisionMesh = StageMain::Instance().GetCollisionMesh();

    for (const auto& area : collisionMesh.areas) {
        float aabbDist = distance;
        if (!area.boundingBox.Intersects(WorldStart, Direction, aabbDist)) continue;

        // �G���A���̎O�p�`������
        for (const auto& triangleIndex : area.triangleIndices) {
            const auto& triangle = collisionMesh.triangles[triangleIndex];
            XMVECTOR A = XMLoadFloat3(&triangle.positions[0]);
            XMVECTOR B = XMLoadFloat3(&triangle.positions[1]);
            XMVECTOR C = XMLoadFloat3(&triangle.positions[2]);

            float triDist = distance;  // �����l��ݒ�
            if (TriangleTests::Intersects(WorldStart, Direction, A, B, C, triDist)) {
                if (triDist < result.distance) {  // �ł��߂��Փ˓_���X�V
                    result.distance = triDist;
                    DirectX::XMStoreFloat3(&result.position, XMVectorAdd(WorldStart, XMVectorScale(Direction, triDist)));
                    result.normal = triangle.normal;  // �O�p�`�̖@�����R�s�[
                    hit = true;
                }
            }
        }
    }

    return hit;
}

