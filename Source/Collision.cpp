#include "Collision.h"

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

// 球と球の交差判定
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionA,
    DirectX::XMFLOAT3& outPositionB)
{
    // B→Aの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // 距離判定
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    Vec = DirectX::XMVector3Normalize(Vec);
    // 当たり判定に質量等を付与して重み判定を行うと重さを表現しやすい
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

    // XZ平面での範囲チェック
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z; 
    float range = radiusA + radiusB;
    float distXZ = sqrtf(vx * vx + vz * vz);
  
    if (distXZ > range)
    {
        return false;
    }

    // AがBを押し出す
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

    // XZ平面での範囲チェック
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float range = cylinderRadius + sphereRadius;
    float distXZ = sqrtf(vx * vx + vz * vz);

    if (distXZ > range)
    {
        return false;
    }

    // AがBを押し出す
    vx /= distXZ;
    vz /= distXZ;

    outCylinderPosition.x = { spherePosition.x + (vx * range) };
    outCylinderPosition.y = { cylinderPosition.y };
    outCylinderPosition.z = { spherePosition.z + (vz * range) };

    return true;
}

// レイキャスト
bool Collision::IntersectRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
{    
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    // ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // メッシュノード取得
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        // レイをワールド空間からローカル空間へ変換
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        // レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        // 三角形（面）との交差判定
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT> indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                // 三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // 三角形の法線ベクトルを算出
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB,BC);

                // 内積の結果がプラスならば裏向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float d;
                DirectX::XMStoreFloat(&d, Dot);
                if (d >= 0) continue;

                // レイと平面の交点を算出
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(SA, N), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
              
                // 交点までの距離が今までに計算した最近距離より大きいときはスキップ
                if (x < .0f || x > neart) continue; 

                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorMultiply(V, X));

                // 交点が三角形の内側にあるか判定
                // １つめ
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(N, Cross1);
                float d1;
                DirectX::XMStoreFloat(&d1, Dot1);
                if (d1 < 0.0f) continue;

                // ２つめ
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(N, Cross2);
                float d2;
                DirectX::XMStoreFloat(&d2, Dot2);
                if (d2 < 0.0f) continue;

                // ３つめ
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(N, Cross3);
                float d3;
                DirectX::XMStoreFloat(&d3, Dot3);
                if (d3 < 0.0f) continue;
    
                // 最近距離を更新
                neart = x;
    
                // 交点と法線を更新
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.materialIndex;
            }
        }

        if (materialIndex >= 0)
        {
            // ローカル空間からワールド空間へと変換
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition,
                WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            // ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal,
                    WorldTransform);

                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal,
                    DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}

bool Collision::IntersectSphereVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, float radius, const Model* model, HitResult& result)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // メッシュノード取得
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        // レイをワールド空間からローカル空間へ変換
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);

        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT> indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;

        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                float distance;
                DirectX::XMVECTOR contactPoint;
                if (IntersectSphereVsTriangle(S, SE, radius, A, B, C, distance, contactPoint))
                {
                    if (distance < result.distance)
                    {
                        result.distance = distance;
                        HitPosition = contactPoint;
                        HitNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(
                            DirectX::XMVectorSubtract(B, A),
                            DirectX::XMVectorSubtract(C, A)
                        ));
                        materialIndex = subset.materialIndex;
                        hit = true;
                    }
                }
            }
        }

        if (materialIndex >= 0)
        {
            DirectX::XMVECTOR WorldHitPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            DirectX::XMVECTOR WorldHitNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

            DirectX::XMStoreFloat3(&result.position, WorldHitPosition);
            DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldHitNormal));
            result.materialIndex = materialIndex;
        }
    }

    return hit;
}

bool Collision::IntersectSphereVsTriangle(DirectX::XMVECTOR sphereStart, DirectX::XMVECTOR sphereDir, float radius, DirectX::XMVECTOR A, DirectX::XMVECTOR B, DirectX::XMVECTOR C, float& outDistance, DirectX::XMVECTOR& outContactPoint)
{
    //  三角形の法線を計算
    DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
    DirectX::XMVECTOR AC = DirectX::XMVectorSubtract(C, A);
    DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(AB, AC));


    DirectX::XMVECTOR sphereToPlane = DirectX::XMVectorSubtract(A, sphereStart);
    float t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereToPlane, normal)) /
        DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereDir, normal));

    if (t < 0 || t > 1) return false;


    DirectX::XMVECTOR contactPoint = DirectX::XMVectorAdd(sphereStart, DirectX::XMVectorScale(sphereDir, t));
    DirectX::XMVECTOR sphereCenterToContact = DirectX::XMVectorSubtract(contactPoint, normal);

    float distToPlane = DirectX::XMVectorGetX(DirectX::XMVector3Length(sphereCenterToContact));

    if (distToPlane > radius) return false;

    if (PointInTriangle(contactPoint, A, B, C))
    {
        outDistance = t;
        outContactPoint = contactPoint;
        return true;
    }

    return false;
}

bool Collision::PointInTriangle(DirectX::XMVECTOR point, DirectX::XMVECTOR A, DirectX::XMVECTOR B, DirectX::XMVECTOR C)
{
    DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
    DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
    DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

    DirectX::XMVECTOR AP = DirectX::XMVectorSubtract(point, A);
    DirectX::XMVECTOR BP = DirectX::XMVectorSubtract(point, B);
    DirectX::XMVECTOR CP = DirectX::XMVectorSubtract(point, C);

    DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(AB, AP);
    DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(BC, BP);
    DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(CA, CP);

    float d1 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(Cross1, Cross2));
    float d2 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(Cross2, Cross3));

    return d1 >= 0.0f && d2 >= 0.0f;
}

