#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "MetaAI.h"

// State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Walk), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
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
void WanderState::Exit()
{
    //�����Ȃ��Ă悢
}


// �ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void IdleState::Enter()
{
    // �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
    // �^�C�}�[�������_���ݒ�
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Idle), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void IdleState::Execute(float elapsedTime)
{
    // �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��
    // �^�C�}�[����
    owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

    // �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J��
    if (owner->GetStateTimer() < 0.0f)
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Search::Wander));

    // �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J��
    if (owner->SearchPlayer())
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyThief::State::Battle));
}

// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void IdleState::Exit()
{
    //�����Ȃ��Ă悢
}


// �ǐՃX�e�[�g�ɓ��������̃��\�b�h
void PursuitState::Enter()
{
    if (owner->GetHealth() <= 0)return;
    // �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

    // TODO 05_06 �G�𔭌�������Œ��Ԃ��Ă�
    // �G�l�~�[���烁�^AI�� MsgCallHelp �𑗐M����B
    Meta::Instance().SendMessaging(
        owner->GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgCallHelp);

}

// �ǐՃX�e�[�g�Ŏ��s���郁�\�b�h
void PursuitState::Execute(float elapsedTime)
{

    if (!owner->IsThrown())
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
    if (owner->GetAttackRange() > dist)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Standby));
    }
}

// �ǐՃX�e�[�g����o�Ă����Ƃ��̃��\�b�h
void PursuitState::Exit()
{
    Player::Instance().SetgetAttackSoon(false);
}

// �U���X�e�[�g�ɓ��������̃��\�b�h
void AttackState::Enter()
{
    randomType = static_cast<AttackType>(Mathf::RandomRange(1, 2));

    if (randomType == AttackType::Punch)
    {
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);

    }

    if (randomType == AttackType::Shot)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Shot));
    }
}

// �U���X�e�[�g�Ŏ��s���郁�\�b�h
void AttackState::Execute(float elapsedTime)
{
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
void AttackState::Exit()
{

}


// �p���`�X�e�[�g�ɓ��������̃��\�b�h
void PunchState::Enter()
{
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
        Player::Instance().ApplyDamage(1, 2.0f);
        //MessageData::CAMERASHAKEDATA	p = { 0.1f, 2.0f };
        //Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
    }
}

// �p���`�X�e�[�g�Ŏ��s���郁�\�b�h
void PunchState::Execute(float elapsedTime)
{
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
void PunchState::Exit()
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
        Player::Instance().SetgetAttackSoon(false);
    }
}



// �V���[�g�X�e�[�g�ɓ��������̃��\�b�h
void ShotState::Enter()
{
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
        owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::AttackShot), false);
        Player::Instance().ApplyDamage(1, 2.0f);
        /*CameraController::Instance().ShakeCamera(0.1f, 0.3f);*/
    }
}

// �V���[�g�X�e�[�g�Ŏ��s���郁�\�b�h
void ShotState::Execute(float elapsedTime)
{
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
void ShotState::Exit()
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
        Player::Instance().SetgetAttackSoon(false);
    }
}


// �f�X�g���N�^
SearchState::~SearchState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// �T�[�`�X�e�[�g�ɓ��������̃��\�b�h
void SearchState::Enter()
{
    SetSubState(static_cast<int>(EnemyThief::Search::Idle));
}

// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void SearchState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void SearchState::Exit()
{

}

BattleState::~BattleState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

void BattleState::Enter()
{
    SetSubState(static_cast<int>(EnemyThief::Search::Wander));
}

void BattleState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}

void BattleState::Exit()
{
}

// TODO 05_03 ���^AI���烁�b�Z�[�W����M�����Ƃ��ɌĂ΂��X�e�[�g��ǉ�
// �f�X�g���N�^
RecieveState::~RecieveState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}

// �f�[�^��M�����Ƃ��̃X�e�[�g
void RecieveState::Enter()
{
    // �����X�e�[�g��ݒ�
    SetSubState(static_cast<int>(EnemyThief::Recieve::Called));
}

// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void RecieveState::Execute(float elapsedTime)
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
void RecieveState::Exit()
{
}

// TODO 05_03 ���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
void CalledState::Enter()
{
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Run), true);
    owner->SetStateTimer(5.0f);
}

// �R�[���h�X�e�[�g�Ŏ��s���郁�\�b�h
void CalledState::Execute(float elapsedTime)
{
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
void CalledState::Exit()
{
}

// �퓬�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void StandbyState::Enter()
{
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

    Player::Instance().SetgetAttackSoon(false);
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::HoldGun), false);
    attackCooldownTimer = Mathf::RandomRange(2.5f, 4.0f);

}

// �퓬�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void StandbyState::Execute(float elapsedTime)
{
    //AttackType randomType = static_cast<AttackType>(Mathf::RandomRange(1, 2));
    //randomType = static_cast<AttackType>(2);

    if (attackCooldownTimer <= attackWarningTime && !Player::Instance().GetAttackSoon()) {
        Player::Instance().SetgetAttackSoon(true);
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
        Player::Instance().SetgetAttackSoon(false);
        // �U���͈͂���o����ǐՃX�e�[�g�֑J��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyThief::Battle::Pursuit));
    }
}

// �퓬�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void StandbyState::Exit()
{
    Player::Instance().SetgetAttackSoon(false);
}

