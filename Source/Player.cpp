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
#include "EventPointManager.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "EnemyThief.h"
Player* Player::instance = nullptr;

// コンストラクタ
Player::Player(bool flag)
{
    // インスタンスポイント設定
    instance = this;

    // モデル読み込み
    //model = std::make_unique<Model>("Data/Model/Jammo/jammo.mdl");
    model = std::make_unique<Model>("Data/Model/Spider-man/spider-man.mdl");
    model->GetNodePoses(nodePoses);

    radius = 0.5f;

    //Model::Node* RightHandPos = model->FindNode("mixamorig:RightHand");
    //DirectX::XMFLOAT3 pos = {};
    //pos.x = RightHandPos->worldTransform._41;
    //pos.y = RightHandPos->worldTransform._42;
    //pos.z = RightHandPos->worldTransform._43;


    //// ジョイント初期化
    //for (int i = 0; i < _countof(joints); ++i)
    //{
    //    Joint& joint = joints[i];
    //    joint.position.x = pos.x + i;
    //    joint.position.y = pos.y;
    //    joint.position.z = pos.z;
    //    joint.oldPosition = joint.position;
    //}

    // モデルが大きいのでスケーリング
    if (!flag)
    {
        position = { 0, 0, 0 };
    }

    scale.x = scale.y = scale.z = 0.01f;

    hitEffect = std::make_unique<Effect>("Data/Effect/hitEffect.efk");

    punch = Audio::Instance().LoadAudioSource("Data/Audio/punch.wav");
    punch2 = Audio::Instance().LoadAudioSource("Data/Audio/punch2.wav");
    kick = Audio::Instance().LoadAudioSource("Data/Audio/kick.wav");

    skillTime = 5.0f;

    // 待機ステートへ遷移
    //TransitionIdleState();
    TransitionCrouchIdleState();
    //TransitionTitleIdleState();

    //// メッセージ受信登録
    //EventModeIndex = Messenger::Instance().AddReceiver(MessageData::EVENTMODEEVENT, [&](void* data) { TransitionIdleState(); state = State::EventMode; });
    //GameModeIndex = Messenger::Instance().AddReceiver(MessageData::GAMEMODEEVENT, [&](void* data) { state = State::Idle; });

    //// イベントの情報登録
    //SetPositionIndex = Messenger::Instance().AddReceiver(MessageData::PLAYERSETPOSITIONEVENT,
    //    [&](void* data)
    //    {
    //        MessageData::PLAYERSETPOSITIONDATA* d = static_cast<MessageData::PLAYERSETPOSITIONDATA*>(data);
    //        position = d->position;
    //    });
    //MoveIndex = Messenger::Instance().AddReceiver(MessageData::PLAYERMOVEEVENT,
    //    [&](void* data)
    //    {
    //        MessageData::PLAYERMOVEDATA* d = static_cast<MessageData::PLAYERMOVEDATA*>(data);
    //        moveData.moveVec = d->moveVec;
    //        moveData.moveSpeed = d->moveSpeed;
    //        moveData.turnSpeed = d->turnSpeed;
    //    });
    //MotionIndex = Messenger::Instance().AddReceiver(MessageData::PLAYERMOTIONEVENT,
    //    [&](void* data)
    //    {
    //        MessageData::PLAYERMOTIONDATA* d = static_cast<MessageData::PLAYERMOTIONDATA*>(data);
    //        if (model->GetCurrentAnimationIndex())
    //            model->PlayAnimation(d->index, d->loop, d->blendSecond);
    //    });


}


// デストラクタ
Player::~Player()
{
    //Messenger::Instance().RemoveReceiver(EventModeIndex);
    //Messenger::Instance().RemoveReceiver(GameModeIndex);

    //Messenger::Instance().RemoveReceiver(SetPositionIndex);
    //Messenger::Instance().RemoveReceiver(MoveIndex);
    //Messenger::Instance().RemoveReceiver(MotionIndex);
}

// 更新処理
void Player::Update(float elapsedTime)
{
    //  カメラステートの更新処理
    UpdateCameraState(elapsedTime);

    // タイトルの場合
    {
        // ステート毎の処理
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
        case State::Dodge:
            UpdateDodgeState(elapsedTime);
            break;
        case State::ClimbTop:
            UpdateClimbTopState(elapsedTime);
            break;
        case State::Grab:
            UpdateGrabState(elapsedTime);
            break;
        case State::CrouchIdle:
            UpdateCrouchIdleState(elapsedTime);
            break;
        case State::TitleIdle:
            UpdateTitleIdleState(elapsedTime);
            break;
        }

        //弾丸更新処理
        projectileManager.Update(elapsedTime);
        brokenprojectileManager.Update(elapsedTime);

        // 速度処理更新
        UpdateVelocity(elapsedTime);

        // 無敵時間更新処理
        UpdateInvincibleTimer(elapsedTime);

        // プレイヤーとエネミーとの衝突処理
        CollisionPlayerVsEnemies();
        CollisionProjectileVsEnemies();

        //// イベントスクリプトポイントクリア
        //EventPointManager::Instance().CheckPoint(position, radius);


        // オブジェクト行列を更新
        UpdateTransform();

        // アニメーション更新
        model->UpdateAnimation(elapsedTime);

        // モデル行列更新
        model->UpdateTransform(transform);

        if (attacking)
        {
            attackTimer++;
        }
        if (skillTime < 5)
        {
            skillTime += elapsedTime * 0.01f;
        }
    }
}

// 移動入力処理
bool Player::InputMove(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //移動処理
    Move(moveVec.x, moveVec.y, moveVec.z, moveSpeed);
    if (!onClimb)
    {
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
    }

    return moveVec.x != 0 || moveVec.y != 0 || moveVec.z != 0;
}


// スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    const DirectX::XMFLOAT3& cameraUp = camera.GetUp();

    // カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLenght =
        sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLenght > 0.0f)
    {
        // 単位ベクトル化
        cameraRightX /= cameraRightLenght;
        cameraRightZ /= cameraRightLenght;
    }

    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLenght =
        sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLenght > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX /= cameraFrontLenght;
        cameraFrontZ /= cameraFrontLenght;
    }

    float cameraUpY = cameraUp.y;

    DirectX::XMFLOAT3 vec;
    {
        // スティックの水平入力値をカメラ右方向に反映し、
        // スティックの垂直入力値をカメラ前方向に反映し、
        // 進行ベクトルを計算する
        vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
        vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
        // Y軸には移動しない
        //vec.y = 0;
        vec.y = ay * cameraUpY * 3.0f;
    }

    return vec;
}


// プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        // 衝突処理
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

            // 押し出し後の位置設定
            enemy->SetPosition(outPosition);

        }
    }
}
// ノードと敵の衝突処理
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    // ノード取得
    Model::Node* node = model->FindNode(nodeName);

    if (node != nullptr)
    {
        // ノード位置取得
        DirectX::XMFLOAT3 nodePosition;
        nodePosition.x = node->worldTransform._41;
        nodePosition.y = node->worldTransform._42;
        nodePosition.z = node->worldTransform._43;


        // 指定のノードとすべての敵を総当たりで衝突処理
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);

            // 衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                nodeRadius,
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                // ダメージを与える
                if (enemy->ApplyDamage(3, 0.5f))
                {
                    // 吹き飛ばす
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

                    // ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();
                        enemyPos.y += enemy->GetHeight() * 1.5f;
                        if (enemy->GetHealth() != 0)
                        {
                            switch (attackCount)
                            {
                            case 1:
                                punch->Play(false);
                                break;
                            case 2:
                                punch2->Play(false);
                                break;
                            case 3:
                                kick->Play(false);
                                break;
                            }
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
    // 連撃のアニメーション
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

void Player::TransitionDodgeState()
{
    state = State::Dodge;
    model->PlayAnimation(Anim_Dodge, false);
}

void Player::UpdateDodgeState(float elapsedTime)
{
    invincibleTimer = 2.0f;
    DirectX::XMVECTOR vel = DirectX::XMLoadFloat3(&velocity);
    vel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&GetFront()), -110.0f);

    DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
    playerPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(vel, elapsedTime));

    DirectX::XMStoreFloat3(&velocity, vel);
    DirectX::XMStoreFloat3(&position, playerPos);

    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionClimbTopState()
{
    state = State::ClimbTop;
    model->PlayAnimation(Anim_ClimbUpWall, false);

}

void Player::UpdateClimbTopState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionGrabState()
{
    state = State::Grab;
    model->PlayAnimation(Anim_GrabAndDrop, false);
    webTimer = 0.0f;
    skillTime -= 1.0f;

}

void Player::UpdateGrabState(float elapsedTime)
{
    webTimer += elapsedTime;
    if (!model->IsPlayAnimation())
    {
        IsUseGrab = false;
        TransitionIdleState();
    }

    //  カメラロック中の処理
    if (lockonState == LockonState::Locked && lockonEnemy != nullptr)
    {
        if (webTimer <= 0.82)
        {
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));

            float moveDistance = 0.04f;
            DirectX::XMVECTOR moveVector = DirectX::XMVectorScale(directionToEnemy, moveDistance);

            DirectX::XMStoreFloat3(&lockDirection, directionToEnemy);
            Turn(elapsedTime, lockDirection.x, lockDirection.z, turnSpeed);
        }

        if (webTimer > 0.82f)
        {
            // 糸の登録
            SwingWeb* swingWebRight = new SwingWeb(&projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            //{
            //    //  レンダラーに登録
            //    sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            //}
            if (sceneGame.shadowmapCasterRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }

        lockonEnemy->ApplyDamage(20.0, 1.0);
    }

}

bool Player::IsNearWallTop()
{
    //　頭の位置を取得
    Model::Node* HeadPos = model->FindNode("mixamorig:Head");
    DirectX::XMFLOAT3 pos;
    pos.x = HeadPos->worldTransform._41;
    pos.y = HeadPos->worldTransform._42;
    pos.z = HeadPos->worldTransform._43;

    const float frontCheckDistance = 1.0f;
    const float verticalOffset = 0.5f;

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

    //レイの開始位置と終点位置
    DirectX::XMFLOAT3 start = { pos };
    DirectX::XMFLOAT3 end = {
        checkpos.x,
        checkpos.y,
        checkpos.z
    };
    checkpos = end;

    //レイキャストによる壁判定
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



//　カメラステート更新処理
void Player::UpdateCameraState(float elapsedTime)
{
    //  ロックオン・オフ判定用
    static bool tabPressed = false;

    CharacterManager& manager = CharacterManager::Instance();
    //  敵の数を取得
    int enemyCount = 0;
    for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
    {
        Character* character = manager.GetCharacter(ii);
        if (character != nullptr && character != this && character->GetHealth() > 0)
        {
            enemyCount++;
            break;
        }
    }

    //  敵がいない場合強制フリーカメラ
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
    case State::Dodge:
    {
        if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_TAB || Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_THUMB)
        {
            // Tabキーを押すと、ロック判定
            if (!tabPressed)
            {
                tabPressed = true;

                //  ロックの時
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
                else //次一番近い敵を比較して探す
                {
                    DirectX::XMVECTOR p, t, v;
                    float length1, length2;
                    CharacterManager& manager = CharacterManager::Instance();
                    for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
                    {
                        Character* character = manager.GetCharacter(ii);
                        if (character == this || character->GetHealth() <= 0)
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
                                lockonEnemy->SetLockedOn(false);
                                lockonEnemy = character;
                                lockonEnemy->SetLockedOn(true);

                                DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                            }
                        }
                        else
                        {
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

        //  今ロックしてる敵がまだ存在するかどうか（死亡）
        if (lockonState == LockonState::Locked && lockonEnemy != nullptr && lockonEnemy->GetHealth() == 0)
        {
            // 死亡したら次一番近いの敵を探す
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

            // 探せるなら新しい敵をロック
            if (lockonEnemy)
            {
                lockonEnemy->SetLockedOn(true);
                MessageData::CAMERACHANGELOCKONMODEDATA p = { position, lockonEnemy->GetPosition() };
                Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
            }
            else
            {
                // もし敵を全部倒したらFreeCameraに戻る
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

            //  敵を遠く過ぎならロックできないの処理
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
    //  死亡した時のカメラ
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
    case State::Grab:
    {
        MessageData::CAMERACHANGEMOTIONMODEDATA p;
        float vx = sinf(angle.y) * 5;
        float vz = -cosf(angle.y) * 5;
        p.data.push_back({ 0, { position.x + vx, position.y + 3, position.z + vz }, position });
        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
        break;
    }

    }
}


//  発射ステートべの遷移
void Player::TransitionShotingState()
{
    state = State::Shot;
    model->PlayAnimation(Anim_Shoting, false);
}

//  発射ステートの更新処理
void Player::UpdateShotingState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//  弾丸の入力処理
bool Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        DirectX::XMFLOAT3 dir;
        if (lockonEnemy)    //　もしカメラロックしてる場合は敵を向かて発射
        {
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&lockonEnemy->GetPosition());
            DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));
            DirectX::XMStoreFloat3(&lockDirection, direction);

            angle.y = atan2f(lockDirection.x, lockDirection.z);
            dir = lockDirection;
        }
        else  //　ロックしてないなら直進弾丸発射
        {
            dir.x = sinf(angle.y);
            dir.y = 0;
            dir.z = cosf(angle.y);
        }

        //　弾が手から出るように、右手のNodeを探す
        Model::Node* RightHandPos = model->FindNode("mixamorig:RightHand");
        DirectX::XMFLOAT3 pos;
        pos.x = RightHandPos->worldTransform._41;
        pos.y = RightHandPos->worldTransform._42;
        pos.z = RightHandPos->worldTransform._43;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);

        //  モデルをレンダラーに登録
        SceneGame& sceneGame = SceneGame::Instance();
        //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        //{
        //    sceneGame.RegisterRenderModel(projectile->GetModel());

        //}
        if (sceneGame.shadowmapCasterRenderer && sceneGame.sceneRenderer)
        {
            //  レンダラーに登録
            sceneGame.RegisterRenderModel(projectile->GetModel());
        }
        return true;
    }
    return false;
}

//  弾丸と敵との衝突処理
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

                if (enemy->ApplyDamage(3, 1.0f))
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
                        e.y += enemy->GetHeight() * 1.5f;
                        hitEffect->Play(e);
                    }
                    //弾丸破棄
                    projectile->Destroy();
                    SceneGame::Instance().UnregisterRenderModel(projectile->GetModel());
                }
            }
        }
    }

}

// 攻撃入力処理
bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT || gamePad.GetButtonDown() & GamePad::BTN_X)
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

bool Player::InputDodge()
{
    // ボタン入力でジャンプ
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE || gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        return true;
    }
    return false;

}

// 待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;
    onSwing = false;
    firstSwing = true;

    //　クライミング中なら別の待機モーション
    if (onClimb)
    {
        model->PlayAnimation(Anim_HoldInWall, true);
    }
    //　普通も待機モーション
    else
    {
        model->PlayAnimation(Anim_Idle, true);
        if (attacking)
        {
            if (attackCount >= attackLimit || attackTimer > 100)
            {
                attackTimer = 0;
                attackCount = 0;
                attacking = false;
            }
        }
    }
}

// 待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
    lastState = state;
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //ジャンプ入力処理
    if (!GetAttackSoon() && InputJump())
    {
        TransitionJumpState();

    }
    else if (GetAttackSoon() && InputDodge())
    {
        TransitionDodgeState();
    }


    if (!onClimb)
    {
        //弾丸入力処理
        if (InputProjectile())
        {
            TransitionShotingState();
        }

        //攻撃入力処理
        if (InputAttack())
        {
            TransitionAttackState();
        }
    }
    if (lockonEnemy && skillTime >= 1)
    {
        Mouse& mouse = Input::Instance().GetMouse();
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (mouse.GetButtonDown() & Mouse::BTN_RIGHT || gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER)
        {
            IsUseGrab = true;
            TransitionGrabState();
        }
    }


}

// 移動ステートへ遷移
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

// 移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //  移動中SHIFTキー同時に押すと
    HitResult hit;
    //if (FindWallSwingPoint(position, 5.0f, hit))
    //{
    if ((gamePad.GetButton() & GamePad::BTN_SHIFT || gamePad.GetButton() & GamePad::BTN_RIGHT_TRIGGER) && InputMove(elapsedTime) && firstSwing)
    {
        lastState = state;
        TransitionSwingState();
    }
    //}

    if (GetAttackSoon() && InputDodge())
    {
        TransitionDodgeState();
    }

    //  目の前に壁がある同時にSpeacキー押したら
    if (hitWall && onClimb)
    {
        //クライミングステートへの遷移
        TransitionClimbWallState();
    }

    //  移動してないなら
    if (!InputMove(elapsedTime))
    {
        //待機ステートへの遷移
        TransitionIdleState();
    }

    //　もし目の前に壁がいない、ジャンプ入力処理
    if (!hitWall && InputJump())
    {
        //ジャンブステートへの遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    if (InputProjectile())
    {
        TransitionShotingState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

//  ジャンブステート
void Player::TransitionJumpState()
{
    state = State::Jump;

    model->PlayAnimation(Anim_Jump, false);

}

//  ジャンブステートの更新処理
void Player::UpdateJumpState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //  ジャンブ中移動とSHIFTキー同時に押すと
    HitResult hit;
    //if (FindWallSwingPoint(position, 5.0f, hit))
    //{
    if ((gamePad.GetButton() & GamePad::BTN_SHIFT || gamePad.GetButton() & GamePad::BTN_RIGHT_TRIGGER) && InputMove(elapsedTime))
    {
        lastState = state;
        //  スイングステートへの遷移
        TransitionSwingState();
    }
    //}

    //  もしクライミング中なら、クライミング状態をキャンセルする
    if (onClimb)
    {
        onClimb = false;
    }

    //移動入力処理
    InputMove(elapsedTime);

    if (!model->IsPlayAnimation())
    {
        // 待機ステートへの遷移
        TransitionIdleState();
    }

    //  弾丸の入力処理
    if (InputProjectile())
    {
        //  発射ステートへの遷移
        TransitionShotingState();
    }

}

//　クライミングステート
void Player::TransitionClimbWallState()
{
    state = State::Climb;
    if (velocity.y >= 0)
    {
        model->PlayAnimation(Anim_Climb, true);
    }
    else
    {
        model->PlayAnimation(Anim_ClimbDown, true);
    }
}

//  クライミングステートへの遷移
void Player::UpdateClimbWallState(float elapsedTime)
{
    //　クライミング中Spaceキー押せば元の状態に戻る
    if (InputJump())
    {
        TransitionJumpState();
    }

    //  移動してないなら待機ステートへの遷移
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    if (IsNearWallTop())
    {
        TransitionClimbTopState();
    }

    else
    {
        CanClimb = false;
    }
}

// 攻撃ステートへ遷移
void Player::TransitionAttackState()
{
    state = State::Attack;

    // 攻撃アニメーション再生
    PlayAttackAnimation();
}

// 攻撃ステート更新処理
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
        //  カメラロック中攻撃すると
        if (lockonState == LockonState::Locked && lockonEnemy != nullptr)
        {
            //  ちょっとだけ敵の方向移動する
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

        //  連撃によって、ノードとエネミーの衝突処理
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

// ダメージステートへ遷移
void Player::TransitionDamageState()
{
    state = State::Damage;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);

    // ダメージ時の振動
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //gamePad.SetVibration(0.5f, 0.5f);

}

// ダメージステート更新処理
void Player::UpdateDamageState(float elapsedTime)
{
    // ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        // 振動停止
        //GamePad& gamePad = Input::Instance().GetGamePad();
        //gamePad.StopVibration();

        TransitionIdleState();
    }
}

// 死亡ステートへ遷移
void Player::TransitionDeathState()
{
    state = State::Death;

    // 死亡アニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

// 死亡ステート更新処理
void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            SceneManager::Instance().ChangeScene(new SceneTitle);
        }
    }
}

// デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    //衝突判定用のデバッグ球を描画
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //衝突判定用のデバッグ円柱を描画
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawSphere(swingPoint, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawSphere(checkpos, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawSphere(checkpos, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

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

// ジャンプ入力処理
bool Player::InputJump()
{
    // ボタン入力でジャンプ
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE || gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //ジャンプ入力した
        Jump(jumpSpeed);
        return true;
    }
    return false;
}

//  着地の時に
void Player::OnLanding()
{
    //下方向の速力が一定以上なら着地ステートへ
    if (velocity.y < gravity * 5.0f && state != State::Death && state != State::Damage)
    {
        //  着地ステートへの遷移
        TransitionLandState();
    }
}

//　着地ステート
void Player::TransitionLandState()
{
    //  スイング同時に着地すると、velocityの影響で床に滑るので
    //この状況を防ぐため velocityを0にする
    velocity.x = 0;
    velocity.y = 0;
    velocity.z = 0;
    //  最初の判定がないと、無限に飛び続けるので
    firstSwing = true;
    state = State::Land;

    model->PlayAnimation(Anim_Landing, false);



}

//  着地ステートの更新処理
void Player::UpdateLandState(float elapsedTime)
{
    onClimb = false;
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

// スイングステート
void Player::TransitionSwingState()
{
    state = State::Swing;

    if (lastState == State::Move)
    {
        if (firstSwing)
        {
            model->PlayAnimation(Anim_StartSwing, false);
            firstSwing = false;
            //  スイングモデル（現在仮、Geometricに変更予定）
            SwingWeb* swingWebRight = new SwingWeb(&projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            //{
            //    //  レンダラーに登録
            //    sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            //}
            if (sceneGame.shadowmapCasterRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }
        else
        {
            //model->PlayAnimation(Anim_Swinging, false);
            //SwingWeb* swingWebLeft = new SwingWeb(&projectileManager, true);
            //SceneGame& sceneGame = SceneGame::Instance();
            //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            //{
            //    //  レンダラーに登録
            //    sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            //}

        }
        HitResult hit;
        FindWallSwingPoint(position, 5, hit);

        onSwing = true;


    }
    else if (lastState == State::Jump)
    {
        //連続スイングのモーション
        if (!onSwing)
        {
            //  スイングモデル（現在仮、Geometricに変更予定）
            SwingWeb* swingWebLeft = new SwingWeb(&projectileManager, true);
            SceneGame& sceneGame = SceneGame::Instance();
            //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            //{
            //    //  レンダラーに登録
            //    sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            //}
            if (sceneGame.shadowmapCasterRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            }

        }
        //　初回スイングのモーション  
        {
            model->PlayAnimation(Anim_Swinging, false);

            //  スイングモデル（現在仮、Geometricに変更予定）
            SwingWeb* swingWebRight = new SwingWeb(&projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            //{
            //    //  レンダラーに登録
            //    sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            //}
            if (sceneGame.shadowmapCasterRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }

        HitResult hit;
        FindWallSwingPoint(position, 5, hit);


        onSwing = true;


    }

}

//  スイングステートの更新処理
void Player::UpdateSwingState(float elapsedTime)
{
    // スイング中壁にぶつかった時の処理
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //レイを設定
        DirectX::XMFLOAT3 start = position;
        DirectX::XMFLOAT3 end = {
            position.x + mx,
            position.y,
            position.z + mz
        };
        float distance = sqrtf(mx * mx + mz * mz);

        //そして判定を精度を上げるため、レイを分割して判定を行う
        int steps = static_cast<int>(distance / 0.5f);
        steps = max(steps, 5);
        HitResult hit;

        const int raySamples = 3;
        float angleStep = DirectX::XM_2PI / raySamples;

        //簡易版のスフィアキャストを作る
        for (int step = 0; step <= steps; ++step) {
            float t = step / static_cast<float>(steps);
            DirectX::XMVECTOR currentPoint = DirectX::XMVectorLerp(XMLoadFloat3(&start), XMLoadFloat3(&end), t);

            for (int i = 0; i < raySamples; ++i) {
                float angle = i * angleStep;
                float offsetX = radius * 0.5f * cosf(angle);
                float offsetZ = radius * 0.5f * sinf(angle);

                DirectX::XMFLOAT3 offsetPoint;
                DirectX::XMStoreFloat3(&offsetPoint, DirectX::XMVectorAdd(currentPoint, DirectX::XMVectorSet(offsetX, 0, offsetZ, 0)));

                //レイを飛ばす
                if (StageManager::Instance().RayCast(position, offsetPoint, hit))
                {
                    DirectX::XMVECTOR hitNormal = XMLoadFloat3(&hit.normal);
                    hitNormal = DirectX::XMVector3Normalize(hitNormal);
                    DirectX::XMVECTOR backOffset = DirectX::XMVectorScale(hitNormal, 0.3f);

                    DirectX::XMVECTOR hitPosition = XMLoadFloat3(&hit.position);
                    DirectX::XMVECTOR adjustedPosition = DirectX::XMVectorAdd(hitPosition, backOffset);

                    DirectX::XMStoreFloat3(&position, adjustedPosition);


                    //当たったらクライミングステートへの変更
                    onClimb = true;
                    TransitionIdleState();
                    velocity.x = 0;
                    velocity.z = 0;
                    velocity.y = 0;
                    return;
                }
            }
        }
        position = end;
    }

    //連続スイングのモーション
    if (!model->IsPlayAnimation())
    {
        SwingWeb* swingWebLeft = new SwingWeb(&projectileManager, true);
        SceneGame& sceneGame = SceneGame::Instance();
        //if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
        //{
        //    //  レンダラーに登録
        //    sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
        //    model->PlayAnimation(Anim_Swinging, true);
        //}
        if (sceneGame.shadowmapCasterRenderer && sceneGame.sceneRenderer)
        {
            //  レンダラーに登録
            sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            model->PlayAnimation(Anim_Swinging, true);
        }
    }


    //スイング位置
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&swingPoint);
    DirectX::XMVECTOR Q = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR displacement = DirectX::XMVectorSubtract(Q, P);
    float currentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(displacement));

    //糸の長さ
    const float ropeLength = 10.0f;
    //糸の向き
    DirectX::XMVECTOR ropeDirection = DirectX::XMVector3Normalize(displacement);

    //仮の重力
    DirectX::XMVECTOR gravity = DirectX::XMVectorSet(0.0f, -10.0f, 0.0f, 0.0f);
    //速力更新
    DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&velocity);
    velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorScale(gravity, elapsedTime));
    //仮の空気阻力系数
    const float dragCoefficient = 0.005f;
    velocityVec = DirectX::XMVectorScale(velocityVec, (1.0f - dragCoefficient));

    //糸の修正
    if (currentLength > ropeLength)
    {
        float correctionFactor = (currentLength - ropeLength) * 0.5f;
        DirectX::XMVECTOR correction = DirectX::XMVectorScale(ropeDirection, -correctionFactor);
        //Q = DirectX::XMVectorAdd(Q, correction);

        DirectX::XMVECTOR targetPosition = DirectX::XMVectorAdd(Q, correction);

        float smoothFactor = 0.1f;
        Q = DirectX::XMVectorLerp(Q, targetPosition, smoothFactor);
    }

    DirectX::XMVECTOR tangentVelocity = DirectX::XMVectorSubtract(
        velocityVec,
        DirectX::XMVectorMultiply(DirectX::XMVector3Dot(velocityVec, ropeDirection), ropeDirection));


    //スイング最高点の判定
    //前方のベクトルの取得
    DirectX::XMFLOAT3 front = GetFront();
    DirectX::XMVECTOR frontVec = DirectX::XMLoadFloat3(&front);
    //もし最高点に着いたら、連続スイングを行う
    float dotProduct = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tangentVelocity, frontVec));
    if (dotProduct <= 0)
    {
        TransitionSwingState();
    }


    //位置更新
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(Q, DirectX::XMVectorScale(tangentVelocity, elapsedTime));
    DirectX::XMStoreFloat3(&position, newPosition);
    DirectX::XMStoreFloat3(&velocity, tangentVelocity);

    //キャラクターの向きを更新
    DirectX::XMVECTOR swingDir = DirectX::XMVector3Normalize(
        DirectX::XMVectorSubtract(XMLoadFloat3(&swingPoint), XMLoadFloat3(&position)));
    DirectX::XMStoreFloat3(&front, swingDir);

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonUp() & GamePad::BTN_SHIFT || gamePad.GetButtonUp() & GamePad::BTN_RIGHT_TRIGGER)
    {
        TransitionIdleState();
        velocity = { 0,0,0 };
        firstSwing = true;
    }
}


//スイングポイントを探す
bool Player::FindWallSwingPoint(const DirectX::XMFLOAT3& start, float maxDistance, HitResult& result) {
    using namespace DirectX;

    DirectX::XMFLOAT3 moveVec = GetMoveVec();


    DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&GetFront());
    DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetUp());
    DirectX::XMVECTOR rightVec = DirectX::XMLoadFloat3(&GetRight());

    forwardVec = DirectX::XMVector3Normalize(forwardVec);
    upVec = DirectX::XMVector3Normalize(upVec);
    rightVec = DirectX::XMVector3Normalize(rightVec);

    forwardVec = DirectX::XMVectorScale(forwardVec, 8.0f);
    upVec = DirectX::XMVectorScale(upVec, 11.0f);
    rightVec = DirectX::XMVectorScale(rightVec, 5.0f);


    SwingwebDirection = DirectX::XMVectorAdd(forwardVec, upVec);
    DirectX::XMVECTOR SwingPoint = DirectX::XMLoadFloat3(&position);
    SwingPoint = DirectX::XMVectorAdd(SwingPoint, SwingwebDirection);
    DirectX::XMStoreFloat3(&swingPoint, SwingPoint);


    previousSwingPoint = swingPoint;




    return false;
}

void Player::TransitionTitleIdleState()
{
    state = State::TitleIdle;
    model->PlayAnimation(Anim_Crouch, false);
}

void Player::UpdateTitleIdleState(float elapsedTime)
{

}

void Player::TransitionCrouchIdleState()
{
    state = State::CrouchIdle;
    model->PlayAnimation(Anim_CrouchIdle, false);

}

void Player::UpdateCrouchIdleState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionTitleIdleState();
    }
}

void Player::TransitionSwingToKick()
{
}

void Player::UpdateSwingToKick(float elapsedTime)
{
}


// デバッグ用GUI描画
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
            ImGui::Text(u8"State　%s", str.c_str());
        }

    }
    ImGui::End();
}

// ダメージを受けたときに呼ばれる
void Player::OnDamaged()
{
    TransitionDamageState();
}

// 死亡したときに呼ばれる
void Player::OnDead()
{
    TransitionDeathState();
}