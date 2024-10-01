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

//コンストラクタ
Player::Player()
{
    instance = this;

    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    model = new Model("Data/Model/Spider-man/spider-man.mdl");
    model->GetNodePoses(nodePoses);

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;

    hitEffect = new Effect("Data/Effect/Hit.efk");

    TransitionIdleState();

    
}

//デストラクタ
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


    //速力処理更新
    UpdateVelocity(elapsedTime);

    //弾丸更新処理
    projectileManager.Update(elapsedTime);

    //プレイヤーと敵との衝突処理
    CollisionPlayerVsEnemies();

    //弾丸と敵の衝突処理
    CollisionProjectileVsEnemies();

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);

    UpdateMotion(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

}


bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //移動処理

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

    //直進弾丸発射
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
    //ノード取得
    Model::Node* node = model->FindNode(nodeName);
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //指定のノードと全ての敵を総当たりで衝突処理
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
            //ダメージを与える
            if (enemy->ApplyDamage(1, 1.0f))
            {
                //吹き飛ばす
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

                //エフェクト生成
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

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    //衝突判定用のデバッグ球を描画
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //衝突判定用のデバッグ円柱を描画
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
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        TransitionJumpState();
    }

    //if (InputDodge())
    //{
    //    TransitionDodgeState();
    //}

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
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

    //ジャンプ入力処理
    InputJump();

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
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
    //移動入力処理
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
//    // アニメーション切り替え操作
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
//    // アニメーション更新処理
//    if (animationIndex >= 0)
//    {
//        const Model::Animation& animation = model->GetAnimations().at(animationIndex);
//
//        // 指定時間のアニメーションの姿勢を取得
//        model->ComputeAnimation(animationIndex, animationSeconds, nodePoses);
//
//        // TODO①:ルートモーション処理を行い、キャラクターを移動せよ
//        {
//            // ルートモーションノード番号取得
//            const int rootMotionNodeIndex = model->GetNodeIndex("mixamorig:Hips");
//
//            //初回、前回、今回のルートモーションノードの姿勢を取得
//            Model::NodePose beginPose, oldPose, nowPose;
//            model->ComputeAnimation(animationIndex, rootMotionNodeIndex, 0.f, beginPose);
//            model->ComputeAnimation(animationIndex, rootMotionNodeIndex, oldAnimationSeconds, oldPose);
//            model->ComputeAnimation(animationIndex, rootMotionNodeIndex, animationSeconds, nowPose);
//
//            DirectX::XMFLOAT3 localTranslation; //ローカルの移動量を格納する変数
//            if (oldAnimationSeconds > animationSeconds)
//            {
//                // TODO②:ループアニメーションに対応せよ
//                // 終端の姿勢を取得
//                //ローカル移動値を算出
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
//                //算出した移動
//                localTranslation.x = beginDiff.x + endDiff.x;
//                localTranslation.y = beginDiff.y + endDiff.y;
//                localTranslation.z = beginDiff.z + endDiff.z;
//
//
//            }
//            else
//            {
//                //ローカル移動値を算出
//                // 移動したの差分を算出
//                localTranslation.x = nowPose.position.x - oldPose.position.x;
//                localTranslation.y = nowPose.position.y - oldPose.position.y;
//                localTranslation.z = nowPose.position.z - oldPose.position.z;
//
//            }
//            //グローバル移動値を算出
//            Model::Node& rootMotionModel = model->GetNodes().at(rootMotionNodeIndex);
//
//            //rootMotionNodeの親ノード(parent)からグローバル行列を取得できるので、
//            //ローカルの移動量をグローバル空間に移動させます。
//            DirectX::XMMATRIX ParentGlobalTransform;
//            ParentGlobalTransform = DirectX::XMLoadFloat4x4(&rootMotionModel.parent->globalTransform);
//
//            DirectX::XMVECTOR LocalTranslation = DirectX::XMLoadFloat3(&localTranslation);
//
//            DirectX::XMVECTOR GlobalTranslation = DirectX::XMVector3TransformNormal(LocalTranslation, ParentGlobalTransform);
//
//            if (bakeTranslationY)
//            {
//                // TODO③:ルートモーションのY移動は適用しないようにせよ
//                // Y成分の移動値を抜く
//                GlobalTranslation = DirectX::XMVectorSetY(GlobalTranslation, 0);
//
//                //今回の姿勢のグローバル位置を算出
//                DirectX::XMVECTOR LocalPosition, GlobalPosition;
//                LocalPosition = DirectX::XMLoadFloat3(&nowPose.position);
//                GlobalPosition = DirectX::XMVector3Transform(LocalPosition, ParentGlobalTransform);
//
//                // XZ成分を削除
//                GlobalPosition = DirectX::XMVectorSetX(GlobalPosition, 0);
//                GlobalPosition = DirectX::XMVectorSetZ(GlobalPosition, 0);
//
//                //ローカル空間変換
//                DirectX::XMMATRIX InverseParentGblobalTransform;
//                InverseParentGblobalTransform = DirectX::XMMatrixInverse(nullptr, ParentGlobalTransform);
//
//                LocalPosition = DirectX::XMVector3Transform(GlobalPosition, InverseParentGblobalTransform);
//
//                //ルートモーションノードの位置を設定
//                DirectX::XMStoreFloat3(&nodePoses[rootMotionNodeIndex].position, LocalPosition);
//            }
//            else
//            {
//                //ルートモーションノードを初回の姿勢にする
//                nodePoses[rootMotionNodeIndex].position = beginPose.position;
//            }
//            //ワールド移動値を算出
//            DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);
//            DirectX::XMVECTOR WorldTranslation;
//
//            WorldTranslation = DirectX::XMVector3TransformNormal(GlobalTranslation, WorldTransform);
//
//            DirectX::XMFLOAT3 worldTranslation{};
//            DirectX::XMStoreFloat3(&worldTranslation, WorldTranslation);
//
//            //キャラクターの位置を更新
//            position.x += worldTranslation.x;
//            position.y += worldTranslation.y;
//            position.z += worldTranslation.z;
//        }
//
//        // アニメーション時間更新
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
//        // 姿勢更新
//        model->SetNodePoses(nodePoses);
//    }
//
//    // ワールドトランスフォーム更新
//    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//
//    DirectX::XMMATRIX WorldTransform = S * R * T;
//    DirectX::XMStoreFloat4x4(&worldTransform, WorldTransform);
//
//    // モデルトランスフォーム更新処理
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
    //  スペースボタンを押すと回避　（未完成）
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
    //　Zボタンを押すと
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

    //下方向の速力が一定以上なら着地ステートへ
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

    //すべての弾丸とすべての敵を総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                //ダメージを与える
                if (enemy->ApplyDamage(1, 1.0f))
                {
                    //吹き飛ばす
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

                    //エフェクト生成
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;
                        hitEffect->Play(e);
                    }
                    //弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }

}
