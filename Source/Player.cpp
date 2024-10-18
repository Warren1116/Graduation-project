#include <imgui.h>
#include "Player.h"
//#include "Gun.h"
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


Player* Player::instance = nullptr;

// �R���X�g���N�^
Player::Player(bool flag)
{
    // �C���X�^���X�|�C���g�ݒ�
    instance = this;

    // ���f���ǂݍ���
    model = new Model("Data/Model/Spider-man/spider-man.mdl");
    model->GetNodePoses(nodePoses);

    radius = 0.5f;

    // ���f�����傫���̂ŃX�P�[�����O
    if (!flag)
    {
        position = { 0, 0, 0 };
    }

    scale.x = scale.y = scale.z = 0.01f;


    //outOfBullets = Audio::Instance().LoadAudioSource("Data/Audio/Tamagire.wav");
    //walk = Audio::Instance().LoadAudioSource("Data/Audio/walk.wav");
    //key = Audio::Instance().LoadAudioSource("Data/Audio/keypick.wav");


    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
}

// �f�X�g���N�^
Player::~Player()
{
    delete model;
    delete hitEffect;
    delete blowEffect;
}

// �X�V����
void Player::Update(float elapsedTime)
{
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
    }


    Mouse& mouse = Input::Instance().GetMouse();

    float ax = 0;
    float ay = 0;

    //ax = (mouse.GetPositionX() - mouse.GetOldPositionX());
    //ay = (mouse.GetPositionY() - mouse.GetOldPositionY());

    //// �J�����̉�]���x
    //float speed = 0.1f * elapsedTime;

    //if (!onClimb)
    //{
    //    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    //    angle.x += ay * speed;
    //    angle.y += ax * speed;

    //    if (angle.x > maxAngleX) angle.x = maxAngleX;
    //    if (angle.x < mixAngleX) angle.x = mixAngleX;

    //}
    //else
    //{
    //    angle.x += ay * speed;
    //}

    //int stage = StageMain::GetStageNum();


    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);

    // ���x�����X�V
    UpdateVelocity(elapsedTime);

    // ���G���ԍX�V����
    UpdateInvincibleTimer(elapsedTime);

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();
    CollisionProjectileVsEnemies();
    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    // ���f���s��X�V
    model->UpdateTransform(transform);

}

// �ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����

    Move(moveVec.x, moveVec.y, moveVec.z, moveSpeed);
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);



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
    if (!onClimb)
    {
        // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
        // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
        // �i�s�x�N�g�����v�Z����
        vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
        vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
        // Y���ɂ͈ړ����Ȃ�
        vec.y = 0;

    }
    else
    {
        vec.x = ax * cameraRightX;
        vec.y = ay * cameraUpY * 3.0f;
        vec.z = ax * cameraRightZ;
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

            // �ォ�瓥��Â����ꍇ�͏��W�����v����
            if (normal.y > 0.8)
            {
                Jump(jumpSpeed * 0.5f);
                enemy->ApplyDamage(1, 0.0f);
                DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();
                enemyPos.y += enemy->GetHeight();
                //if (enemy->GetHealth() != 0) hitEffect->Play(enemyPos);
                //else blowEffect->Play({ enemyPos.x, enemyPos.y -= enemy->GetHeight() * 0.5f, enemyPos.z });
            }
            else
            {
                // �����o����̈ʒu�ݒ�
                enemy->SetPosition(outPosition);
            }
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
                if (enemy->ApplyDamage(1, 0.5f))
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
                        //if (enemy->GetHealth() != 0) hitEffect->Play(enemyPos);
                        //else blowEffect->Play(enemyPos);
                    }
                }
            }
        }
    }
}

void Player::PlayAttackAnimation()
{
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

void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //���i�e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {

        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0;
        dir.z = cosf(angle.y);
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);

        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);

        SceneGame& sceneGame = SceneGame::Instance();

        if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        {
            sceneGame.RegisterRenderModel(projectile->GetModel());

        }


    }

}

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
                if (enemy->ApplyDamage(1, 1.0f))
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
                        //hitEffect->Play(e);
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
    if (onClimb)
    {
        model->PlayAnimation(Anim_HoldInWall, true);
    }
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
    //CheckHaveWall();
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

    //if (InputDodge())
    //{
    //    TransitionDodgeState();
    //}

    if (!onClimb)
    {
        //�e�ۓ��͏���
        InputProjectile();

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
    //CheckHaveWall();
    if (hitWall && onClimb)
    {
        TransitionClimbWallState();
    }

    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    //�W�����v���͏���
    InputJump();

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

void Player::TransitionJumpState()
{
    state = State::Jump;
    model->PlayAnimation(Anim_Jump, false);
}

void Player::UpdateJumpState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
    {
        TransitionSwingState();
    }

    if (onClimb)
    {
        onClimb = false;
    }
    //�ړ����͏���
    InputMove(elapsedTime);
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }


    //if (InputJump())
    //{
    //    model->PlayAnimation(Anim_Jump_Flip, false);
    //}
    //if (!model->IsPlayAnimation())
    //{
    //    model->PlayAnimation(Anim_Falling, false);
    //}
    InputProjectile();

}

void Player::TransitionClimbWallState()
{
    state = State::Climb;
    model->PlayAnimation(Anim_Climb, true);
}

void Player::UpdateClimbWallState(float elapsedTime)
{
    //CheckHaveWall();
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    if (!hitWall)
    {
        //model->PlayAnimation(Anim_ClimbUpWall, false);
        TransitionMoveState();
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

    InputProjectile();


    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 1.0f;

    if (attackCollisionFlag)
    {
        switch (attackCount)
        {
        case 1:
            CollisionNodeVsEnemies("mixamorig:LeftHandMiddle1", HandRadius);
            break;
        case 2:
            CollisionNodeVsEnemies("mixamorig:RightHandMiddle2", HandRadius);
            break;
        case 3:
            CollisionNodeVsEnemies("mixamorig:RightFoot", HandRadius + 0.2f);
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
                HandRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1));
        }
        if (attackCount == 2)
        {
            Model::Node* rightHandBone = model->FindNode("mixamorig:RightHand");
            debugRender->DrawSphere(DirectX::XMFLOAT3(
                rightHandBone->worldTransform._41,
                rightHandBone->worldTransform._42,
                rightHandBone->worldTransform._43),
                HandRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1));
        }
        if (attackCount == 3)
        {
            Model::Node* rightFootBone = model->FindNode("mixamorig:RightFoot");
            debugRender->DrawSphere(DirectX::XMFLOAT3(
                rightFootBone->worldTransform._41,
                rightFootBone->worldTransform._42,
                rightFootBone->worldTransform._43),
                HandRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1));
        }


    }

}

// �W�����v���͏���
bool Player::InputJump()
{
    // �{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
    {
        jumpCount++;
        switch (jumpCount)
        {
        case 1:
            // �W�����v���͂���
            Jump(jumpSpeed);
            return true;
            break;
        default:
            break;
        }

    }
    return false;
}

void Player::OnLanding()
{
    jumpCount = 0;

    //�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
    if (velocity.y < gravity * 5.0f)
    {
        TransitionLandState();
    }

    if (state != State::Death && state != State::Damage)
    {
        TransitionLandState();
    }

}

void Player::TransitionLandState()
{
    state = State::Land;
    model->PlayAnimation(Anim_Landing, false);

}

void Player::UpdateLandState(float elapsedTime)
{
    onClimb = false;
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionSwingState()
{
    state = State::Swing;
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&moveVec);

    DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR swingDirection = DirectX::XMVectorAdd(forwardVec, upVec);

    swingDirection = DirectX::XMVector3Normalize(swingDirection);

    DirectX::XMVECTOR SwingPoint = DirectX::XMLoadFloat3(&position); 
    SwingPoint = DirectX::XMVectorAdd(SwingPoint, DirectX::XMVectorScale(swingDirection, 10.0f));

    DirectX::XMStoreFloat3(&swingPoint, SwingPoint);

}

void Player::UpdateSwingState(float elapsedTime)
{
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&swingPoint);
    DirectX::XMVECTOR Q = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR displacement = DirectX::XMVectorSubtract(Q, P);

    float currentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(displacement));

    const float ropeLength = 3.0f;

    float springConstant = 12.0f;
    float elasticForce = -(springConstant * (currentLength - ropeLength));
    float dampingFactor = 0.95f;

    DirectX::XMVECTOR forceDirection = DirectX::XMVector3Normalize(displacement);
    DirectX::XMVECTOR force = DirectX::XMVectorScale(forceDirection, elasticForce);

    DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&velocity);
    velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorScale(force, elapsedTime));
    velocityVec = DirectX::XMVectorScale(velocityVec, dampingFactor);

    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(Q, DirectX::XMVectorScale(velocityVec, elapsedTime));
    DirectX::XMStoreFloat3(&position, newPosition);

    DirectX::XMStoreFloat3(&velocity, velocityVec);

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
    {
        TransitionIdleState();
        velocity = { 0.0f, 0.0f, 0.0f };
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

            ImGui::SliderInt("PlayerHP", &health, 0.0f, 10.f);
            ImGui::InputInt("attackCount", &attackCount);
            ImGui::InputFloat("attackTimer", &attackTimer);

            if (hitWall)
            {
                ImGui::Text("Is Hit Wall");
            }
            else ImGui::Text("OFF");

            if (onClimb)
            {
                ImGui::Text("On Climb");
            }
            else ImGui::Text("OFF");

            if (CanClimb)
            {
                ImGui::Text("Can Climb");
            }
            else ImGui::Text("Can't Climb");
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