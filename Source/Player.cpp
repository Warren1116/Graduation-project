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

    hitEffect = std::make_unique<Effect>("Data/Effect/Hit.efk");

    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
}

// �f�X�g���N�^
Player::~Player()
{

}

// �X�V����
void Player::Update(float elapsedTime)
{
    //  �J�����X�e�[�g�̍X�V����
    UpdateCameraState(elapsedTime);

    // �X�e�[�g���̏���
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Climb:
        UpdateClimbWallState(elapsedTime);
        break;
    case State::Swing:
        UpdateSwingState(elapsedTime);
        break;
    case State::Shot:
        UpdateShotingState(elapsedTime);
        break;
    }

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

}

// �ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    //// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    //Camera& camera = Camera::Instance();
    //DirectX::XMFLOAT3 cameraRight = camera.GetRight();
    //DirectX::XMFLOAT3 cameraFront = camera.GetFront();

    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����

    Move(moveVec.x, moveVec.y, moveVec.z, moveSpeed);
    if (!onClimb)
    {
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
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
    //if (!onClimb)
    {
        // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
        // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
        // �i�s�x�N�g�����v�Z����
        vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
        vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
        // Y���ɂ͈ړ����Ȃ�
        //vec.y = 0;
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
                if (enemy->ApplyDamage(3, 0.5f))
                {
                    // ������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
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
                        enemyPos.y += enemy->GetHeight() * 0.5f;
                        if (enemy->GetHealth() != 0)
                        {
                            hitEffect->Play(enemyPos);
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
    switch (attackCount)
    {
    case 1:
        model->PlayAnimation(Anim_Attack, false, 0.1f);
        break;
    case 2:
        model->PlayAnimation(Anim_Attack2, false, 0.1f);
        break;
    case 3:
        model->PlayAnimation(Anim_Kick, false, 0.1f);
        break;
    default:
        break;
    }

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
        if (character != this && character->GetHealth() > 0)
        {
            enemyCount++;
            break;
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

    switch (state)
    {
    case State::Idle:
    case State::Move:
    case State::Jump:
    case State::Land:
    case State::Attack:
    case State::Damage:
    case State::Climb:
    case State::Swing:
    {
        if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_TAB)
        {
            // Tab�L�[�������ƁA���b�N����
            if (!tabPressed)
            {
                tabPressed = true;

                //  ���b�N�̎�
                if (lockonState == LockonState::Locked)
                {
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
                        if (character == this || character->GetHealth() == 0)
                            continue;

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
                                lockonEnemy = character;
                                DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                            }
                        }
                        else
                        {
                            p = DirectX::XMLoadFloat3(&position);
                            t = DirectX::XMLoadFloat3(&character->GetPosition());
                            v = DirectX::XMVectorSubtract(t, p);
                            DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));

                            lockonEnemy = character;
                            DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                            lockonState = LockonState::Locked;
                        }
                    }

                    if (lockonEnemy)
                    {
                        lockonState = LockonState::Locked;
                        MessageData::CAMERACHANGELOCKONMODEDATA p = { position, lockonEnemy->GetPosition() };
                        Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
                        break;
                    }
                    else
                    {

                        MessageData::CAMERACHANGEFREEMODEDATA	p = { position };
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
        if (lockonState == LockonState::Locked && lockonEnemy != nullptr && lockonEnemy->GetHealth() == 0)
        {
            // ���S�����玟��ԋ߂��̓G��T��
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

                if (distance < closestDistance)
                {
                    closestDistance = distance;
                    lockonEnemy = character;
                    DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(toEnemy));
                }
            }

            // �T����Ȃ�V�����G�����b�N
            if (lockonEnemy)
            {
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
        MessageData::CAMERACHANGEMOTIONMODEDATA p;
        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        p.data.push_back({ 0, { position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 90, { position.x + vx, position.y + 15, position.z + vz }, position });
        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
        break;
    }
}

//  ���˃X�e�[�g�ׂ̑J��
void Player::TransitionShotingState()
{
    state = State::Shot;
    model->PlayAnimation(Anim_Shoting, false);
}

//  ���˃X�e�[�g�̍X�V����
void Player::UpdateShotingState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//  �e�ۂ̓��͏���
bool Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
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

                if (enemy->ApplyDamage(3, 1.0f))
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
                        e.y += enemy->GetHeight() * 0.5f;
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
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        attacking = true;
        if (attackCount < attackLimit && attacking)
        {
            attackCount++;
            return true;
        }
        else
        {
            attackCount = 0;
            attackTimer = 0;
        }
    }
    return false;

}

// �ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;
    onSwing = false;

    //�@�N���C�~���O���Ȃ�ʂ̑ҋ@���[�V����
    if (onClimb)
    {
        model->PlayAnimation(Anim_HoldInWall, true);
    }
    //�@���ʂ��ҋ@���[�V����
    else
    {
        model->PlayAnimation(Anim_Idle, true);
        if (attacking)
        {
            if (attackCount >= attackLimit || attackTimer > 120)
            {
                attackTimer = 0;
                attackCount = 0;
                attacking = false;
            }
        }
    }
}

// �ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(float elapsedTime)
{
    if (attacking)
    {
        attackTimer++;
    }
    //�ړ����͏���
    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        TransitionJumpState();
    }


    if (!onClimb)
    {
        //�e�ۓ��͏���
        if (InputProjectile())
        {
            TransitionShotingState();
        }

        //�U�����͏���
        if (InputAttack())
        {
            TransitionAttackState();
        }

    }
}

// �ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
    if (!attacking)
    {
        attackCount = 0;
        attackTimer = 0;
        attacking = false;
    }
    state = State::Move;
    model->PlayAnimation(Anim_Running, true);
}

// �ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //  �W�����u���ړ���SHIFT�L�[�����ɉ�����
    if (gamePad.GetButton() & GamePad::BTN_SHIFT && InputMove(elapsedTime) && firstSwing)
    {
        lastState = State::Move;
        TransitionSwingState();
    }

    //  �ڂ̑O�ɕǂ����铯����Speac�L�[��������
    if (hitWall && onClimb)
    {
        //�N���C�~���O�X�e�[�g�ւ̑J��
        TransitionClimbWallState();
    }

    //  �ړ����ĂȂ��Ȃ�
    if (!InputMove(elapsedTime))
    {
        //�ҋ@�X�e�[�g�ւ̑J��
        TransitionIdleState();
    }

    //�@�����ڂ̑O�ɕǂ����Ȃ��A�W�����v���͏���
    if (!hitWall && InputJump())
    {
        //�W�����u�X�e�[�g�ւ̑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    if (InputProjectile())
    {
        TransitionShotingState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

//  �W�����u�X�e�[�g
void Player::TransitionJumpState()
{
    state = State::Jump;

    model->PlayAnimation(Anim_Jump, false);

}

//  �W�����u�X�e�[�g�̍X�V����
void Player::UpdateJumpState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //�@�W�����u��Space�L�[������(��)
    //if (gamePad.GetButtonDown() & GamePad::BTN_SPACE || gamePad.GetButtonDown() & GamePad::BTN_A)

    //  �W�����u���ړ���SHIFT�L�[�����ɉ�����
    if (gamePad.GetButton() & GamePad::BTN_SHIFT && InputMove(elapsedTime))
    {
        lastState = State::Jump;
        //  �X�C���O�X�e�[�g�ւ̑J��
        TransitionSwingState();
    }

    //  �����N���C�~���O���Ȃ�A�N���C�~���O��Ԃ��L�����Z������
    if (onClimb)
    {
        onClimb = false;
    }

    //�ړ����͏���
    InputMove(elapsedTime);

    if (!model->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ւ̑J��
        TransitionIdleState();
    }

    //  �e�ۂ̓��͏���
    if (InputProjectile())
    {
        //  ���˃X�e�[�g�ւ̑J��
        TransitionShotingState();
    }

}

//�@�N���C�~���O�X�e�[�g
void Player::TransitionClimbWallState()
{
    state = State::Climb;
    model->PlayAnimation(Anim_Climb, true);
}

//  �N���C�~���O�X�e�[�g�ւ̑J��
void Player::UpdateClimbWallState(float elapsedTime)
{
    //�@�N���C�~���O��Space�L�[�����Ό��̏�Ԃɖ߂�
    if (InputJump())
    {
        TransitionJumpState();
    }

    //  �ړ����ĂȂ��Ȃ�ҋ@�X�e�[�g�ւ̑J��
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }
    InputMove(elapsedTime);
}

// �U���X�e�[�g�֑J��
void Player::TransitionAttackState()
{
    state = State::Attack;

    // �U���A�j���[�V�����Đ�
    PlayAttackAnimation();
}

// �U���X�e�[�g�X�V����
void Player::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    if (InputProjectile())
    {
        TransitionShotingState();
    }


    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 1.0f;

    if (attackCollisionFlag)
    {
        //  �J�������b�N���U�������
        if (lockonState == LockonState::Locked && lockonEnemy != nullptr)
        {
            //  ������Ƃ����G�̕����ړ�����
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));

            float moveDistance = 0.04f;
            DirectX::XMVECTOR moveVector = DirectX::XMVectorScale(directionToEnemy, moveDistance);

            DirectX::XMStoreFloat3(&lockDirection, directionToEnemy);
            Turn(elapsedTime, lockDirection.x, lockDirection.z, turnSpeed);

            playerPos = DirectX::XMVectorAdd(playerPos, moveVector);
            DirectX::XMStoreFloat3(&position, playerPos);
        }

        //  �A���ɂ���āA�m�[�h�ƃG�l�~�[�̏Փˏ���
        switch (attackCount)
        {
        case 1:
            CollisionNodeVsEnemies("mixamorig:LeftHandMiddle1", attackRadius);
            break;
        case 2:
            CollisionNodeVsEnemies("mixamorig:RightHandMiddle2", attackRadius);
            break;
        case 3:
            CollisionNodeVsEnemies("mixamorig:RightFoot", attackRadius + 0.2f);
            break;

        }

    }

}

// �_���[�W�X�e�[�g�֑J��
void Player::TransitionDamageState()
{
    state = State::Damage;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

// �_���[�W�X�e�[�g�X�V����
void Player::UpdateDamageState(float elapsedTime)
{
    // �_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

// ���S�X�e�[�g�֑J��
void Player::TransitionDeathState()
{
    state = State::Death;

    // ���S�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

// ���S�X�e�[�g�X�V����
void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {

    }
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


    projectileManager.DrawDebugPrimitive();

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
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE || gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // �W�����v���͂���
        Jump(jumpSpeed);
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
        TransitionLandState();
    }
}

//�@���n�X�e�[�g
void Player::TransitionLandState()
{
    //  �X�C���O�����ɒ��n����ƁAvelocity�̉e���ŏ��Ɋ���̂�
    //���̏󋵂�h������ velocity��0�ɂ���
    velocity.x = 0;
    velocity.y = 0;
    velocity.z = 0;
    //  �ŏ��̔��肪�Ȃ��ƁA�����ɔ�ё�����̂�
    firstSwing = true;
    state = State::Land;
    model->PlayAnimation(Anim_Landing, false);

}

//  ���n�X�e�[�g�̍X�V����
void Player::UpdateLandState(float elapsedTime)
{
    onClimb = false;
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

// �X�C���O�X�e�[�g
void Player::TransitionSwingState()
{
    firstSwing = false;
    state = State::Swing;

    if (lastState == State::Move)
    {
        model->PlayAnimation(Anim_StartSwing, false);

        //  �X�C���O���̓\��t�����Position(����)
        //  �v���C���[�̑O����ɐݒ�
        DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&GetFront());
        DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetUp());
        forwardVec = DirectX::XMVector3Normalize(forwardVec);
        upVec = DirectX::XMVector3Normalize(upVec);

        forwardVec = DirectX::XMVectorScale(forwardVec, 8.0f);
        upVec = DirectX::XMVectorScale(upVec, 11.0f);

        swingwebDirection = DirectX::XMVectorAdd(forwardVec, upVec);

        DirectX::XMVECTOR SwingPoint = DirectX::XMLoadFloat3(&position);
        SwingPoint = DirectX::XMVectorAdd(SwingPoint, swingwebDirection);

        DirectX::XMStoreFloat3(&swingPoint, SwingPoint);
        onSwing = true;

        //  �X�C���O���f���i���݉��AGeometric�ɕύX�\��j
        SwingWeb* swingWeb = new SwingWeb(&projectileManager);
        SceneGame& sceneGame = SceneGame::Instance();
        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            //  �����_���[�ɓo�^
            sceneGame.RegisterRenderModel(swingWeb->GetModel());

        }
    }
    else
    {
        //  �A���X�C���O�̃��[�V����
        if (!onSwing)
        {
            model->PlayAnimation(Anim_Swinging2, false);
        }
        else //�@����X�C���O�̃��[�V����  
        {
            model->PlayAnimation(Anim_Swinging, false);
        }
        //  �X�C���O���̓\��t�����Position(����)
        //  �v���C���[�̑O����ɐݒ�
        DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&GetFront());
        DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetUp());
        forwardVec = DirectX::XMVector3Normalize(forwardVec);
        upVec = DirectX::XMVector3Normalize(upVec);

        forwardVec = DirectX::XMVectorScale(forwardVec, 10.0f);
        upVec = DirectX::XMVectorScale(upVec, 7.0f);

        swingwebDirection = DirectX::XMVectorAdd(forwardVec, upVec);

        DirectX::XMVECTOR SwingPoint = DirectX::XMLoadFloat3(&position);
        SwingPoint = DirectX::XMVectorAdd(SwingPoint, swingwebDirection);

        DirectX::XMStoreFloat3(&swingPoint, SwingPoint);
        onSwing = true;

        //  �X�C���O���f���i���݉��AGeometric�ɕύX�\��j
        SwingWeb* swingWeb = new SwingWeb(&projectileManager);
        SceneGame& sceneGame = SceneGame::Instance();
        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            //  �����_���[�ɓo�^
            sceneGame.RegisterRenderModel(swingWeb->GetModel());

        }
    }

}

//  �X�C���O�X�e�[�g�̍X�V����
void Player::UpdateSwingState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        model->PlayAnimation(Anim_Swinging2, true);
    }

    //�X�C���O�ʒu
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&swingPoint);
    DirectX::XMVECTOR Q = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR displacement = DirectX::XMVectorSubtract(Q, P);
    float currentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(displacement));

    //���̒���
    const float ropeLength = 10.0f;
    //���̌���
    DirectX::XMVECTOR ropeDirection = DirectX::XMVector3Normalize(displacement);
    //���̏d��
    DirectX::XMVECTOR gravity = DirectX::XMVectorSet(0.0f, -10.0f, 0.0f, 0.0f);

    //���͍X�V
    DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&velocity);
    velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorScale(gravity, elapsedTime));
    //���̋�C�j�͌n��
    const float dragCoefficient = 0.005f;
    velocityVec = DirectX::XMVectorScale(velocityVec, (1.0f - dragCoefficient));


    //���̏C��
    if (currentLength > ropeLength)
    {
        float correctionFactor = (currentLength - ropeLength) * 0.5f;
        DirectX::XMVECTOR correction = DirectX::XMVectorScale(ropeDirection, -correctionFactor);
        Q = DirectX::XMVectorAdd(Q, correction);
    }

    DirectX::XMVECTOR tangentVelocity = DirectX::XMVectorSubtract(velocityVec, DirectX::XMVectorMultiply(DirectX::XMVector3Dot(velocityVec, ropeDirection), ropeDirection));

    //�ʒu�X�V
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(Q, DirectX::XMVectorScale(tangentVelocity, elapsedTime));
    DirectX::XMStoreFloat3(&position, newPosition);
    DirectX::XMStoreFloat3(&velocity, tangentVelocity);

    GamePad& gamePad = Input::Instance().GetGamePad();


    if (gamePad.GetButtonUp() & GamePad::BTN_SHIFT)
    {
        
        //TransitionSwingState();
        TransitionIdleState();
        //velocity = { 0.0f, 0.0f, 0.0f }; 
    }
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
                ImGui::Text("Can Climb Up");
            }
            else ImGui::Text("Can't Climb Up");

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
    TransitionDamageState();
}

// ���S�����Ƃ��ɌĂ΂��
void Player::OnDead()
{
    TransitionDeathState();
}