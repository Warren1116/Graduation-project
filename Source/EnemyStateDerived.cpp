#include "EnemyStateDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "MetaAI.h"
#include "SceneGame.h"



//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �eSearchState
//-----------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
EnemyStates::SearchState::~SearchState()
{
    for (EnemyState* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// �T�[�`�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::SearchState::Enter()
{
    SetSubState(static_cast<int>(EnemyThief::Search::Idle));
}

// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::SearchState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::SearchState::Exit()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qBattleState�iSearchState�j
//----------------------------------------------------------------------------------------------------------------------------------------
// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::WanderState::Enter()
{
    if (owner->GetHealth() <= 0)
        return;

    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::WanderState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)
        return;

    // �ړI�n�_�܂ł�XZ���ʂł̋�������
    DirectX::XMFLOAT3 position = owner->GetPosition();
    DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;

    // �ړI�n�֒�����
    float radius = owner->GetRadius();
    if (distSq < radius * radius)
    {
        // �ҋ@�X�e�[�g�֑J��
        // ChangeState�N���X�œ�w�ڂ�State��؂�ւ���
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Search::Idle));
    }
    // �ړI�n�_�ֈړ�
    owner->MoveToTarget(elapsedTime, 0.5f);
    // �v���C���[���G
    if (owner->SearchPlayer())
    {
        // ����������ǐՃX�e�[�g�֑J��
        // ChangeState�N���X�ň�w�ڂ�State��؂�ւ���
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }


}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::WanderState::Exit()
{
    //�����Ȃ��Ă悢
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�qidleState�iSearchState�j
//-----------------------------------------------------------------------------------------------------------------------------------------
// �ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::IdleState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    // �^�C�}�[�������_���ݒ�
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Idle), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::IdleState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    // �^�C�}�[����
    owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

    // �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J��
    if (owner->GetStateTimer() < 0.0f)
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Search::Wander));

    // �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J��
    if (owner->SearchPlayer())
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }
}

// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::IdleState::Exit()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �eBattleState
//-----------------------------------------------------------------------------------------------------------------------------------------
EnemyStates::BattleState::~BattleState()
{
    for (EnemyState* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

void EnemyStates::BattleState::Enter()
{
    if (owner->GetHealth() <= 0)
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Dead));
        return;
    }
    SetSubState(static_cast<int>(EnemyThief::Battle::Pursuit));
}

void EnemyStates::BattleState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

void EnemyStates::BattleState::Exit()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qPursuitState�iBattleState�j
//-----------------------------------------------------------------------------------------------------------------------------------------
// �ǐՃX�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::PursuitState::Enter()
{
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

    // �G�𔭌�������Œ��Ԃ��Ă�
    // �G�l�~�[���烁�^AI�� MsgCallHelp �𑗐M����B
    Meta::Instance().SendMessaging(
        owner->GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgCallHelp);

}

// �ǐՃX�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::PursuitState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Dead));
        return;
    }
    if (!owner->IsGetThrow())
    {
        // �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��
        // �ڕW�n�_���v���C���[�ʒu�ɐݒ�
        owner->SetTargetPosition(Player::Instance().GetPosition());

    }

    // �ړI�n�_�ֈړ�
    owner->MoveToTarget(elapsedTime, 1.0);

    // �^�C�}�[����
    owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

    // �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J��
    if (owner->GetStateTimer() < 0.0f)
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Search));

    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);


    //// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J��
    if (owner->GetAttackRange() * 0.5f > dist)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
    }
}

// �ǐՃX�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::PursuitState::Exit()
{
    Player::Instance().SetgetAttackSoon(false);
    Player::Instance().SetgetShotSoon(false);
}
//----------------------------------------------------------------------------------------------------------------------------------------- 
//                                                          �qAttackState�iBattleState�j
// -----------------------------------------------------------------------------------------------------------------------------------------
// �U���X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::AttackState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    // �U���^�C�v�������_���Ō���
    // �p���`�U��
    if (owner->randomType == EnemyThief::AttackType::Punch)
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);

    }
    // �V���[�g�U��
    if (owner->randomType == EnemyThief::AttackType::Shot)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Shot));
    }

}

// �U���X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::AttackState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    owner->SetTargetPosition(Player::Instance().GetPosition());

    // �ړI�n�_�ֈړ�
    owner->MoveToTarget(elapsedTime, 1.0);

    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    //// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J��
    if (owner->GetPunchRange() > dist)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Punch));
        return;
    }


}

// �U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::AttackState::Exit()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qPunchState�iBattleState�j
//-----------------------------------------------------------------------------------------------------------------------------------------
// �p���`�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::PunchState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    // �U�������Ȃ����
    if (!owner->GetAttackFlg())
    {
        // �U���������^AI���狁�߂�
        // �G�l�~�[���烁�^AI�� MsgAskAttackRight �𑗐M����
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgAskAttackRight);
    }
    // �U����������΃��[�V�����Đ��J�n
    if (owner->GetAttackFlg())
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackPunch), false);
        Player::Instance().SetgetAttackSoon(false);
        Player::Instance().ApplyDamage(12, 2.0f);
    }
}

// �p���`�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::PunchState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    owner->SetTargetPosition(Player::Instance().GetPosition());

    // �ړI�n�_�ֈړ�
    owner->MoveToTarget(elapsedTime, 1.0);

    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    // �U����������Ƃ�
    if (owner->GetAttackFlg())
    {
        if (!owner->GetModel()->IsPlayAnimation())
        {
            // �U�����[�V�������I����Ă���ΒǐՂ֑J��
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
            return;
        }
    }
    else
    {
        // �U�������Ȃ��Ƃ��X�e�[�g�ύX
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
        return;
    }

}
// �p���`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::PunchState::Exit()
{
    if (owner->GetAttackFlg())
    {
        // �U�����I������Ƃ��U�����̕���
        // �U���t���O��false�ɐݒ�
        // �G�l�~�[���烁�^AI�� MsgChangeAttackRight �𑗐M����
        owner->SetAttackFlg(false);
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgChangeAttackRight);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qShotState�iBattleState�j
//-----------------------------------------------------------------------------------------------------------------------------------------
// �V���[�g�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::ShotState::Enter()
{
    Fire = Audio::Instance().LoadAudioSource("Data/Audio/Fire.wav");

    if (owner->GetHealth() <= 0)return;

    // �U�������Ȃ����
    if (!owner->GetAttackFlg())
    {
        // �U���������^AI���狁�߂�
        // �G�l�~�[���烁�^AI�� MsgAskAttackRight �𑗐M����
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgAskAttackRight);
    }
    // �U����������΃��[�V�����Đ��J�n
    if (owner->GetAttackFlg())
    {

        // �v���C���[�Ɍ����Ēe�𔭎�
        DirectX::XMFLOAT3 playerPosition = Player::Instance().GetPosition();
        DirectX::XMFLOAT3 ownerPosition = owner->GetPosition();
        DirectX::XMVECTOR toPlayer = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPosition), DirectX::XMLoadFloat3(&ownerPosition));
        toPlayer = DirectX::XMVector3Normalize(toPlayer);

        // �V�����������v�Z
        float angleY = atan2f(DirectX::XMVectorGetX(toPlayer), DirectX::XMVectorGetZ(toPlayer));
        owner->SetAngle(DirectX::XMFLOAT3(0, angleY, 0));

        Fire->Play(false);
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackShot), false);
        Player::Instance().SetgetAttackSoon(false);
        Player::Instance().ApplyDamage(8, 2.0f);
    }
}

// �V���[�g�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::ShotState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    // �U����������Ƃ�
    if (owner->GetAttackFlg())
    {
        // �U�����[�V�������I����Ă���ΒǐՂ֑J��
        if (!owner->GetModel()->IsPlayAnimation())
        {
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
        }
    }
    else
    {
        // �U�������Ȃ��Ƃ��X�e�[�g�ύX
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
    }
}

// �V���[�g�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::ShotState::Exit()
{
    if (owner->GetAttackFlg())
    {
        // �U�����I������Ƃ��U�����̕���
        // �U���t���O��false�ɐݒ�
        // �G�l�~�[���烁�^AI�� MsgChangeAttackRight �𑗐M����
        owner->SetAttackFlg(false);
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgChangeAttackRight);
        Player::Instance().SetgetShotSoon(false);

    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qStandbyState�iBattleState�j
// -----------------------------------------------------------------------------------------------------------------------------------------
// �퓬�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::StandbyState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    // �U�������Ȃ����
    if (!owner->GetAttackFlg())
    {
        // �U���������^AI���狁�߂�
        // �G�l�~�[���烁�^AI�� MsgAskAttackRight �𑗐M����
        Meta::Instance().SendMessaging(
            owner->GetId(),
            static_cast<int>(Meta::Identity::Meta),
            MESSAGE_TYPE::MsgAskAttackRight);
    }

    //�@�U���������_���ɐݒ肷��
    owner->randomType = (static_cast<EnemyThief::AttackType>(Mathf::RandomRange(1, 2)));

    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::HoldGun), false);
    attackCooldownTimer = Mathf::RandomRange(2.5f, 4.0f);

}

// �퓬�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::StandbyState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)return;

    // �v���C���[�Ɍ�����
    DirectX::XMFLOAT3 playerPosition = Player::Instance().GetPosition();
    DirectX::XMFLOAT3 ownerPosition = owner->GetPosition();
    DirectX::XMVECTOR toPlayer = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPosition), DirectX::XMLoadFloat3(&ownerPosition));
    toPlayer = DirectX::XMVector3Normalize(toPlayer);
    // �V�����������v�Z
    float angleY = atan2f(DirectX::XMVectorGetX(toPlayer), DirectX::XMVectorGetZ(toPlayer));
    owner->SetAngle(DirectX::XMFLOAT3(0, angleY, 0));

    // �U����������Ƃ�
    if (attackCooldownTimer <= attackWarningTime && !Player::Instance().GetAttackSoon())
    {
        Player::Instance().SetgetAttackSoon(true);
        if (owner->randomType == EnemyThief::AttackType::Shot)
        {
            Player::Instance().SetgetShotSoon(true);

        }

        Player::Instance().SetAttackEnemy(owner);
    }


    // �U����������Ƃ�
    if (owner->GetAttackFlg())
    {
        attackCooldownTimer -= elapsedTime;
        if (attackCooldownTimer <= 0)
        {
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Attack));
        }

    }

    // �ڕW�n�_���v���C���[�ʒu�ɐݒ�
    owner->SetTargetPosition(Player::Instance().GetPosition());
    float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
    float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
    float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist > owner->GetAttackRange())
    {
        // �U���͈͂���o����ǐՃX�e�[�g�֑J��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Pursuit));
    }
}

// �퓬�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::StandbyState::Exit()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qDamageState�iBattleState�j
//-----------------------------------------------------------------------------------------------------------------------------------------
// �_���[�W�X�e�[�g�ɓ��������̃��\�b�h
void EnemyStates::DamageState::Enter()
{
    //�@Player��SwingKick�ɍU������鎞
    if (Player::Instance().GetIsUseSwingKick() && owner->GetIsLockedOn())
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::KickDown), false);
    }
    else
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::GetHit), false);
    }
}


// �_���[�W�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::DamageState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation() && owner->GetHealth() > 0)
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }

}
// �_���[�W�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::DamageState::Exit()
{

}

//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �eRecieveState
//-----------------------------------------------------------------------------------------------------------------------------------------
// ���^AI���烁�b�Z�[�W����M�����Ƃ��ɌĂ΂��X�e�[�g��ǉ�
// �f�X�g���N�^
EnemyStates::RecieveState::~RecieveState()
{
    for (EnemyState* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// �f�[�^��M�����Ƃ��̃X�e�[�g
void EnemyStates::RecieveState::Enter()
{
    // �����X�e�[�g��ݒ�
    SetSubState(static_cast<int>(EnemyThief::Recieve::Called));
}

// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::RecieveState::Execute(float elapsedTime)
{
    // �q�X�e�[�g���s
    subState->Execute(elapsedTime);

    if (owner->SearchPlayer())
    {
        // Battle�X�e�[�g�֑J��
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }
}

// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::RecieveState::Exit()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//                                                          �qCalledState�iRecieveState�j
//-----------------------------------------------------------------------------------------------------------------------------------------
// ���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
void EnemyStates::CalledState::Enter()
{
    if (owner->GetHealth() <= 0)return;

    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(5.0f);
}

// �R�[���h�X�e�[�g�Ŏ��s���郁�\�b�h
void EnemyStates::CalledState::Execute(float elapsedTime)
{
    if (owner->GetHealth() <= 0)
        return;

    // �^�C�}�[����
    float timer = owner->GetStateTimer();
    timer -= elapsedTime;
    owner->SetStateTimer(timer);
    if (timer < 0.0f)
    {
        // �p�j�X�e�[�g�֑J��
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Search));
    }

    // �Ώۂ��v���C���[�n�_�ɐݒ�
    owner->SetTargetPosition(Player::Instance().GetPosition());
    owner->MoveToTarget(elapsedTime, 1.0f);

}

// �R�[���h�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void EnemyStates::CalledState::Exit()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------
