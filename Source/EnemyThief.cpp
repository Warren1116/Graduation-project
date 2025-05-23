#include "EnemyThief.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "EnemyStateDerived.h"
#include "SceneGame.h"
#include "MessageData.h"
#include "Camera.h"
#include "CharacterManager.h"

// �R���X�g���N�^
EnemyThief::EnemyThief()
{
    // ���f���ǂݍ���
    model = std::make_unique<Model>("Data/Model/Enemy/Thief.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    // ���f���̒��S�ʒu��ݒ�
    radius = 0.5f;
    height = 1.0f;

    // StateMachine�𐶐����A�K�w�^�X�e�[�g�}�V���ɑΉ�����悤�ɓo�^�X�e�[�g��ύX���Ă����B
    stateMachine = new EnemyStateMachine();

    stateMachine->RegisterState(new EnemyStates::SearchState(this));
    stateMachine->RegisterState(new EnemyStates::BattleState(this));

    // �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̂P�w�ڂ̃X�e�[�g��ǉ��o�^
    stateMachine->RegisterState(new EnemyStates::RecieveState(this));

    // �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Search), new EnemyStates::WanderState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Search), new EnemyStates::IdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::PursuitState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::AttackState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::PunchState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::ShotState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::DamageState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::DeadState(this));

    // �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̃T�u�X�e�[�g��ǉ��o�^
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Recieve), new EnemyStates::CalledState(this));

    // �퓬���U�����̎����Ă��Ȃ���Ԃł̑ҋ@�X�e�[�g��ǉ��o�^
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::StandbyState(this));

    // �X�e�[�g���Z�b�g
    stateMachine->SetState(static_cast<int>(State::Search));

    GetThrowFlag = false;

}

// �f�X�g���N�^
EnemyThief::~EnemyThief()
{
    delete stateMachine;
}

void EnemyThief::Update(float elapsedTime)
{
    //�@Player��Grab�ɍU������鎞
    if (Player::Instance().GetIsUseGrab() && IsLockedOn)
    {
        GetThrowFlag = true;
    }

    // �����Z����鎞�̏���
    if (GetThrowFlag)
    {
        webTimer += elapsedTime;

        // 3�b��ɓ����Z�����
        if (webTimer > 3.0f)
        {
            IsLockedOn = false;
            DirectX::XMVECTOR pointA, pointB;

            //  �v���C���[�̕����ɓ����Z�����
            DirectX::XMVECTOR DirectionVec;
            DirectX::XMVECTOR playerBack = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Player::Instance().GetFront()), -1.0f);
            DirectX::XMVECTOR playerUp = DirectX::XMLoadFloat3(&Player::Instance().GetUp());
            playerBack = DirectX::XMVector3Normalize(playerBack);
            playerUp = DirectX::XMVector3Normalize(playerUp);

            playerBack = DirectX::XMVectorScale(playerBack, 22.0f);
            playerUp = DirectX::XMVectorScale(playerUp, 22.0f);
            DirectionVec = DirectX::XMVectorAdd(playerBack, playerUp);

            //  �����Z�����ʒu
            pointA = DirectX::XMLoadFloat3(&position);
            pointB = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Player::Instance().GetPosition()), DirectionVec);

            //  �����Z��������
            DirectX::XMFLOAT3 directionVec;
            DirectX::XMStoreFloat3(&directionVec, DirectionVec);

            velocity = directionVec;

            float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectionVec));

            //  �����Z����鑬�x
            float speed = 10.0f;
            DirectX::XMVECTOR Velocity = DirectX::XMLoadFloat3(&velocity);

            //  ���̏d��
            float gravity = 1.0f;

            static float timeElapsed = 0.0f;
            timeElapsed += elapsedTime;

            //  �����Z�����ʒu�̍X�V
            DirectX::XMVECTOR newPosition = pointA;
            newPosition = DirectX::XMVectorAdd(newPosition, DirectX::XMVectorScale(Velocity, timeElapsed));
            newPosition = DirectX::XMVectorAdd(newPosition, DirectX::XMVectorSet(0.0f, -0.5f * gravity * timeElapsed * timeElapsed, 0.0f, 0.0f));

            position = DirectX::XMFLOAT3(DirectX::XMVectorGetX(newPosition), DirectX::XMVectorGetY(newPosition), DirectX::XMVectorGetZ(newPosition));

            GetThrowFlag = false;
        }

    }

    //  HP��0�ȏ�Ɠ����Z����ĂȂ��Ȃ�X�e�[�W�}�V���X�V
    if (!Player::Instance().GetIsUseGrab())
    {
        stateMachine->Update(elapsedTime);
        model->ResumeAnimation();
    }
    else
    {
        model->PauseAnimation();
    }

    // ���͏����X�V
    UpdateVelocity(elapsedTime);

    // ���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    // �I�u�W�F�N�g�s��X�V
    UpdateTransform();

    // �A�j���[�V�����X�V
    model->UpdateAnimation(elapsedTime);

    // ���f���s��X�V
    model->UpdateTransform(transform);

}

// ���S�������ɌĂ΂��
void EnemyThief::OnDead()
{
    CharacterManager::Instance().Remove(this);
    Player::Instance().SetgetShotSoon(false);
    stateMachine->SetState(static_cast<int>(State::Battle));
    stateMachine->ChangeSubState(static_cast<int>(EnemyThief::Battle::Dead));
}

void EnemyThief::OnDamaged()
{
    stateMachine->ChangeSubState(static_cast<int>(EnemyThief::Battle::Damage));

}


void EnemyThief::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

    // �^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

    // ���G�͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}

// �꒣��ݒ�
void EnemyThief::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

// �^�[�Q�b�g�ʒu�������_���ݒ�
void EnemyThief::SetRandomTargetPosition()
{
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta) * range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// �ړI�n�_�ֈړ�
void EnemyThief::MoveToTarget(float elapsedTime, float speedRate)
{
    // �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz + vy * vy);
    vx /= dist;
    vz /= dist;
    vy /= dist;

    // �ړ�����
    Move(vx, vy, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}


// ���b�Z�[�W��M����
bool EnemyThief::OnMessage(const Telegram& telegram)
{
    switch (telegram.msg)
    {
    case MESSAGE_TYPE::MsgCallHelp:
        if (!SearchPlayer())
        {	// �v���C���[�������Ă��Ȃ��Ƃ��Ɉ�w�ڃX�e�[�g��Receive�ɕύX����
            stateMachine->SetState(static_cast<int>(EnemyThief::State::Recieve));
        }
        return true;
        //	���^AI����U������^����ꂽ�Ƃ�
    case MESSAGE_TYPE::MsgGiveAttackRight:
        // �U���t���O��true�ɐݒ�
        SetAttackFlg(true);
        return true;
    }
    return false;
}

// �v���C���[��T��
bool EnemyThief::SearchPlayer()
{
    // �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        // �P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;

        // �����x�N�g����
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        // 2�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}


// �f�o�b�O�G�l�~�[���\��
void EnemyThief::DrawDebugGUI()
{
    std::string str = "";
    std::string subStr = "";

    switch (static_cast<State>(stateMachine->GetStateIndex()))
    {
    case State::Search:
        str = "Search";
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Search::Wander))
        {
            subStr = "Wander";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Search::Idle))
        {
            subStr = "Idle";
        }
        break;
    case State::Battle:
        str = "Battle";
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Pursuit))
        {
            subStr = "Pursuit";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Attack))
        {
            subStr = "Attack";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Standby))
        {
            subStr = "Standby";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Shot))
        {
            subStr = "Shot";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Punch))
        {
            subStr = "Punch";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Damage))
        {
            subStr = "Damage";
        }
        if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyThief::Battle::Dead))
        {
            subStr = "Dead";
        }
        break;
    case State::Recieve:
        if (stateMachine->GetStateIndex() == static_cast<int>(EnemyThief::Recieve::Called))
        {
            subStr = "Called";
        }
        break;

    }

    //�g�����X�t�H�[��
    if (ImGui::CollapsingHeader("EnemyThief", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // �ʒu
        ImGui::InputFloat3("Position", &position.x);
        // ��]
        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        // �X�P�[��
        ImGui::InputFloat3("Scale", &scale.x);

        ImGui::Text(u8"State�@%s", str.c_str());
        ImGui::Text(u8"SubState �@%s", subStr.c_str());
        ImGui::Checkbox("lockon", &IsLockedOn);
        ImGui::InputFloat("webTimer", &webTimer);
        ImGui::InputInt("Hp", &health);
        ImGui::Checkbox("Alive", &isAlive);

    }


}
