#include "EnemySlime.h"
#include "Graphics\Graphics.h"
#include "Player.h"
#include "Mathf.h"
#include "Collision.h"

EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");
    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;
    radius = 0.5f;
    height = 1.0f;

    TransitionWanderState();
}

EnemySlime::~EnemySlime()
{
    delete model;
}

void EnemySlime::Update(float elapsedTime)
{
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;    
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;

    }

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

bool EnemySlime::SearchPlayer()
{
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;

    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        //�P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;
        //�O���x�N�g��
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //�Q�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;

    //���b�ԒǐՂ���^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_RunFWD, true);
}

void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 1.0f);

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;

    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        TransitionAttackState();
    }

}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 0.35f)
    {
        //�ڋʃm�[�h�ƃv���C���[�̏Փˏ���
        CollisionNodeVsPlayer("EyeBall",0.2f);
    }
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //�����蔻��\��
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1));

        //�v���C���[�Ɠ����蔻��
        Player& player = Player::Instance();
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player.GetPosition(),
            player.GetRadius(),
            player.GetHealth(),
            outPosition))
        {
            //�_���[�W��^����
            if (player.ApplyDamage(1, 1.5f))
            {
                //�G�𐁂���΂��̓x�N�g�����Z�o
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;

                //XZ���ʂɐ�����΂��͂�������
                float power = 10.0f;
                vec.x *= power;
                vec.z *= power;

                //Y�����ɂ��͂�������
                vec.y = 5.0f;

                //������΂�
                player.AddImpulse(vec);

            }
        }

    }
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //���b�ԑҋ@����^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);
    //�퓬�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleBattle, true);

}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;

        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            TransitionAttackState();
        }
        else
        {
            TransitionWanderState();
        }

    }
    MoveToTarget(elapsedTime, 0.0f);
}

void EnemySlime::TransitionDamageState()
{
    state = State::Damage;
    model->PlayAnimation(Anim_GetHit, false);
}

void EnemySlime::UpdateDamageState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::TransitionDeathState()
{
    state = State::Death;
    model->PlayAnimation(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}

void EnemySlime::SetRandomTargetPosition()
{   
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f,territoryRange);

    targetPosition.x = territoryOrigin.x + sinf(theta) * range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;

    //targetPosition.x = Mathf::RandomRange(position.x - territoryRange,position.x+ territoryRange);
    //targetPosition.y = position.y;
    //targetPosition.z = Mathf::RandomRange(position.z - territoryRange, position.z + territoryRange);
}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    SetRandomTargetPosition();

    model->PlayAnimation(Anim_WalkFWD, true);
}

void EnemySlime::UpdateWanderState(float elapsedTime)
{
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //���̖ڕW�n�_�ݒ�
        //SetRandomTargetPosition();

        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();

    }
    MoveToTarget(elapsedTime, 0.5f);
    if (SearchPlayer())
    {
        TransitionPursuitState();
    }
}

void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    //�^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
{
    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        TransitionWanderState();
    }
    //�v���C���[���G
    if (SearchPlayer())
    {
        //����������ǐՃX�e�[�g�֑J��
        TransitionPursuitState();
    }
}

void EnemySlime::OnDead()
{
    //Destroy();
    TransitionDeathState();
}

void EnemySlime::OnDamaged()
{
    TransitionDamageState();
}
