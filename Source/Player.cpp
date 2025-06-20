#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "StageMain.h"
#include "StageManager.h"
#include "ProjectileStraight.h"
#include "SceneGame.h"
#include "ProjectileWall.h"
#include "CharacterManager.h"
#include "SwingWeb.h"
#include "MessageData.h"
#include "Messenger.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
#include "EnemyThief.h"
#include "PlayerStateDerived.h"

Player* Player::instance = nullptr;

// �R���X�g���N�^
Player::Player(bool flag)
{
    // �C���X�^���X�|�C���g�ݒ�
    instance = this;

    // ���f���ǂݍ���
    model = std::make_unique<Model>("Data/Model/Spider-man/spider-man.mdl");
    model->GetNodePoses(nodePoses);

    radius = 0.5f;

    // ���f�����傫���̂ŃX�P�[�����O
    if (!flag)
    {
        position = { 0, 0, 0 };
    }

    scale.x = scale.y = scale.z = 0.01f;

    // �X�e�[�g�}�V��
    stateMachine = new PlayerStateMachine();

    // �X�e�[�g�̓o�^
    stateMachine->RegisterState(new PlayerStates::IdleState(this));
    stateMachine->RegisterState(new PlayerStates::MoveState(this));
    stateMachine->RegisterState(new PlayerStates::JumpState(this));
    stateMachine->RegisterState(new PlayerStates::LandState(this));
    stateMachine->RegisterState(new PlayerStates::AttackState(this));
    stateMachine->RegisterState(new PlayerStates::DamageState(this));
    stateMachine->RegisterState(new PlayerStates::DeathState(this));
    stateMachine->RegisterState(new PlayerStates::DodgeState(this));
    stateMachine->RegisterState(new PlayerStates::ClimbState(this));
    stateMachine->RegisterState(new PlayerStates::SwingState(this));
    stateMachine->RegisterState(new PlayerStates::ShotState(this));
    stateMachine->RegisterState(new PlayerStates::ClimbTopState(this));
    stateMachine->RegisterState(new PlayerStates::GrabState(this));
    stateMachine->RegisterState(new PlayerStates::CrouchIdleState(this));
    stateMachine->RegisterState(new PlayerStates::TitleIdleState(this));
    stateMachine->RegisterState(new PlayerStates::SwingToKickState(this));
    stateMachine->RegisterState(new PlayerStates::UltimateState(this));
    stateMachine->RegisterState(new PlayerStates::BounceState(this));

    // ������Ԑݒ�
    stateMachine->SetState(static_cast<int>(State::CrouchIdle));

    // �G�t�F�N�g
    hitEffect = std::make_unique<Effect>("Data/Effect/hitEffect.efk");

    // �T�E���h�ǂݍ���
    // �U����
    punch = Audio::Instance().LoadAudioSource("Data/Audio/punch.wav");
    punch2 = Audio::Instance().LoadAudioSource("Data/Audio/punch2.wav");
    kick = Audio::Instance().LoadAudioSource("Data/Audio/kick.wav");
    // ���Ƃ���
    Fall = Audio::Instance().LoadAudioSource("Data/Audio/Fall.wav");
    // �X�p�C�_�[�Z���X
    spiderSense = Audio::Instance().LoadAudioSource("Data/Audio/spiderSense.wav");
    // �V���[�g��
    FirstSwing = Audio::Instance().LoadAudioSource("Data/Audio/FirstSwingWeb.wav");
    // �X�C���O��
    Swing = Audio::Instance().LoadAudioSource("Data/Audio/SwingWeb.wav");
    // �V���b�g��
    ShotWeb = Audio::Instance().LoadAudioSource("Data/Audio/ShotWeb.wav");
    // �񕜉�
    Healing = Audio::Instance().LoadAudioSource("Data/Audio/Healing.wav");
    // �_���[�W��
    Damage = Audio::Instance().LoadAudioSource("Data/Audio/Damage.wav");

    skillTime = 5.0f;
}


// �f�X�g���N�^
Player::~Player()
{
    if (instance == this)instance = nullptr;
}

// �X�V����
void Player::Update(float elapsedTime)
{
    // �X�e�[�g�}�V���̍X�V
    stateMachine->Update(elapsedTime);
    //  �J�����X�e�[�g�̍X�V����
    UpdateCameraState(elapsedTime);

    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);
    brokenprojectileManager.Update(elapsedTime);

    // ���x�����X�V
    UpdateVelocity(elapsedTime);

    // ���G���ԍX�V����
    UpdateInvincibleTimer(elapsedTime);

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();
    CollisionProjectileVsEnemies();

    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    // �A�j���[�V�����X�V
    model->UpdateAnimation(elapsedTime);

    // ���f���s��X�V
    model->UpdateTransform(transform);

    //�@�U���E���^�[�E�X�L���^�C���X�V
    if (attacking)  attackTimer++;
    if (skillTime < skillTimeMax) skillTime += elapsedTime * 0.01f;

    if (EnemyManager::Instance().GetEnemyCount() == 0) getAttacksoon = false;

    // spiderSense�̍Đ���Ԃ��Ǘ�
    if (getAttacksoon)
    {
        if (!spiderSensePlayed)
        {
            spiderSense->Play(false, 0.8f);
            spiderSensePlayed = true;
        }
    }
    else
    {
        spiderSensePlayed = false;
    }

}

// �ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����
    Move(moveVec.x, moveVec.y, moveVec.z, moveSpeed);
    if (!onClimb)
    {
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
    }
    else
    {
        moveVec.x = 0;
        moveVec.z = 0;
    }
    return moveVec.x != 0 || moveVec.y != 0 || moveVec.z != 0;
}


// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // ���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    const DirectX::XMFLOAT3& cameraUp = camera.GetUp();

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLenght =
        sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLenght > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraRightX /= cameraRightLenght;
        cameraRightZ /= cameraRightLenght;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLenght =
        sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLenght > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraFrontX /= cameraFrontLenght;
        cameraFrontZ /= cameraFrontLenght;
    }

    float cameraUpY = cameraUp.y;

    DirectX::XMFLOAT3 vec;
    {
        // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
        // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
        // �i�s�x�N�g�����v�Z����
        vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
        vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
        // Y��
        vec.y = ay * cameraUpY * 3.0f;
    }

    return vec;
}

// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        // �Փˏ���
        DirectX::XMFLOAT3 outPosition;

        if (Collision::IntersectCylinderVsCylinder(
            GetPosition(), GetRadius(), GetHeight(),
            enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
            outPosition))
        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            // �����o����̈ʒu�ݒ�
            enemy->SetPosition(outPosition);
        }
    }
}
// �m�[�h�ƓG�̏Փˏ���
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    // �m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);

    if (node != nullptr)
    {
        // �m�[�h�ʒu�擾
        DirectX::XMFLOAT3 nodePosition;
        nodePosition.x = node->worldTransform._41;
        nodePosition.y = node->worldTransform._42;
        nodePosition.z = node->worldTransform._43;

        // �w��̃m�[�h�Ƃ��ׂĂ̓G�𑍓�����ŏՓˏ���
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);

            // �Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                nodeRadius,
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                // �_���[�W��^����
                if (enemy->ApplyDamage(15, 0.5f))
                {
                    // ������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 8.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = nodePosition;
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->AddImpulse(impulse);
                    }
                    // �q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();
                        enemyPos.y += enemy->GetHeight() * 1.5f;
                        if (enemy->GetHealth() != 0)
                        {
                            switch (attackCount)
                            {
                            case 1:
                                punch->Play(false, 0.4f);
                                break;
                            case 2:
                                punch2->Play(false, 0.4f);
                                break;
                            case 3:
                                kick->Play(false, 0.4f);
                                MessageData::CAMERASHAKEDATA	p = { 0.1f, 1.0f };
                                Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
                                break;
                            }
                            hitEffect->Play(enemyPos);
                            if (skillTime < skillTimeMax)
                            {
                                skillTime += 0.1f;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Player::PlayAttackAnimation()
{
    // �A���̃A�j���[�V����
    float attackAnimationSpeed = 1.5f; // �A�j���[�V�������x�𑬂����邽�߂̔{��
    switch (attackCount)
    {
    case 1: model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Attack), false, 0.1f, attackAnimationSpeed); break;
    case 2: model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Attack2), false, 0.1f, attackAnimationSpeed); break;
    case 3: model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Kick), false, 0.1f, attackAnimationSpeed); break;
    default: break;
    }
}

bool Player::IsNearWallTop()
{
    //�@���̈ʒu���擾
    Model::Node* HeadPos = model->FindNode("mixamorig:Head");
    DirectX::XMFLOAT3 pos;
    pos.x = HeadPos->worldTransform._41;
    pos.y = HeadPos->worldTransform._42;
    pos.z = HeadPos->worldTransform._43;

    const float frontCheckDistance = 1.0f;
    const float verticalOffset = 0.5f;

    //�@�O���̃`�F�b�N�ʒu���v�Z
    DirectX::XMFLOAT3 front = GetFront();
    DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&GetFront());
    DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetUp());
    forwardVec = DirectX::XMVector3Normalize(forwardVec);
    upVec = DirectX::XMVector3Normalize(upVec);

    forwardVec = DirectX::XMVectorScale(forwardVec, 1.1f);
    upVec = DirectX::XMVectorScale(upVec, 0.3f);

    checkDirection = DirectX::XMVectorAdd(forwardVec, upVec);

    DirectX::XMVECTOR CheckPoint = DirectX::XMLoadFloat3(&pos);
    CheckPoint = DirectX::XMVectorAdd(CheckPoint, checkDirection);

    DirectX::XMStoreFloat3(&checkpos, CheckPoint);

    //���C�̊J�n�ʒu�ƏI�_�ʒu
    DirectX::XMFLOAT3 start = { pos };
    DirectX::XMFLOAT3 end = {
        checkpos.x,
        checkpos.y,
        checkpos.z
    };
    checkpos = end;

    //���C�L���X�g�ɂ��ǔ���
    HitResult hit;
    if (!StageManager::Instance().RayCast(start, end, hit))
    {
        onClimb = false;
        CanClimb = true;
        position = checkpos;
        return true;
    }
    return false;
}

//�@�J�����X�e�[�g�X�V����
void Player::UpdateCameraState(float elapsedTime)
{
    //  ���b�N�I���E�I�t����p
    static bool tabPressed = false;

    CharacterManager& manager = CharacterManager::Instance();
    //  �G�̐����擾
    int enemyCount = 0;
    for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
    {
        Character* character = manager.GetCharacter(ii);
        if (character != nullptr && character != this)
        {
            if (character->GetHealth() > 0)
            {
                enemyCount++;
                break;
            }
        }
    }

    //  �G�����Ȃ��ꍇ�����t���[�J����
    if (enemyCount == 0)
    {
        lockonState = LockonState::NotLocked;
        lockonEnemy = nullptr;

        MessageData::CAMERACHANGEFREEMODEDATA p = { position };
        Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
        return;
    }

    // �X�e�[�g�ɉ����ăJ�����̏�Ԃ��X�V
    switch (state)
    {
    case State::Idle:
    case State::Move:
    case State::Jump:
    case State::Land:
    case State::Attack:
    case State::Shot:
    case State::Damage:
    case State::Climb:
    case State::Swing:
    case State::Dodge:
    case State::SwingToKick:
    case State::Bounce:
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButton() & GamePad::BTN_KEYBOARD_TAB || gamePad.GetButtonDown() & GamePad::BTN_RIGHT_THUMB)
        {
            // Tab�L�[�������ƁA���b�N����
            if (!tabPressed)
            {
                tabPressed = true;

                //  ���b�N�̎�
                if (lockonState == LockonState::Locked)
                {
                    if (lockonEnemy)
                    {
                        lockonEnemy->SetLockedOn(false);
                    }
                    lockonState = LockonState::NotLocked;
                    lockonEnemy = nullptr;
                    Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &position);
                }
                else //����ԋ߂��G���r���ĒT��
                {
                    DirectX::XMVECTOR p, t, v;
                    float length1, length2;
                    CharacterManager& manager = CharacterManager::Instance();
                    for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
                    {
                        Character* character = manager.GetCharacter(ii);
                        if (character == this || character->GetHealth() <= 0 || character == nullptr)
                            continue;

                        //�@���b�N�I������Ȃ�������
                        if (lockonState != LockonState::NotLocked)
                        {
                            p = DirectX::XMLoadFloat3(&position);
                            t = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
                            v = DirectX::XMVectorSubtract(t, p);
                            DirectX::XMStoreFloat(&length2, DirectX::XMVector3LengthSq(v));

                            p = DirectX::XMLoadFloat3(&position);
                            t = DirectX::XMLoadFloat3(&character->GetPosition());
                            v = DirectX::XMVectorSubtract(t, p);
                            DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));

                            if (length1 < length2)
                            {
                                lockonEnemy->SetLockedOn(false);
                                lockonEnemy = character;
                                lockonEnemy->SetLockedOn(true);

                                DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                            }
                        }
                        else
                        {
                            //�@��ԋ߂��G��ݒ肷��
                            p = DirectX::XMLoadFloat3(&position);
                            t = DirectX::XMLoadFloat3(&character->GetPosition());
                            v = DirectX::XMVectorSubtract(t, p);
                            DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));

                            if (length1 <= MAX_LOCKON_DISTANCE * MAX_LOCKON_DISTANCE)
                            {
                                lockonEnemy = character;
                                lockonEnemy->SetLockedOn(true);
                                DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                                lockonState = LockonState::Locked;
                            }
                        }
                    }

                    // ���b�N�I���G������������J���������b�N�I�����[�h�ɕύX
                    if (lockonEnemy)
                    {
                        lockonState = LockonState::Locked;
                        lockonEnemy->SetLockedOn(true);
                        MessageData::CAMERACHANGELOCKONMODEDATA p = { position, lockonEnemy->GetPosition() };
                        Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
                        break;
                    }
                    else
                    {
                        MessageData::CAMERACHANGEFREEMODEDATA p = { position };
                        Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
                        break;
                    }
                }
            }
        }
        else
        {
            tabPressed = false;
        }

        //  �����b�N���Ă�G���܂����݂��邩�ǂ����i���S�j
        if (lockonState == LockonState::Locked && lockonEnemy != nullptr && lockonEnemy->GetHealth() < 1)
        {
            // ���S�����玟��ԋ߂��̓G��T��
            lockonEnemy->SetLockedOn(false);
            lockonEnemy = nullptr;
            float closestDistance = FLT_MAX;
            CharacterManager& manager = CharacterManager::Instance();
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);

            for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
            {
                Character* character = manager.GetCharacter(ii);
                if (character == this || character->GetHealth() == 0)
                    continue;

                DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&character->GetPosition());
                DirectX::XMVECTOR toEnemy = DirectX::XMVectorSubtract(enemyPos, playerPos);
                float distance;
                DirectX::XMStoreFloat(&distance, DirectX::XMVector3LengthSq(toEnemy));

                if (distance < closestDistance && distance <= MAX_LOCKON_DISTANCE * MAX_LOCKON_DISTANCE)
                {
                    closestDistance = distance;
                    lockonEnemy = character;
                    DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(toEnemy));
                }
            }

            // �T����Ȃ�V�����G�����b�N
            if (lockonEnemy)
            {
                lockonEnemy->SetLockedOn(true);
                MessageData::CAMERACHANGELOCKONMODEDATA p = { position, lockonEnemy->GetPosition() };
                Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
            }
            else
            {
                // �����G��S���|������FreeCamera�ɖ߂�
                lockonState = LockonState::NotLocked;
                MessageData::CAMERACHANGEFREEMODEDATA p = { position };
                Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
            }
        }
        else if (lockonState == LockonState::Locked && lockonEnemy != nullptr)
        {
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
            DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));
            DirectX::XMStoreFloat3(&lockDirection, direction);

            //  �G�������߂��Ȃ烍�b�N�ł��Ȃ��̏���
            float distanceSq;
            DirectX::XMStoreFloat(&distanceSq, DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(enemyPos, playerPos)));
            if (distanceSq > MAX_LOCKON_DISTANCE * MAX_LOCKON_DISTANCE)
            {
                lockonState = LockonState::NotLocked;
                lockonEnemy = nullptr;

                MessageData::CAMERACHANGEFREEMODEDATA p = { position };
                Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
                return;
            }

            MessageData::CAMERACHANGELOCKONMODEDATA p = { position, lockonEnemy->GetPosition() };
            Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
        }
        else
        {
            MessageData::CAMERACHANGEFREEMODEDATA p = { position };
            Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
        }
        break;
    }
    //  ���S�������̃J����
    case State::Death:
    {
        MessageData::CAMERACHANGEMOTIONMODEDATA p;
        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        p.data.push_back({ 0, { position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 90, { position.x + vx, position.y + 10, position.z + vz }, position });
        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
        break;
    }
    //�@�����Z���̃J����
    case State::Grab:
    {
        MessageData::CAMERACHANGEMOTIONMODEDATA p;
        float backDistance = 7.0f;
        float heightOffset = 2.0f;

        float vx = sinf(angle.y) * -backDistance;
        float vz = cosf(angle.y) * -backDistance;
        p.data.push_back({ 0, { position.x + vx, position.y + 3, position.z + vz }, position });

        vx = sin(angle.y + DirectX::XM_PIDIV2 * 0.5f) * -backDistance;
        vz = cos(angle.y + DirectX::XM_PIDIV2 * 0.5f) * -backDistance;
        p.data.push_back({ 100, { position.x + vx, position.y + 3, position.z + vz }, position });

        vx = sin(angle.y + DirectX::XM_PIDIV2) * -backDistance;
        vz = cos(angle.y + DirectX::XM_PIDIV2) * -backDistance;
        p.data.push_back({ 200, { position.x + vx, position.y + 3, position.z + vz }, position });

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

        break;
    }
    //  �͈͍U�����̃J����
    case State::Ultimate:
    {
        MessageData::CAMERACHANGEMOTIONMODEDATA p;
        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        p.data.push_back({ 0, { position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 15, { position.x + vx, position.y + 3, position.z + vz }, position });

        vx = sinf(angle.y + DirectX::XM_PIDIV2) * -10;
        vz = cosf(angle.y + DirectX::XM_PIDIV2) * -10;
        p.data.push_back({ 40, { position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 55, { position.x + vx, position.y + 3, position.z + vz }, position });

        vx = sinf(angle.y) * -6;
        vz = cosf(angle.y) * -6;
        p.data.push_back({ 80, { position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 95, { position.x + vx, position.y + 3, position.z + vz }, position });

        vx = sinf(angle.y + DirectX::XM_PIDIV2) * 10;
        vz = cosf(angle.y + DirectX::XM_PIDIV2) * 10;
        p.data.push_back({ 115, { position.x + vx, position.y + 5, position.z + vz }, position });
        p.data.push_back({ 120, { position.x + vx, position.y + 5, position.z + vz }, position });

        vx = sinf(angle.y) * 8;
        vz = cosf(angle.y) * 8;
        p.data.push_back({ 145, { position.x + vx, position.y + 2, position.z + vz }, position });

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
        break;
    }

    }
}

//  �e�ۂ̓��͏���
bool Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER || gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_C)
    {
        ShotWeb->Play(false, 0.8f);

        DirectX::XMFLOAT3 dir;
        if (lockonEnemy)    //�@�����J�������b�N���Ă�ꍇ�͓G�������Ĕ���
        {
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
            DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));
            DirectX::XMStoreFloat3(&lockDirection, direction);

            angle.y = atan2f(lockDirection.x, lockDirection.z);
            dir = lockDirection;
        }
        else  //�@���b�N���ĂȂ��Ȃ璼�i�e�۔���
        {
            dir.x = sinf(angle.y);
            dir.y = 0;
            dir.z = cosf(angle.y);
        }

        //�@�e���肩��o��悤�ɁA�E���Node��T��
        Model::Node* RightHandPos = model->FindNode("mixamorig:RightHand");
        DirectX::XMFLOAT3 pos;
        pos.x = RightHandPos->worldTransform._41;
        pos.y = RightHandPos->worldTransform._42;
        pos.z = RightHandPos->worldTransform._43;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);

        //  ���f���������_���[�ɓo�^
        SceneGame& sceneGame = SceneGame::Instance();
        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            sceneGame.RegisterRenderModel(projectile->GetModel());

        }
        return true;
    }
    return false;
}

//  �e�ۂƓG�Ƃ̏Փˏ���
void Player::CollisionProjectileVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                //�_���[�W��^����

                if (enemy->ApplyDamage(15, 1.0f))
                {
                    //������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();

                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;
                        enemy->AddImpulse(impulse);
                    }
                    //�G�t�F�N�g����
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 1.5f;
                        hitEffect->Play(e);
                    }
                    //�e�۔j��
                    projectile->Destroy();
                    SceneGame::Instance().UnregisterRenderModel(projectile->GetModel());
                }
            }
        }
    }

}

// �U�����͏���
bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT || gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        attacking = true;
        if (attackCount < attackLimit)
        {
            attackCount++;
            attacking = true;
            return true;
        }
        else
        {
            attackCount = 0;
            attackTimer = 0;
            attacking = false;
        }
    }
    return false;

}

//�@�����͏���
bool Player::InputDodge()
{
    // �{�^�����͂ŃW�����v
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_CTLR || gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        // GetMoveVec ����ړ��x�N�g�����擾
        DirectX::XMFLOAT3 moveVec = GetMoveVec();

        // �ړ��x�N�g�����[���̏ꍇ�A���Ɉړ�
        if (moveVec.x == 0 && moveVec.z == 0)
        {
            dodgeDirection = GetFront();
            dodgeDirection.x = -dodgeDirection.x * 50.0f;
            dodgeDirection.z = -dodgeDirection.z * 50.0f;
        }
        else
        {
            // y ���̐����𖳎����� x, z ���̐����݂̂��g�p
            moveVec.y = 0.0f;
            dodgeDirection = moveVec;
        }
        return true;
    }
    return false;

}

//�@�񕜓��͏���
bool Player::InputHealing()
{
    // �{�^�����͂ŃW�����v
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_ALT || gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER)
    {
        Healing->Play(false, 0.4f);
        skillTime -= 1.0f;
        health += 20;
        if (health > 100)
        {
            health = 100;
        }
        return true;
    }
    return false;
}

// �f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    //�Փ˔���p�̃f�o�b�O����`��
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawSphere(swingPoint, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawSphere(checkpos, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    projectileManager.DrawDebugPrimitive();

    //  SwingToKick�U���͈�
    {
        Model::Node* RightToeBase = model->FindNode("mixamorig:RightToeBase");
        debugRender->DrawSphere(DirectX::XMFLOAT3(
            RightToeBase->worldTransform._41,
            RightToeBase->worldTransform._42,
            RightToeBase->worldTransform._43),
            attackRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1));
    }

    //  �y�U���͈�
    if (attackCollisionFlag)
    {
        if (attackCount == 1)
        {
            Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
            debugRender->DrawSphere(DirectX::XMFLOAT3(
                leftHandBone->worldTransform._41,
                leftHandBone->worldTransform._42,
                leftHandBone->worldTransform._43),
                attackRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1));
        }
        if (attackCount == 2)
        {
            Model::Node* rightHandBone = model->FindNode("mixamorig:RightHand");
            debugRender->DrawSphere(DirectX::XMFLOAT3(
                rightHandBone->worldTransform._41,
                rightHandBone->worldTransform._42,
                rightHandBone->worldTransform._43),
                attackRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1));
        }
        if (attackCount == 3)
        {
            Model::Node* rightFootBone = model->FindNode("mixamorig:RightFoot");
            debugRender->DrawSphere(DirectX::XMFLOAT3(
                rightFootBone->worldTransform._41,
                rightFootBone->worldTransform._42,
                rightFootBone->worldTransform._43),
                attackRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1));
        }
    }
}

// �W�����v���͏���
bool Player::InputJump()
{
    // �{�^�����͂ŃW�����v
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_SPACE || gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //�W�����v���͂���
        Jump(jumpSpeed * 1.1f);
        return true;
    }
    return false;
}

//  ���n�̎���
void Player::OnLanding()
{
    //�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
    if (velocity.y < gravity * 5.0f && state != State::Death && state != State::Damage)
    {
        //  ���n�X�e�[�g�ւ̑J��
        stateMachine->ChangeState(static_cast<int>(State::Land));
    }
}

//�X�C���O�|�C���g��T��
bool Player::FindWallSwingPoint()
{
    //���ɑO���Ə���Ƀx�N�g�����쐬
    DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&GetFront());
    DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetUp());

    forwardVec = DirectX::XMVector3Normalize(forwardVec);
    upVec = DirectX::XMVector3Normalize(upVec);

    forwardVec = DirectX::XMVectorScale(forwardVec, 8.0f);
    upVec = DirectX::XMVectorScale(upVec, 12.0f);

    SwingwebDirection = DirectX::XMVectorAdd(forwardVec, upVec);
    DirectX::XMVECTOR SwingPoint = DirectX::XMLoadFloat3(&position);
    SwingPoint = DirectX::XMVectorAdd(SwingPoint, SwingwebDirection);
    DirectX::XMStoreFloat3(&swingPoint, SwingPoint);

    previousSwingPoint = swingPoint;

    return false;
}

void Player::SwingCollision(float elapsedTime)
{
    // �X�C���O���ǂɂԂ��������̏���
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // ���C�̎n�_�ƏI�_���v�Z
        DirectX::XMFLOAT3 start = position;
        DirectX::XMFLOAT3 end = {
            position.x + mx,
            position.y,
            position.z + mz
        };
        float distance = sqrtf(mx * mx + mz * mz);

        // ���C���������v�Z�i���x����̂��߁j
        int steps = static_cast<int>(distance / 0.5f);
        steps = max(steps, 5);
        HitResult hit;

        const int raySamples = 3;
        float angleStep = DirectX::XM_2PI / raySamples;

        // �ȈՃX�t�B�A�L���X�g
        for (int step = 0; step <= steps; ++step) {
            float t = step / static_cast<float>(steps);
            DirectX::XMVECTOR currentPoint = DirectX::XMVectorLerp(XMLoadFloat3(&start), XMLoadFloat3(&end), t);

            for (int i = 0; i < raySamples; ++i) {
                float angle = i * angleStep;
                float offsetX = radius * 0.5f * cosf(angle);
                float offsetZ = radius * 0.5f * sinf(angle);

                DirectX::XMFLOAT3 offsetPoint;
                DirectX::XMStoreFloat3(&offsetPoint, DirectX::XMVectorAdd(currentPoint, DirectX::XMVectorSet(offsetX, 0, offsetZ, 0)));

                // ���C���΂�
                if (StageManager::Instance().RayCast(position, offsetPoint, hit))
                {
                    // �@�����擾���Đ��K��
                    DirectX::XMVECTOR hitNormal = XMLoadFloat3(&hit.normal);
                    hitNormal = DirectX::XMVector3Normalize(hitNormal);

                    // �@����Y�������������قǁA�����Ȗʁi�ǁj�Ɣ��f
                    float normalY = DirectX::XMVectorGetY(hitNormal);
                    if (fabsf(normalY) < 0.5f)
                    {
                        //��C�o�������邽�߂ɁA�v���C���[�̌����𒲐�
                        DirectX::XMVECTOR flatNormalVec = hitNormal;
                        flatNormalVec = DirectX::XMVectorSetY(flatNormalVec, 0.0f);
                        flatNormalVec = DirectX::XMVector3Normalize(flatNormalVec);

                        // �p�x�v�Z�̂��߂�XMFLOAT3�֕ϊ�
                        DirectX::XMFLOAT3 flatNormal;
                        DirectX::XMStoreFloat3(&flatNormal, flatNormalVec);

                        // �����𔽓]���ĕǂɌ���
                        flatNormal.x *= -1.0f;
                        flatNormal.z *= -1.0f;
                        // �v���C���[�̌������X�V
                        DirectX::XMFLOAT3 playerAngle = GetAngle();
                        playerAngle.y = atan2f(flatNormal.x, flatNormal.z);
                        SetAngle(playerAngle);

                        // �Փˈʒu�𒲐����ăv���C���[�����������߂�
                        DirectX::XMVECTOR backOffset = DirectX::XMVectorScale(hitNormal, 0.05f);
                        DirectX::XMVECTOR hitPosition = XMLoadFloat3(&hit.position);
                        DirectX::XMVECTOR adjustedPosition = DirectX::XMVectorAdd(hitPosition, backOffset);
                        DirectX::XMStoreFloat3(&position, adjustedPosition);

                        // �N���C�~���O�X�e�[�g�֑J��
                        hitWall = true;
                        onClimb = true;
                        stateMachine->ChangeState(static_cast<int>(State::Idle));
                        velocity = { 0, 0, 0 };
                        return;
                    }
                }
            }
        }
        // �Փ˂��Ȃ���΂��̂܂܈ʒu�X�V
        position = end;
    }
}

//�U��q�^���̌v�Z
void Player::HandleSwingPhysics(float elapsedTime, float ropeLength, float gravityStrength, float dragCoefficient)
{    
    // �o�E���X�X�e�[�g�Ȃ�Ȃ�����Ȃ�
    if (!ropeAttached && state == State::Bounce)return;

    //�X�C���O�ʒu
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&swingPoint);
    DirectX::XMVECTOR Q = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR displacement = DirectX::XMVectorSubtract(Q, P);
    float currentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(displacement));

    //���̌���
    DirectX::XMVECTOR ropeDirection = DirectX::XMVector3Normalize(displacement);

    //���̏d��
    DirectX::XMVECTOR gravity = DirectX::XMVectorSet(0.0f, -gravityStrength, 0.0f, 0.0f);
    //���͍X�V
    DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&velocity);
    velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorScale(gravity, elapsedTime));
    //���̋�C�j�͌n��
    velocityVec = DirectX::XMVectorScale(velocityVec, (1.0f - dragCoefficient));

    //���̏C��
    if (currentLength > ropeLength)
    {
        float correctionFactor = (currentLength - ropeLength) * 0.5f;
        DirectX::XMVECTOR correction = DirectX::XMVectorScale(ropeDirection, -correctionFactor);

        DirectX::XMVECTOR targetPosition = DirectX::XMVectorAdd(Q, correction);

        float smoothFactor = 0.1f;
        Q = DirectX::XMVectorLerp(Q, targetPosition, smoothFactor);
    }

    //���x�̏C��
    DirectX::XMVECTOR tangentVelocity = DirectX::XMVectorSubtract(
        velocityVec,
        DirectX::XMVectorMultiply(DirectX::XMVector3Dot(velocityVec, ropeDirection), ropeDirection));

    //�ʒu�X�V
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(Q, DirectX::XMVectorScale(tangentVelocity, elapsedTime));
    DirectX::XMStoreFloat3(&position, newPosition);
    DirectX::XMStoreFloat3(&velocity, tangentVelocity);

    DirectX::XMFLOAT3 front = GetFront();
    DirectX::XMVECTOR frontVec = DirectX::XMLoadFloat3(&front);
    //�����ō��_�ɒ�������A�A���X�C���O���s��
    float dotProduct = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tangentVelocity, frontVec));
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (state == State::Swing)
    {
        //�@�X�C���O�ō��_�B����������ƃo�E���X����
        if (dotProduct <= 0)
        {
            ropeAttached = false;

            DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&GetFront());
            DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetUp());
            forwardVec = DirectX::XMVector3Normalize(forwardVec);
            upVec = DirectX::XMVector3Normalize(upVec);

            forwardVec = DirectX::XMVectorScale(forwardVec, 5.0f);
            upVec = DirectX::XMVectorScale(upVec, 12.0f);

            DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&velocity);
            velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorAdd(forwardVec, upVec));
            DirectX::XMStoreFloat3(&velocity, velocityVec);

            bounceTimer = 0.0f;
            stateMachine->ChangeState(static_cast<int>(State::Bounce));
            return;
        }

        //�L�����N�^�[�̌������X�V
        DirectX::XMVECTOR swingDir = DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(XMLoadFloat3(&swingPoint), XMLoadFloat3(&position)));
        DirectX::XMStoreFloat3(&front, swingDir);
    }
    else if (state == State::SwingToKick)
    {
        //�L�����N�^�[�̌������X�V
        if (lockonEnemy)
        {
            // ���b�N�I�����Ă���G�̈ʒu���擾
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));
            DirectX::XMStoreFloat3(&front, directionToEnemy);
            Turn(elapsedTime, front.x, front.z, turnSpeed);
        }
        else
        {
            DirectX::XMVECTOR swingDir = DirectX::XMVector3Normalize(
                DirectX::XMVectorSubtract(XMLoadFloat3(&swingPoint), XMLoadFloat3(&position)));
            DirectX::XMStoreFloat3(&front, swingDir);
        }
    }
}

//�@�����ZActive
bool Player::ActiveGrabWeb(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos)
{
    if (IsUseGrab)
    {
        LineRenderer* lineRender = Graphics::Instance().GetLineRenderer();
        lineRender->DrawLine({ startPos },
            DirectX::XMFLOAT3(endPos),
            DirectX::XMFLOAT4(1, 1, 1, 1));
        return true;
    }
    return false;
}

// �f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Position", &position.x);

            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);

            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            ImGui::InputFloat3("Scale", &scale.x);

            ImGui::InputFloat3("velocity", &velocity.x);
            float l = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
            ImGui::InputFloat("v", &l);

            ImGui::SliderInt("PlayerHP", &health, 0.0f, 10.f);
            ImGui::InputInt("attackCount", &attackCount);
            ImGui::InputFloat("attackTimer", &attackTimer);

        }
        if (ImGui::CollapsingHeader("Skill", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat("webTimer", &webTimer);

            if (ImGui::CollapsingHeader("Spider Sense", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("SpiderSense", &getAttacksoon);
            }

            if (ImGui::CollapsingHeader("Clime Wall", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (hitWall)
                {
                    ImGui::Text("Is Hit Wall");
                }
                else ImGui::Text("Not Hiting Wall");

                if (onClimb)
                {
                    ImGui::Text("On Climb");
                }
                else ImGui::Text("Not On Climb");
                if (CanClimb)
                {
                    ImGui::Text("Can Climb");
                }
                else ImGui::Text("Can't Climb");
            }
        }
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            std::string str = "";
            switch (lockonState)
            {
            case LockonState::NotLocked:
                str = "FreeCamera";
                break;
            case LockonState::Locked:
                str = "LockOnCamera";
                break;
            }
            ImGui::Text(u8"State�@%s", str.c_str());
        }
    }
    ImGui::End();
}

// �_���[�W���󂯂��Ƃ��ɌĂ΂��
void Player::OnDamaged()
{
    stateMachine->ChangeState(static_cast<int>(State::Damage));
}

// ���S�����Ƃ��ɌĂ΂��
void Player::OnDead()
{
    stateMachine->ChangeState(static_cast<int>(State::Death));
}