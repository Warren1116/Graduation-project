#include "StageMoveFloor.h"

StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
    delete model;
}

void StageMoveFloor::Update(float elapsedTime)
{
    //前回の情報を保存
    oldTransform = transform;
    oldAngle = angle;
    //スタートからゴールまでの距離を算出
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);

    //スタートからゴールまでの間を一秒間で進めむ割合 (0.0から1.0)を算出する
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    //ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    //線形補完で位置を算出する
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    //回転
    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    //行列更新
    UpdateTransform();

    //model->UpdateTransform(transform);
    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    model->UpdateTransform(transformIdentity);
}

void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform);
    shader->Draw(dc, model);
}

bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    //前回のワールド行列と逆行列を求める
    DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&oldTransform);
    DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

    //DirectX::XMMATRIX oldWorld = DirectX::XMLoadFloat4x4(&oldTransform);
    //oldWorld = DirectX::XMMatrixInverse(nullptr, oldWorld);

    //前回のローカル空間でのレイに変換
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);;
    DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
    DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);

    //DirectX::XMVECTOR localStartV = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&start), oldWorld);
    //DirectX::XMVECTOR loaclEndV = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&end), oldWorld);

    //ローカル空間でのレイとの交点を求める
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, LocalStart);
    DirectX::XMStoreFloat3(&localEnd, LocalEnd);

    HitResult localHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
    {
        //前回のローカル空間から今回のワールド空間へ変換
        //前回から今回にかけて変換された内容が乗っているオブジェクトに反映される
        WorldTransform = DirectX::XMLoadFloat4x4(&transform);
        DirectX::XMVECTOR LocalPosition = DirectX::XMLoadFloat3(&localHit.position);
        DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(LocalPosition, WorldTransform);

        DirectX::XMVECTOR LocalNormal = DirectX::XMLoadFloat3(&localHit.normal);
        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(LocalNormal, WorldTransform);

        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
        DirectX::XMVECTOR Dist = DirectX::XMVector3Length(Vec);
        DirectX::XMStoreFloat3(&hit.position, WorldPosition);
        DirectX::XMStoreFloat3(&hit.normal, WorldNormal);
        DirectX::XMStoreFloat(&hit.distance, Dist);
        hit.materialIndex = localHit.materialIndex;

        //回転差分を算出
        hit.rotation.x = angle.x - oldAngle.x;
        hit.rotation.y = angle.y - oldAngle.y;
        hit.rotation.z = angle.z - oldAngle.z;
        return true;
    }
    return false;


}

void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}
