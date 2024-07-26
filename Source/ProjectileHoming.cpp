#include "ProjectileHoming.h"
#include "EnemyManager.h"



ProjectileHoming::ProjectileHoming(ProjectileManager* manager) : Projectile(manager)
{
    model = new Model("Data/Model/Sword/Sword.mdl");
    scale.x = scale.y = scale.z = 3.0f;
}


ProjectileHoming::~ProjectileHoming()
{
    delete model;
}

void ProjectileHoming::Update(float elaspedTime)
{
    //寿命処理
    lifetimer -= elaspedTime;
    if (lifetimer <= 0.0f)
    {
        Destroy();
    }

    //移動
    {
        float moveSpeed = this->moveSpeed * elaspedTime;
        position.x += direction.x * moveSpeed;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed;

    }

    //回転
    {
        float turnSpeed = this->turnSpeed * elaspedTime;
        //ターゲットまでのベクトルを算出
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        //ゼロベクトルをでないなら回転処理
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        if (lengthSq > 0.00001f)
        {
            //ターゲットまでのベクトルを単位ベクトル化
            Vec = DirectX::XMVector3Normalize(Vec);
            //向いている方向ベクトルを算出
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            //前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction,Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            //2つの単位ベクトルの角度が小さいほど
            //1.0に近づくという性質を利用して回転速度を調整する
            float rot = 1.0 - dot;
            if (rot > turnSpeed) rot = turnSpeed;

            //回転角度があるなら回転処理をする
            if (fabsf(rot)>0.0001f)
            {
                //回転軸を算出
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction,Vec);
                Axis = DirectX::XMVector3Normalize(Axis);
                //回転軸と回転量から回転行列を算出
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis,rot);
                    //現在の行列を回転させる(1)
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform,Rotation);
                //回転後の前方方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
                    //現在の行列を回転させる(2)
                //Direction = DirectX::XMVector3Transform(Direction, Rotation);
                //Direction = DirectX::XMVector3Normalize(Direction);
                //DirectX::XMStoreFloat3(&direction, Direction);
            }

        }
    }

    

    UpdateTransform();
    model->UpdateTransform(transform);

}

void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target)
{
    this->direction = direction;
    this->position = position;
    this->target = target;

    UpdateTransform();
}
