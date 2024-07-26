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
    model = new Model("Data/Model/Jammo/Jammo.mdl");
    model->SetEnableRootMotion(true);

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
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    //{
    //    model->PlayAnimation(0,false,0.1f);
    //}
    //if (!model->IsPlayAnimation())
    //{
    //    model->PlayAnimation(1, true);
    //}

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
    case State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    case State::Dodge:
        UpdateDodgeState(elapsedTime);
        break;
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

    UpdateInvincibleTimer(elapsedTime);

}


bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);
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
        pos.y = position.y+height*0.5f;
        pos.z = position.z;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);
    }
    //�ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        DirectX::XMFLOAT3 dir;
        dir.x =sinf(angle.y);
        dir.y =0.0f;
        dir.z =cosf(angle.y);

        DirectX::XMFLOAT3 pos;
        pos.x =position.x;
        pos.y =position.y + height * 0.5f;
        pos.z =position.z;

        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.f;
        target.y = pos.y + dir.y * 1000.f;
        target.z = pos.z + dir.z * 1000.f;

        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; i++)
        {
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d,D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
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
            if (enemy->ApplyDamage(1,1.0f))
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

    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = 
        sqrtf(cameraRightX* cameraRightX+
        cameraRightZ* cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = 
        sqrtf(cameraFrontX * cameraFrontX +
            cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    vec.y = 0.0f;

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
   
    if (ImGui::Begin("Player",nullptr,ImGuiWindowFlags_None))
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
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRender->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
            );
    }
}

void Player::TransitionIdleState()
{
    state = State::Idle;
    model->PlayAnimation(Anim_Idle, true);
}

void Player::UpdateIdleState(float elapsedTime)
{
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

    if (InputDodge())
    {
        TransitionDodgeState();
    }

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
    if (InputJump())
    {
        model->PlayAnimation(Anim_Jump_Flip, false);
    }
    if (!model->IsPlayAnimation())
    {
        model->PlayAnimation(Anim_Falling, false);
    }
    InputProjectile();
}

void Player::TransitionLandState()
{
    state = State::Land;
    model->PlayAnimation(Anim_Landing, false);

}

void Player::UpdateLandState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

bool Player::InputAttack()
{
    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        return true;
    }
    return false;

}

void Player::TransitionAttackState()
{
    state = State::Attack;
    model->PlayAnimation(Anim_Attack, false, 0.1f);
}

void Player::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    InputProjectile();


    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

bool Player::InputDodge()
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    if (gamepad.GetButtonDown() & GamePad::BTN_SPACE)
    {
        return true;
    }
    return false;
}

void Player::TransitionDodgeState()
{
    state = State::Attack;
    model->PlayAnimation(Anim_Dodge, false, 0.1f);
}

void Player::UpdateDodgeState(float elapsedTime)
{
    if (InputAttack())
    {
        TransitionAttackState();
    }

    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

}

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
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }
    }
}

void Player::TransitionReviveState()
{
    state = State::Revive;

    health = maxHealth;

    model->PlayAnimation(Anim_Revive, false);
}

void Player::UpdateReviveState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
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
    if (velocity.y < gravity * 5.0f)
    {
        TransitionLandState();
    }

    if (state != State::Death && state != State::Damage)
    {
        TransitionLandState();
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
                   if (enemy->ApplyDamage(1,1.0f))
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
