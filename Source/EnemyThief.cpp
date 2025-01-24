#include "EnemyThief.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "StateDerived.h"
#include "SceneGame.h"
#include "MessageData.h"
#include "Camera.h"
#include "CharacterManager.h"


#define USESTATEMACHINE3

// �R���X�g���N�^
EnemyThief::EnemyThief()
{
    model = std::make_unique<Model>("Data/Model/Enemy/Thief.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;
    height = 1.0f;

    // StateMachine�𐶐����A�K�w�^�X�e�[�g�}�V���ɑΉ�����悤�ɓo�^�X�e�[�g��ύX���Ă����B
    stateMachine = new StateMachine();

    stateMachine->RegisterState(new SearchState(this));
    stateMachine->RegisterState(new BattleState(this));

    // �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̂P�w�ڂ̃X�e�[�g��ǉ��o�^
    stateMachine->RegisterState(new RecieveState(this));

    // �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^(WanderState�ȊO��2�w�ڂ̃X�e�[�g�����l�̕��@�Ŋe���ǉ����Ă��������B)
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Search), new WanderState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Search), new IdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new PursuitState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new AttackState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new PunchState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new ShotState(this));

    // �X�e�[�g�}�V���Ƀ��b�Z�[�W����M�����Ƃ��̃T�u�X�e�[�g��ǉ��o�^
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Recieve), new CalledState(this));

    // �퓬���U�����̎����Ă��Ȃ���Ԃł̑ҋ@�X�e�[�g��ǉ��o�^
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new StandbyState(this));

    // �X�e�[�g���Z�b�g
    stateMachine->SetState(static_cast<int>(State::Search));
    ThrowFlag = false;



}

// �f�X�g���N�^
EnemyThief::~EnemyThief()
{
    delete stateMachine;
}

void EnemyThief::Update(float elapsedTime)
{

    switch (state)
    {
    case EnemyThief::State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    }

    if (Player::Instance().GetIsUseSwingKick() && IsLockedOn)
    {
        TransitionDamageState();
    }

    if (Player::Instance().GetIsUseGrab() && IsLockedOn)
    {
        ThrowFlag = true;
    }

    if (ThrowFlag)
    {
        webTimer += elapsedTime;

        if (webTimer > 3.0f)
        {
            IsLockedOn = false;
            DirectX::XMVECTOR pointA, pointB;

            DirectX::XMVECTOR DirectionVec;
            DirectX::XMVECTOR playerBack = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Player::Instance().GetFront()), -1.0f);
            DirectX::XMVECTOR playerUp = DirectX::XMLoadFloat3(&Player::Instance().GetUp());

            playerBack = DirectX::XMVector3Normalize(playerBack);
            playerUp = DirectX::XMVector3Normalize(playerUp);

            playerBack = DirectX::XMVectorScale(playerBack, 22.0f);
            playerUp = DirectX::XMVectorScale(playerUp, 22.0f);

            DirectionVec = DirectX::XMVectorAdd(playerBack, playerUp);

            pointA = DirectX::XMLoadFloat3(&position);
            pointB = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Player::Instance().GetPosition()), DirectionVec);

            DirectX::XMFLOAT3 directionVec;
            DirectX::XMStoreFloat3(&directionVec, DirectionVec);

            velocity = directionVec;

            float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectionVec));

            float speed = 10.0f;
            DirectX::XMVECTOR Velocity = DirectX::XMLoadFloat3(&velocity);

            float gravity = 1.0f;

            static float timeElapsed = 0.0f;
            timeElapsed += elapsedTime;

            DirectX::XMVECTOR newPosition = pointA;
            newPosition = DirectX::XMVectorAdd(newPosition, DirectX::XMVectorScale(Velocity, timeElapsed));
            newPosition = DirectX::XMVectorAdd(newPosition, DirectX::XMVectorSet(0.0f, -0.5f * gravity * timeElapsed * timeElapsed, 0.0f, 0.0f));

            position = DirectX::XMFLOAT3(DirectX::XMVectorGetX(newPosition), DirectX::XMVectorGetY(newPosition), DirectX::XMVectorGetZ(newPosition));
            model->PlayAnimation(static_cast<int>(EnemyAnimation::GetThrow), false);

            ThrowFlag = false;
        }

    }


    if (!Player::Instance().GetIsUseGrab() && health >= 0)
    {
        stateMachine->Update(elapsedTime);
    }

    // ���͏����X�V
    UpdateVelocity(elapsedTime);

    // ���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    // �I�u�W�F�N�g�s��X�V
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    // ���f���s��X�V
    model->UpdateTransform(transform);

    if (state == State::Dead && !Player::Instance().GetIsUseGrab())
    {
        UpdateDeathState(elapsedTime);
    }
}

void EnemyThief::TransitionDeathState()
{
    velocity.x = 0;
    velocity.y = 0;
    velocity.z = 0;
    state = State::Dead;
    if (Player::Instance().GetIsUseGrab())
    {
        model->PlayAnimation(static_cast<int>(EnemyAnimation::GetThrow), false);
    }
    else
    {
        model->PlayAnimation(static_cast<int>(EnemyAnimation::Die), false);
    }
    SetAttackFlg(false);

    Meta::Instance().SendMessaging(
        GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgChangeAttackRight);

}

void EnemyThief::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        isAlive = false;
        Destroy();
        SceneGame::Instance().UnregisterRenderModel(model.get());
    }
}

void EnemyThief::TransitionDamageState()
{
    state = State::Damage;
    float attackAnimationSpeed = 1.5f; // �A�j���[�V�������x�𑬂����邽�߂̔{��

    model->PlayAnimation(static_cast<int>(EnemyAnimation::KickDown), false, 0.2f, attackAnimationSpeed);
}

void EnemyThief::UpdateDamageState(float elapsedTime)
{
    if (!model->IsPlayAnimation() && health > 0)
    {
        stateMachine->ChangeState(static_cast<int>(EnemyThief::State::Battle));
    }
    else if (!model->IsPlayAnimation() && health < 1)
    {
        isAlive = false;
        Destroy();
        SceneGame::Instance().UnregisterRenderModel(model.get());
    }
}


// ���S�������ɌĂ΂��
void EnemyThief::OnDead()
{
    TransitionDeathState();
    Player::Instance().SetgetShotSoon(false);
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



bool EnemyThief::OnMessage(const Telegram& telegram)
{
    switch (telegram.msg)
    {
    case MESSAGE_TYPE::MsgCallHelp:
        if (!SearchPlayer())
        {	// �v���C���[�������Ă��Ȃ��Ƃ��Ɉ�w�ڃX�e�[�g��Receive�ɕύX����
            stateMachine->ChangeState(static_cast<int>(EnemyThief::State::Recieve));
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
    }


}
