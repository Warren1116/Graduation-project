#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "ProjectileManager.h"

static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//�R���X�g���N�^
Player::Player()
{
    instance = this;

    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    model = new Model("Data/Model/Spider-man/spider-man.mdl");
    model->GetNodePoses(nodePoses);

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    hitEffect = new Effect("Data/Effect/Hit.efk");

    TransitionIdleState();

    
}

//�f�X�g���N�^
Player::~Player()
{
    delete hitEffect;
    delete model;
}

void Player::Update(float elapsedTime)
{
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
        //case State::Land:
        //    UpdateLandState(elapsedTime);
        //    break;
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
        //case State::Revive:
        //    UpdateReviveState(elapsedTime);
        //    break;
        //case State::Dodge:
        //    UpdateDodgeState(elapsedTime);
        //    break;
    }


    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);

    //�v���C���[�ƓG�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();

    //�e�ۂƓG�̏Փˏ���
    CollisionProjectileVsEnemies();

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);

    UpdateMotion(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

}


bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����

    Move(moveVec.x,moveVec.y,moveVec.z, moveSpeed);


    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);


    return moveVec.x != 0 || moveVec.y != 0 || moveVec.z != 0;
}


void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 outPosition;

        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))

        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {
                Jump((jumpSpeed * 0.5f));
            }
            else
            {
                enemy->SetPosition(outPosition);
            }


        }
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
    }
}

void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    //�m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //�w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(nodePosition,
            nodeRadius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))

        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {
                Jump((jumpSpeed * 0.5f));
            }
            else
            {
                enemy->SetPosition(outPosition);
            }
            //�_���[�W��^����
            if (enemy->ApplyDamage(1, 1.0f))
            {
                //������΂�
                {
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    const DirectX::XMFLOAT3& p = position;

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

            }

        }
    }


}

void Player::OnDamaged()
{
    TransitionDamageState();
}

void Player::OnDead()
{
    TransitionDeathState();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    const DirectX::XMFLOAT3& cameraUp = camera.GetUp();

    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightY = cameraRight.y;
    float cameraRightLength =
        sqrtf(cameraRightX * cameraRightX +
            cameraRightY * cameraRightY + 
            cameraRightZ * cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
        cameraRightY /= cameraRightLength;
    }

    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontY = cameraFront.y;
    float cameraFrontLength =
        sqrtf(cameraFrontX * cameraFrontX +
            cameraFrontY * cameraFrontY +
            cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
        cameraFrontY /= cameraFrontLength;
    }

    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    if (hitWall && onClimb)
    {
        vec.y = (cameraRightY * ax) + (cameraFrontY * ay);
    }
    else
    {
        vec.y = 0.0f;
    }

    return vec;

}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
    projectileManager.Render(dc, shader);
}

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
                ImGui::Text("ON");
            }
            else ImGui::Text("OFF");
        }

    }
    ImGui::End();
}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    //�Փ˔���p�̃f�o�b�O����`��
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

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

void Player::TransitionIdleState()
{
    if (attacking)
    {
        if (attackCount >= attackLimit ||  attackTimer > 90)
        {
            attackTimer = 0;
            attackCount = 0;
            attacking = false;
        }
    }
    state = State::Idle;
    model->PlayAnimation(Anim_Idle, true);
}

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

    //if (InputDodge())
    //{
    //    TransitionDodgeState();
    //}

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

void Player::TransitionMoveState()
{
    state = State::Move;

    model->PlayAnimation(Anim_Running, true);
    

}

void Player::UpdateMoveState(float elapsedTime)
{
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

//void Player::TransitionLandState()
//{
//    state = State::Land;
//    model->PlayAnimation(Anim_Landing, false);
//
//}

//void Player::UpdateLandState(float elapsedTime)
//{
//    if (!model->IsPlayAnimation())
//    {
//        TransitionIdleState();
//    }
//}

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

void Player::TransitionAttackState()
{
    state = State::Attack;
    PlayAttackAnimation();
    
}

void Player::UpdateAttackState(float elapsedTime)
{
    if (attacking)
    {
        attackTimer++;
    }
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    InputProjectile();


    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.6f;
    
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

//bool Player::InputDodge()
//{
//    //GamePad& gamepad = Input::Instance().GetGamePad();
//    //if (gamepad.GetButtonDown() & GamePad::BTN_SPACE)
//    //{
//    //    return true;
//    //}
//    return false;
//}

//void Player::TransitionDodgeState()
//{
//    state = State::Dodge;
//    model->PlayAnimation(Anim_Dodge, false, 0.1f);
//}
//
//void Player::UpdateDodgeState(float elapsedTime)
//{
//    //animationIndex = model->GetAnimationIndex("Dodge");
//    //animationSeconds = oldAnimationSeconds = 0;
//
//    // �A�j���[�V�����؂�ւ�����
//    int newAnimationIndex = animationIndex;
//    if (GetAsyncKeyState(VK_UP) & 0x8000)
//    {
//        newAnimationIndex = model->GetAnimationIndex("Dodge");
//    }
//    else
//    {
//        newAnimationIndex = model->GetAnimationIndex("Idle");
//    }
//    if (animationIndex != newAnimationIndex)
//    {
//        animationIndex = newAnimationIndex;
//        animationSeconds = oldAnimationSeconds = 0;
//    }
//
//
//    // �A�j���[�V�����X�V����
//    if (animationIndex >= 0)
//    {
//        const Model::Animation& animation = model->GetAnimations().at(animationIndex);
//
//        // �w�莞�Ԃ̃A�j���[�V�����̎p�����擾
//        model->ComputeAnimation(animationIndex, animationSeconds, nodePoses);
//
//        // TODO�@:���[�g���[�V�����������s���A�L�����N�^�[���ړ�����
//        {
//            // ���[�g���[�V�����m�[�h�ԍ��擾
//            const int rootMotionNodeIndex = model->GetNodeIndex("mixamorig:Hips");
//
//            //����A�O��A����̃��[�g���[�V�����m�[�h�̎p�����擾
//            Model::NodePose beginPose, oldPose, nowPose;
//            model->ComputeAnimation(animationIndex, rootMotionNodeIndex, 0.f, beginPose);
//            model->ComputeAnimation(animationIndex, rootMotionNodeIndex, oldAnimationSeconds, oldPose);
//            model->ComputeAnimation(animationIndex, rootMotionNodeIndex, animationSeconds, nowPose);
//
//            DirectX::XMFLOAT3 localTranslation; //���[�J���̈ړ��ʂ��i�[����ϐ�
//            if (oldAnimationSeconds > animationSeconds)
//            {
//                // TODO�A:���[�v�A�j���[�V�����ɑΉ�����
//                // �I�[�̎p�����擾
//                //���[�J���ړ��l���Z�o
//                Model::NodePose endPose;
//                model->ComputeAnimation(animationIndex, rootMotionNodeIndex,
//                    animation.secondsLength, endPose);
//                DirectX::XMFLOAT3 endDiff, beginDiff;
//                endDiff.x = endPose.position.x - oldPose.position.x;
//                endDiff.y = endPose.position.y - oldPose.position.y;
//                endDiff.z = endPose.position.z - oldPose.position.z;
//
//                beginDiff.x = nowPose.position.x - beginPose.position.x;
//                beginDiff.y = nowPose.position.y - beginPose.position.y;
//                beginDiff.z = nowPose.position.z - beginPose.position.z;
//
//                //�Z�o�����ړ�
//                localTranslation.x = beginDiff.x + endDiff.x;
//                localTranslation.y = beginDiff.y + endDiff.y;
//                localTranslation.z = beginDiff.z + endDiff.z;
//
//
//            }
//            else
//            {
//                //���[�J���ړ��l���Z�o
//                // �ړ������̍������Z�o
//                localTranslation.x = nowPose.position.x - oldPose.position.x;
//                localTranslation.y = nowPose.position.y - oldPose.position.y;
//                localTranslation.z = nowPose.position.z - oldPose.position.z;
//
//            }
//            //�O���[�o���ړ��l���Z�o
//            Model::Node& rootMotionModel = model->GetNodes().at(rootMotionNodeIndex);
//
//            //rootMotionNode�̐e�m�[�h(parent)����O���[�o���s����擾�ł���̂ŁA
//            //���[�J���̈ړ��ʂ��O���[�o����ԂɈړ������܂��B
//            DirectX::XMMATRIX ParentGlobalTransform;
//            ParentGlobalTransform = DirectX::XMLoadFloat4x4(&rootMotionModel.parent->globalTransform);
//
//            DirectX::XMVECTOR LocalTranslation = DirectX::XMLoadFloat3(&localTranslation);
//
//            DirectX::XMVECTOR GlobalTranslation = DirectX::XMVector3TransformNormal(LocalTranslation, ParentGlobalTransform);
//
//            if (bakeTranslationY)
//            {
//                // TODO�B:���[�g���[�V������Y�ړ��͓K�p���Ȃ��悤�ɂ���
//                // Y�����̈ړ��l�𔲂�
//                GlobalTranslation = DirectX::XMVectorSetY(GlobalTranslation, 0);
//
//                //����̎p���̃O���[�o���ʒu���Z�o
//                DirectX::XMVECTOR LocalPosition, GlobalPosition;
//                LocalPosition = DirectX::XMLoadFloat3(&nowPose.position);
//                GlobalPosition = DirectX::XMVector3Transform(LocalPosition, ParentGlobalTransform);
//
//                // XZ�������폜
//                GlobalPosition = DirectX::XMVectorSetX(GlobalPosition, 0);
//                GlobalPosition = DirectX::XMVectorSetZ(GlobalPosition, 0);
//
//                //���[�J����ԕϊ�
//                DirectX::XMMATRIX InverseParentGblobalTransform;
//                InverseParentGblobalTransform = DirectX::XMMatrixInverse(nullptr, ParentGlobalTransform);
//
//                LocalPosition = DirectX::XMVector3Transform(GlobalPosition, InverseParentGblobalTransform);
//
//                //���[�g���[�V�����m�[�h�̈ʒu��ݒ�
//                DirectX::XMStoreFloat3(&nodePoses[rootMotionNodeIndex].position, LocalPosition);
//            }
//            else
//            {
//                //���[�g���[�V�����m�[�h������̎p���ɂ���
//                nodePoses[rootMotionNodeIndex].position = beginPose.position;
//            }
//            //���[���h�ړ��l���Z�o
//            DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);
//            DirectX::XMVECTOR WorldTranslation;
//
//            WorldTranslation = DirectX::XMVector3TransformNormal(GlobalTranslation, WorldTransform);
//
//            DirectX::XMFLOAT3 worldTranslation{};
//            DirectX::XMStoreFloat3(&worldTranslation, WorldTranslation);
//
//            //�L�����N�^�[�̈ʒu���X�V
//            position.x += worldTranslation.x;
//            position.y += worldTranslation.y;
//            position.z += worldTranslation.z;
//        }
//
//        // �A�j���[�V�������ԍX�V
//        oldAnimationSeconds = animationSeconds;
//        animationSeconds += elapsedTime;
//        if (animationSeconds > animation.secondsLength)
//        {
//            if (animationLoop)
//            {
//                animationSeconds -= animation.secondsLength;
//            }
//            else
//            {
//                animationSeconds = animation.secondsLength;
//            }
//        }
//
//        // �p���X�V
//        model->SetNodePoses(nodePoses);
//    }
//
//    // ���[���h�g�����X�t�H�[���X�V
//    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//
//    DirectX::XMMATRIX WorldTransform = S * R * T;
//    DirectX::XMStoreFloat4x4(&worldTransform, WorldTransform);
//
//    // ���f���g�����X�t�H�[���X�V����
//    model->UpdateTransform(worldTransform);
//
//    if (!model->IsPlayAnimation())
//    {
//        TransitionIdleState();
//    }
//
//}

void Player::TransitionDamageState()
{
    state = State::Damage;

    model->PlayAnimation(Anim_GetHit1, false);
}

void Player::UpdateDamageState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    model->PlayAnimation(Anim_Death, false);
}

void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        //if (gamePad.GetButtonDown() & GamePad::BTN_A)
        //{
        //    TransitionReviveState();
        //}
    }
}

void Player::TransitionClimbWallState()
{
    state = State::Climb;
    model->PlayAnimation(Anim_Climb, true);
}

void Player::UpdateClimbWallState(float elapsedTime)
{
    InputMove(elapsedTime);
    if (!hitWall)
    {
        TransitionMoveState();
    }
}

//void Player::TransitionReviveState()
//{
//    state = State::Revive;
//
//    health = maxHealth;
//
//    model->PlayAnimation(Anim_Revive, false);
//}
//
//void Player::UpdateReviveState(float elapsedTime)
//{
//    if (!model->IsPlayAnimation())
//    {
//        TransitionIdleState();
//    }
//}

void Player::UpdateMotion(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();
    //  �X�y�[�X�{�^���������Ɖ���@�i�������j
    if (gamepad.GetButtonDown() & GamePad::BTN_SPACE)
    {
        int index = model->GetAnimationIndex("Dodge");
        model->PlayAnimation(index, false);
    }
    model->UpdateAnimation(elapsedTime);
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

bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //�@Z�{�^����������
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE && !onClimb)
    {

        if (jumpCount < jumpLimit)
        {
            jumpCount++;
            Jump(jumpSpeed);
            return true;
        }
    }
    return false;
}

void Player::OnLanding()
{
    jumpCount = 0;

    //�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
    //if (velocity.y < gravity * 5.0f)
    //{
    //    TransitionLandState();
    //}

    //if (state != State::Death && state != State::Damage)
    //{
    //    TransitionLandState();
    //}

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
                        hitEffect->Play(e);
                    }
                    //�e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }

}
