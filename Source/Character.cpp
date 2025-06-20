#include <algorithm>
#include "Character.h"
#include "StageManager.h"
#include "StageMain.h"
#include "Input/Input.h"
#include "Camera.h"
#include <cmath>

// �s��X�V����
void Character::UpdateTransform()
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    // ��]�s����쐬

    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

    DirectX::XMMATRIX R = Y * X * Z;;
    // �O�p�`�f�[�^���i�[
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    // �R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    // �v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

// �_���[�W��^����
bool Character::ApplyDamage(int damage, float invincibleTime)
{
    // �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0) return false;

    if (invincibleTimer > 0.0f) return false;

    invincibleTimer = invincibleTime;

    // �_���[�W����
    health -= damage;

    // ���S�ʒm
    if (health <= 0)
    {
        isAlive = false;
        OnDead();
    }
    // �_���[�W�ʒm
    else
    {
        OnDamaged();
    }

    // ���N��Ԃ�ύX�����ꍇ��true��Ԃ�
    return true;
}

// �Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    // ���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

// ���G���ԍX�V
void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}


// �������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //�d�͏���
    if (!onClimb)
    {
        velocity.y += gravity * elapsedFrame;
    }
    else
    {
        velocity.y = 0;
    }
}

// �����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
    //���������̈ړ���
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //������
    if (my < 0.0f)
    {
        if (isGround)
        {
            fallStartHeight = position.y;
        }

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
            position = hit.position;

            //��]
            angle.y += hit.rotation.y;

            //���n����
            if (!isGround && (fallStartHeight - position.y) >= landingHeightThreshold)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;

            //�X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

        }
        else if (!onClimb)
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
    if (onClimb)
    {
        position.y += velocity.y * elapsedTime;
        isGround = false;
        return;
    }

    //�N���C�~���O���ĂȂ��ꍇ
    if (!onClimb)
    {
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

}

// �������͍X�V�����@
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{

    // XZ���ʂ̑��͂���������
    float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
    if (!onSwing && length > 0.0f)
    {
        // ���C��
        float friction = this->friction * elapsedFrame;

        // �󒆂ɂ���Ƃ����C�͂����炷
        if (!isGround) friction *= airControl;

        // ���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            // �P�ʃx�N�g����
            float vx = velocity.x / length;
            float vz = velocity.z / length;
            float vy = velocity.y / length;

            velocity.x -= friction * vx;
            velocity.z -= friction * vz;
            velocity.y -= friction * vy;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    // XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecY * moveVecY + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // ������
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ������͂����炷
            if (!isGround) acceleration *= airControl;

            // �ړ��x�N�g���ɂ���������
            if (!onClimb)
            {
                velocity.x += acceleration * moveVecX;
                velocity.z += acceleration * moveVecZ;
            }
            else
            {
                velocity.y += acceleration * moveVecY * 1.5f;
            }

            // �ő呬�x����
            length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vy = velocity.y / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.y = vy * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            // �����ŃK�^�K�^���Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }

        }
    }
    // �ړ��x�N�g�����Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

// �����ړ��X�V�����@RayCast�p
void Character::UpdateHorizontalMove(float elapsedTime)
{
    //�������͗ʌv�Z
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {

        //�����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //���C�̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset ,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        DirectX::XMVECTOR velocityDirection = DirectX::XMVector3Normalize(DirectX::XMVectorSet(velocity.x, 0.0f, velocity.z, 0.0f));
        DirectX::XMVECTOR offset = DirectX::XMVectorScale(velocityDirection, 0.71f);
        DirectX::XMVECTOR endWithOffset = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&end), offset);
        DirectX::XMStoreFloat3(&end, endWithOffset);

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
                if (gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_SPACE || gamePad.GetButtonDown() & GamePad::BTN_A)
                {
                    //  �N���C�~���O�ɓ���܂�
                    onClimb = true;
                    //  ���f�����ǂɂԂ����񂽂̂ŁA�ʒu�̏C��
                    DirectX::XMVECTOR collisionNormal = DirectX::XMLoadFloat3(&hit.normal);
                    collisionNormal = DirectX::XMVector3Normalize(collisionNormal);
                    DirectX::XMVECTOR offset = DirectX::XMVectorScale(collisionNormal, 0.3f);

                    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&collectPosition);
                    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosition, offset);
                    DirectX::XMFLOAT3 newpos;
                    DirectX::XMStoreFloat3(&newpos, newPosition);

                    position.x = newpos.x;
                    position.z = newpos.z;

                }
            }
            else //�ǂɂԂ���Ȃ�����
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }

        }
        else //�ǂɂԂ���Ȃ�����
        {
            hitWall = false;
            onClimb = false;
            position.x += mx;
            position.z += mz;
        }
    }
}

void Character::CheckWallCollision(float elapsedTime)
{
    // ���C�̊J�n�ʒu�͑�����菭����
    DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };

    // ������Ƌ��������
    float checkDistance = 0.3f;

    // �e���ʂ̕����x�N�g��
    DirectX::XMFLOAT3 directions[4] = {
        {  1.0f, 0.0f,  0.0f },
        { -1.0f, 0.0f,  0.0f },
        {  0.0f, 0.0f,  1.0f },
        {  0.0f, 0.0f, -1.0f }
    };

    // �e�����Ƀ��C�L���X�g���s���A�Փ˔�����s��
    for (int i = 0; i < 4; ++i)
    {
        DirectX::XMFLOAT3 dir = directions[i];
        DirectX::XMFLOAT3 end = {
            start.x + dir.x * checkDistance,
            start.y,
            start.z + dir.z * checkDistance
        };

        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&hit.normal);
            normal = DirectX::XMVector3Normalize(normal);
            DirectX::XMVECTOR offset = DirectX::XMVectorScale(normal, 0.3f);

            DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(pos, offset);
            DirectX::XMStoreFloat3(&position, newPos);

            break;
        }
    }

}


//�ړ�����
void Character::Move(float vx, float vy, float vz, float speed)
{
    // �ړ������x�N�g����ݒ�
    moveVecX = vx;
    moveVecY = vy;
    moveVecZ = vz;
    maxMoveSpeed = speed;

}
void Character::Move(float vx, float vz, float speed)
{
    // �ړ������x�N�g����ݒ�
    moveVecX = vx;
    moveVecZ = vz;

    // �ő呬�x�ݒ�
    maxMoveSpeed = speed;
}

// ���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed += elapsedTime;

    if (vx == 0.0f && vz == 0.0f) return;

    float vl = sqrtf(vx * vx + vz * vz);
    vx /= vl;
    vz /= vl;

    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    float frontL = sqrtf(frontX * frontX + frontZ * frontZ);
    frontX /= frontL;
    frontZ /= frontL;

    float dot = (frontX * vx) + (frontZ * vz);
    float rot = 1.0f - dot;
    if (rot > speed) rot = speed;

    float cross = (frontZ * vx) - (frontX * vz);

    if (cross <= 0.0f)
    {
        angle.y -= rot * 0.5f;
    }
    else
    {
        angle.y += rot * 0.5f;
    }
}


// �W�����v����
void Character::Jump(float speed)
{
    // ������̗͂�ݒ�
    velocity.y = speed;
}

// ���x�����X�V
void Character::UpdateVelocity(float elapsedTime)
{
    // �o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    // �������͍X�V�����@
    UpdateHorizontalVelocity(elapsedFrame);

    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    // �����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);

    // �Փ˔���
    CheckWallCollision(elapsedTime);
}