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
    //��������
    lifetimer -= elaspedTime;
    if (lifetimer <= 0.0f)
    {
        Destroy();
    }

    //�ړ�
    {
        float moveSpeed = this->moveSpeed * elaspedTime;
        position.x += direction.x * moveSpeed;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed;

    }

    //��]
    {
        float turnSpeed = this->turnSpeed * elaspedTime;
        //�^�[�Q�b�g�܂ł̃x�N�g�����Z�o
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        //�[���x�N�g�����łȂ��Ȃ��]����
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        if (lengthSq > 0.00001f)
        {
            //�^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);
            //�����Ă�������x�N�g�����Z�o
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            //�O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction,Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            //2�̒P�ʃx�N�g���̊p�x���������ق�
            //1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
            float rot = 1.0 - dot;
            if (rot > turnSpeed) rot = turnSpeed;

            //��]�p�x������Ȃ��]����������
            if (fabsf(rot)>0.0001f)
            {
                //��]�����Z�o
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction,Vec);
                Axis = DirectX::XMVector3Normalize(Axis);
                //��]���Ɖ�]�ʂ����]�s����Z�o
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis,rot);
                    //���݂̍s�����]������(1)
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform,Rotation);
                //��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
                    //���݂̍s�����]������(2)
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
