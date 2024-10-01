#include "Character.h"
#include "StageManager.h"
#include "Mathf.h"
#include "Input/Input.h"

//�s��X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //�R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);

}

bool Character::ApplyDamage(int damage)
{
    if (damage == 0)    return false;
    if (health <= 0)    return false;

    health -= damage;

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }
    return true;
}

bool Character::ApplyDamage(int damage, float invincibleTime)
{
    if (damage == 0)    return false;
    if (health <= 0)    return false;

    if (invincibleTimer > 0) return false;

    health -= damage;

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        invincibleTimer = invincibleTime;
        OnDamaged();
        //UpdateInvincibleTimer(invincibleTime);

    }
    return true;

}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}


//�ړ�����
void Character::Move(float vx,float vy, float vz, float speed)
{
    moveVecX = vx;
    moveVecZ = vz;
    moveVecY = vy;
    maxMoveSpeed = speed;

}
void Character::Move(float vx, float vz, float speed)
{
    moveVecX = vx;
    moveVecZ = vz;
    maxMoveSpeed = speed;

}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    //if (vx == 0 && vz == 0) return;
    float length = sqrtf((vx * vx) + (vz * vz));
    if (length < 0.001f) return;
    //�i�s�x�N�g����P�ʃx�N�g����
    vx /= length;
    vz /= length;

    //���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (frontX * vx) + (frontZ * vz);

    //���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    //�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐߂���
    float rot = 1.0 - dot;
    if (rot > speed) rot = speed;

    //���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (frontZ * vx) - (frontX * vz);

    if (cross < 0.0f)
    {
        //angle.y -= speed;
        angle.y -= rot;
    }
    else
    {
        //angle.y += speed;
        angle.y += rot;
    }

}

void Character::Jump(float speed)
{
    velocity.y = speed;
}

void Character::UpdateVelocity(float elapsedTime)
{
    float elapsedFrame = 60.0f * elapsedTime;

    //�������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    //�������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    //�����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    //�����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);

}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //�d�͏���
    if (!onClimb)
    {
        velocity.y += gravity * elapsedFrame;
    }
    else 
    {
        velocity.y = climbSpeed;
    }
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    ////Y���ړ�����
    //position.y += velocity.y * elapsedTime;
    ////�n�ʔ���
    //if (position.y < 0.0f)
    //{
    //    position.y = 0.0f;
    //    velocity.y = 0.0f;
    //    if (!isGround)
    //    {
    //        OnLanding();
    //    }
    //    isGround = true;
    //}
    //else
    //{
    //    isGround = false;
    //}


    //���������̈ړ���
    float my = velocity.y * elapsedTime;


    slopeRate = 0.0f;

    //�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //������
    if (my < 0.0f)
    {
        //���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

        //���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            normal = hit.normal;

            //�n�ʂɐڒn���Ă���
            //position.y = hit.position.y;
            position = hit.position;

            //��]
            angle.y += hit.rotation.y;

            //���n����
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;

            //�X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

        }
        else if(!onClimb)
        {
            //�󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    //�㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    //�n�ʂ̌����ɉ����悤��XZ����]
    {
        //Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
        float ax = atan2f(normal.z, normal.y);
        float az = -atan2f(normal.x, normal.y);

        //���`�⊮�Ŋ��炩�ɉ�]����
        float time = 0.2f;
        angle.x = Mathf::Lerp(angle.x, ax, time);
        angle.z = Mathf::Lerp(angle.z, az, time);
    }
}

void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //XZ���ʂ̑��͂���������
    float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        //���C��
        float friction = this->friction * elapsedFrame;


        //�󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround) friction *= airControl;


        //���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            //�P�ʃx�N�g����
            float vx = velocity.x / length;
            float vz = velocity.z / length;
            float vy = velocity.y / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
            velocity.y -= vy * friction;
        }
        //�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }
    //XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecY * moveVecY + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            float acceleration = this->acceleration * elapsedFrame;

            //�󒆂ɂ���Ƃ��͉����͂����炷
            if (!isGround) acceleration *= airControl;

            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //�����ŃK�^�K�^�����Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsedTime)
{
    ////XZ���ړ�����
    //position.x += velocity.x * elapsedTime;
    //position.z += velocity.z * elapsedTime;

    //�������͗ʌv�Z
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //�����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;
        float my = velocity.y * climbSpeed * elapsedTime;

        //���C�̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset ,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        //���C�L���X�g�ɂ��ǔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //�ǂ���Ԃ����񂽂̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
            //���˃x�N�g����@���Ɏˉe
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 1.1f);


            //�␳�ʒu�̌v�Z
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            HitResult hit2;
            if (!StageManager::Instance().RayCast(start, collectPosition, hit2))
            {
                hitWall = true;
                GamePad& gamePad = Input::Instance().GetGamePad();
                if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
                {
                   onClimb = true;
                   position.x = collectPosition.x;
                   position.z = collectPosition.z;
                }

            }
            else
            {
                
                position.x = hit2.position.x;
                position.y = hit.position.y;
                position.z = hit2.position.z;
            }

        }
        else
        {
            onClimb = false;
            position.x += mx;
            position.z += mz;
            position.y += my;
        }
    }
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

