#include "PlayerStateDerived.h"
#include "Player.h"
#include "Input/Input.h"
#include "SwingWeb.h"
#include "SceneGame.h"
#include "scenemanager.h"
#include "SceneLoading.h"

void PlayerStates::IdleState::Enter()
{
    owner->state = Player::State::Idle;
    owner->onSwing = false;
    owner->firstSwing = true;

    if (owner->lastState == Player::State::Swing)
    {
        // 前方に進むためのベクトルを計算
        DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&owner->GetFront());
        DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&owner->GetUp());

        forwardVec = DirectX::XMVector3Normalize(forwardVec);
        upVec = DirectX::XMVector3Normalize(upVec);

        forwardVec = DirectX::XMVectorScale(forwardVec, 5.0f); // 勢いを調整
        upVec = DirectX::XMVectorScale(upVec, 8.0f);

        // 速度に加算
        DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&owner->velocity);
        velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorAdd(forwardVec, upVec));
        DirectX::XMStoreFloat3(&owner->velocity, velocityVec);

    }

    //　クライミング中なら別の待機モーション
    if (owner->onClimb)
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_HoldInWall), true);
    }
    //　普通も待機モーション
    else
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Idle), true);
    }
    if (owner->attacking && owner->attackCount >= owner->attackLimit)
    {
        owner->attackTimer = 0;
        owner->attackCount = 0;
        owner->attacking = false;

    }

}
void PlayerStates::IdleState::Execute(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    owner->lastState = owner->state;
    //移動入力処理
    if (owner->InputMove(elapsedTime))
    {
        owner->SetState(Player::State::Move);
    }

    //ジャンプ入力処理
    if (owner->InputJump())
    {
        owner->SetState(Player::State::Jump);

    }

    if (!owner->onClimb)
    {
        // 回避入力処理
        if (owner->InputDodge())
        {
            owner->SetState(Player::State::Dodge);
        }

        //弾丸入力処理
        if (owner->InputProjectile())
        {
            owner->SetState(Player::State::Shot);
        }

        //攻撃入力処理
        if (owner->InputAttack())
        {
            owner->SetState(Player::State::Attack);
        }
    }

    // 回復入力処理
    if (owner->skillTime > 1.0f)
    {
        owner->InputHealing();
    }

    //  投げ技入力処理
    if (owner->lockonEnemy && owner->skillTime >= 1)
    {
        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_RIGHT || gamePad.GetButtonDown() & GamePad::BTN_Y)
        {
            owner->IsUseGrab = true;
            owner->SetState(Player::State::Grab);
        }

    }

    //  必殺技入力処理
    if (owner->skillTime >= 3)
    {
        if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER || gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_V)
        {
            owner->SetState(Player::State::Ultimate);
        }
    }



}
void PlayerStates::IdleState::Exit()
{
}

void PlayerStates::MoveState::Enter()
{
    if (!owner->attacking)
    {
        owner->attackCount = 0;
        owner->attackTimer = 0;
        owner->attacking = false;
    }

    owner->state = Player::State::Move;
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Running), true);
    }
}

void PlayerStates::MoveState::Execute(float elapsedTime)
{
    if (!owner->canSwing)
    {
        owner->swingCooldownTimer -= elapsedTime;
        if (owner->swingCooldownTimer <= 0.0f)
        {
            owner->canSwing = true;
        }
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    //  移動中SHIFTキー同時に押すと
    if ((gamePad.GetButton() & GamePad::BTN_KEYBOARD_SHIFT || gamePad.GetButton() & GamePad::BTN_RIGHT_TRIGGER) && owner->InputMove(elapsedTime) && owner->firstSwing)
    {
        owner->lastState = owner->state;
        if (owner->canSwing)
        {
            //  スイングステートへの遷移
            owner->canSwing = false;
            owner->swingCooldownTimer = owner->swingCooldown;
            owner->SetState(Player::State::Swing);
        }
    }

    if (owner->InputDodge())
    {
        owner->SetState(Player::State::Dodge);
    }

    //  目の前に壁がある同時にSpeacキー押したら
    if (owner->hitWall && owner->onClimb)
    {
        //クライミングステートへの遷移
        owner->SetState(Player::State::Climb);
    }

    //  移動してないなら
    if (!owner->InputMove(elapsedTime))
    {
        //待機ステートへの遷移
        owner->SetState(Player::State::Idle);
    }

    //　もし目の前に壁がいない、ジャンプ入力処理
    if (!owner->hitWall && owner->InputJump())
    {
        //ジャンブステートへの遷移
        owner->SetState(Player::State::Jump);
    }

    // 弾丸入力処理
    if (owner->InputProjectile())
    {
        owner->SetState(Player::State::Shot);
    }

    // 攻撃入力処理
    if (owner->InputAttack())
    {
        owner->SetState(Player::State::Attack);
    }

    // 回復入力処理
    if (owner->skillTime > 1.0f)
    {
        owner->InputHealing();
    }

    //  必殺技入力処理
    if (owner->skillTime >= 3)
    {
        if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER || gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_V)
        {
            owner->SetState(Player::State::Ultimate);
        }
    }

    //　投げ技入力処理
    if (owner->lockonEnemy && owner->skillTime >= 1)
    {
        Mouse& mouse = Input::Instance().GetMouse();
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (mouse.GetButtonDown() & Mouse::BTN_RIGHT || gamePad.GetButtonDown() & GamePad::BTN_Y)
        {
            owner->IsUseGrab = true;
            owner->SetState(Player::State::Grab);
        }
    }
}

void PlayerStates::MoveState::Exit()
{
}

void PlayerStates::JumpState::Enter()
{
    owner->state = Player::State::Jump;

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Jump), false);
}

void PlayerStates::JumpState::Execute(float elapsedTime)
{
    if (owner->InputAttack())
    {
        owner->SetState(Player::State::SwingToKick);
    }

    GamePad& gamePad = Input::Instance().GetGamePad();

    //  ジャンブ中移動とSHIFTキー同時に押すと
    if ((gamePad.GetButton() & GamePad::BTN_KEYBOARD_SHIFT || gamePad.GetButton() & GamePad::BTN_RIGHT_TRIGGER) && owner->InputMove(elapsedTime))
    {
        owner->lastState = owner->state;
        //  スイングステートへの遷移
        owner->SetState(Player::State::Swing);
    }

    //  もしクライミング中なら、クライミング状態をキャンセルする
    if (owner->onClimb)
    {
        owner->onClimb = false;
    }

    //移動入力処理
    owner->InputMove(elapsedTime);

    if (!owner->model->IsPlayAnimation())
    {
        // 待機ステートへの遷移
        owner->SetState(Player::State::Idle);
    }

    //  弾丸の入力処理
    if (owner->InputProjectile())
    {
        //  発射ステートへの遷移
        owner->SetState(Player::State::Shot);
    }
}

void PlayerStates::JumpState::Exit()
{
}

void PlayerStates::LandState::Enter()
{
    //  スイング同時に着地すると、velocityの影響で床に滑るので
    //この状況を防ぐため velocityを0にする
    owner->velocity.x = 0;
    owner->velocity.y = 0;
    owner->velocity.z = 0;
    //  最初の判定がないと、無限に飛び続けるので
    owner->firstSwing = true;
    owner->state = Player::State::Land;

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Landing), false);
}

void PlayerStates::LandState::Execute(float elapsedTime)
{
    owner->onClimb = false;
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
    }
}

void PlayerStates::LandState::Exit()
{
}

void PlayerStates::AttackState::Enter()
{
    owner->state = Player::State::Attack;
    owner->attacking = true;
    // 攻撃アニメーション再生
    owner->PlayAttackAnimation();
}
void PlayerStates::AttackState::Execute(float elapsedTime)
{
    //　モーションが終わったら待機ステートへ遷移
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
    }
    //　シュート入力処理
    if (owner->InputProjectile())
    {
        owner->SetState(Player::State::Shot);
    }
    //　Dodge入力処理
    if (owner->InputDodge())
    {
        owner->SetState(Player::State::Dodge);
    }

    // アニメーションの再生時間を取得
    float animationTime = owner->model->GetCurrentAnimationSeconds();
    float animationSpeed = owner->model->GetCurrentAnimationSpeed();

    // アニメーションの速度に応じて攻撃の当たり判定を調整
    float adjustedStartTime = 0.3f / animationSpeed;
    float adjustedEndTime = 1.0f / animationSpeed;
    owner->attackCollisionFlag = animationTime >= adjustedStartTime && animationTime <= adjustedEndTime;

    //  攻撃の当たり判定
    if (owner->attackCollisionFlag)
    {
        //  カメラロック中攻撃すると
        if (owner->lockonState == Player::LockonState::Locked && owner->lockonEnemy != nullptr)
        {
            //  ちょっとだけ敵の方向移動する
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&owner->position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&owner->lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));

            float moveDistance = 0.075f;
            DirectX::XMVECTOR moveVector = DirectX::XMVectorScale(directionToEnemy, moveDistance);

            DirectX::XMStoreFloat3(&owner->lockDirection, directionToEnemy);
            owner->Turn(elapsedTime, owner->lockDirection.x, owner->lockDirection.z, owner->turnSpeed);

            if (owner->attackCount <= 2)
            {
                playerPos = DirectX::XMVectorAdd(playerPos, moveVector);
                DirectX::XMStoreFloat3(&owner->position, playerPos);
            }
            else
            {
                float moveDistance = 0.1;
                playerPos = DirectX::XMVectorAdd(playerPos, moveVector);
                DirectX::XMStoreFloat3(&owner->position, playerPos);
            }
        }

        //  連撃によって、ノードとエネミーの衝突処理
        switch (owner->attackCount)
        {
        case 1:
            owner->CollisionNodeVsEnemies("mixamorig:LeftHandMiddle1", owner->attackRadius);
            break;
        case 2:
            owner->CollisionNodeVsEnemies("mixamorig:RightHandMiddle2", owner->attackRadius);
            break;
        case 3:
            owner->CollisionNodeVsEnemies("mixamorig:RightFoot", owner->attackRadius + 0.2f);
            break;

        }

    }
}
void PlayerStates::AttackState::Exit()
{
}

void PlayerStates::ShotState::Enter()
{
    owner->state = Player::State::Shot;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Shoting), false);
}
void PlayerStates::ShotState::Execute(float elapsedTime)
{
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
    }
}
void PlayerStates::ShotState::Exit()
{

}

void PlayerStates::DeathState::Enter()
{
    //  死亡したらHPを0にする
    if (owner->health <= 0)
    {
        owner->health = 0;
    }

    owner->state = Player::State::Death;

    // 死亡アニメーション再生
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Death), false);
}

void PlayerStates::DeathState::Execute(float elapsedTime)
{
    //　モーションが終わったらシーンを切り替える
    if (!owner->model->IsPlayAnimation())
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
}

void PlayerStates::DeathState::Exit()
{
}


void PlayerStates::DodgeState::Enter()
{
    owner->state = Player::State::Dodge;
    float attackAnimationSpeed = 1.2f; // アニメーション速度を速くするための倍率

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Dodge), false, 0.2, attackAnimationSpeed);
    owner->getAttacksoon = false;

}

void PlayerStates::DodgeState::Execute(float elapsedTime)
{
    //無敵時間
    owner->invincibleTimer = 2.0f;

    // ドッジの方向に基づいて移動
    DirectX::XMVECTOR vel = DirectX::XMLoadFloat3(&owner->velocity);
    vel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&owner->dodgeDirection), 1.2f);

    // プレイヤーの位置を更新
    DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&owner->position);
    playerPos = DirectX::XMVectorAdd(playerPos, DirectX::XMVectorScale(vel, elapsedTime));

    DirectX::XMStoreFloat3(&owner->velocity, vel);
    DirectX::XMStoreFloat3(&owner->position, playerPos);

    if (!owner->model->IsPlayAnimation()) owner->SetState(Player::State::Idle);
}

void PlayerStates::DodgeState::Exit()
{
}

void PlayerStates::ClimbState::Enter()
{
    owner->state = Player::State::Climb;
    if (owner->velocity.y >= 0)
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Climb), true);
    }
    else
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_ClimbDown), true);
    }
    owner->onSwing = false;
}

void PlayerStates::ClimbState::Execute(float elapsedTime)
{
    //　クライミング中Spaceキー押せば元の状態に戻る
    if (owner->InputJump())
    {
        owner->SetState(Player::State::Jump);
    }

    //  移動してないなら待機ステートへの遷移
    if (!owner->InputMove(elapsedTime))
    {
        owner->SetState(Player::State::Idle);
    }

    //　壁の上に登れるかどうかの判定
    if (owner->IsNearWallTop())
    {
        owner->SetState(Player::State::ClimbTop);
    }
    else
    {
        owner->CanClimb = false;
    }
}

void PlayerStates::ClimbState::Exit()
{
}

void PlayerStates::DamageState::Enter()
{
    owner->state = Player::State::Damage;
    owner->Damage->Play(false, 0.4f);

    // ダメージアニメーション再生
    float attackAnimationSpeed = 1.5f; // アニメーション速度を速くするための倍率
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_GetHit1), false, 0.2f, attackAnimationSpeed);

    MessageData::CAMERASHAKEDATA	p = { 0.1f, 1.0f };
    Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);

    // ダメージ時コントローラー振動
    GamePad& gamePad = Input::Instance().GetGamePad();
    gamePad.SetVibration(0.5f, 0.5f);
}
void PlayerStates::DamageState::Execute(float elapsedTime)
{
    // ダメージアニメーションが終わったら待機ステートへ遷移
    if (!owner->model->IsPlayAnimation())
    {
        // コントローラー振動停止
        GamePad& gamePad = Input::Instance().GetGamePad();
        gamePad.StopVibration();

        owner->SetState(Player::State::Idle);
    }
}
void PlayerStates::DamageState::Exit()
{
}

void PlayerStates::SwingState::Enter()
{
    if (owner->onClimb) return;
    owner->state = Player::State::Swing;
    //  移動中スイングすると
    if (owner->lastState == Player::State::Move)
    {
        if (owner->firstSwing)
        {
            owner->FirstSwing->Play(false, 0.8f);
            owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_StartSwing), false);
            owner->firstSwing = false;
            //  スイングモデル（現在仮、Geometricに変更予定）
            SwingWeb* swingWebRight = new SwingWeb(&owner->projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }
        else
        {
            owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Swinging), false);
            SwingWeb* swingWebLeft = new SwingWeb(&owner->projectileManager, true);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            }

        }
        //  スイング位置を探す
        owner->FindWallSwingPoint();
        owner->onSwing = true;
    }
    //  ジャンプ中スイングすると
    else if (owner->lastState == Player::State::Jump)
    {
        //連続スイングのモーション
        if (!owner->onSwing)
        {
            owner->FirstSwing->Play(false, 0.8f);
            //  スイングモデル（現在仮、Geometricに変更予定）
            SwingWeb* swingWebLeft = new SwingWeb(&owner->projectileManager, true);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            }

        }
        //　初回スイングのモーション  
        {
            owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Swinging), false);
            //  スイングモデル（現在仮、Geometricに変更予定）
            SwingWeb* swingWebRight = new SwingWeb(&owner->projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  レンダラーに登録
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }
        //  スイング位置を探す
        owner->FindWallSwingPoint();
        owner->onSwing = true;
    }
}

void PlayerStates::SwingState::Execute(float elapsedTime)
{
    //スイングの当たり判定
    owner->SwingCollision(elapsedTime);

    //振り子運動の計算
    owner->HandleSwingPhysics(elapsedTime, 10.0f, 10.0f, 0.005f);

    GamePad& gamePad = Input::Instance().GetGamePad();

    //  キーを押されてない時スイングをキャンセルする
    if (gamePad.GetButtonUp() & GamePad::BTN_KEYBOARD_SHIFT || gamePad.GetButtonUp() & GamePad::BTN_RIGHT_TRIGGER)
    {
        owner->lastState = owner->state;
        owner->SetState(Player::State::Idle);
    }
}

void PlayerStates::SwingState::Exit()
{
}

void PlayerStates::ClimbTopState::Enter()
{
    owner->state = Player::State::ClimbTop;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_ClimbUpWall), false);
}

void PlayerStates::ClimbTopState::Execute(float elapsedTime)
{
    if (!owner->model->IsPlayAnimation()) owner->SetState(Player::State::Idle);
}

void PlayerStates::ClimbTopState::Exit()
{
}

void PlayerStates::GrabState::Enter()
{
    owner->state = Player::State::Grab;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_GrabAndDrop), false);
    owner->webTimer = 0.0f;
    owner->skillTime -= 1.0f;
    owner->getShotsoon = false;
}

void PlayerStates::GrabState::Execute(float elapsedTime)
{
    owner->webTimer += elapsedTime;

    if (!owner->model->IsPlayAnimation())
    {
        owner->shotWebPlayed = false;
        owner->fallSoundPlayed = false;

        owner->lockonEnemy->ApplyDamage(110.0, 1.0);
        owner->IsUseGrab = false;

        if (owner->lockonEnemy)
        {
            // lockonEnemy が Character と等しい場合にクリア
            CharacterManager& manager = CharacterManager::Instance();
            for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
            {
                Character* character = manager.GetCharacter(ii);
                if (character == owner->lockonEnemy)
                {
                    manager.Remove(character);
                    owner->lockonEnemy = nullptr;
                    break;
                }
            }
        }
        owner->SetState(Player::State::Idle);
    }
    //  カメラロック中の処理
    if (owner->lockonState == Player::LockonState::Locked && owner->lockonEnemy != nullptr)
    {
        if (owner->webTimer <= 0.82)
        {
            //プレイヤー向きを更新処理
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&owner->position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&owner->lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));

            float moveDistance = 0.04f;
            DirectX::XMVECTOR moveVector = DirectX::XMVectorScale(directionToEnemy, moveDistance);

            DirectX::XMStoreFloat3(&owner->lockDirection, directionToEnemy);
            owner->Turn(elapsedTime, owner->lockDirection.x, owner->lockDirection.z, owner->turnSpeed);
        }

        if (owner->webTimer > 0.82f)
        {
            //敵の位置を取得
            auto enemyPos = Player::Instance().GetLockonEnemy()->GetPosition();
            float animationProgress = (owner->model->GetCurrentAnimationSeconds() / owner->model->GetCurrentAnimationLength()) * 0.2f;
            enemyPos.y += 1.0f;
            enemyPos.y = enemyPos.y * (1.0f + animationProgress);

            //手の位置を取得
            Model::Node* RightHandPos = owner->model->FindNode("mixamorig:RightHand");
            DirectX::XMFLOAT3 pos;
            pos.x = RightHandPos->worldTransform._41;
            pos.y = RightHandPos->worldTransform._42;
            pos.z = RightHandPos->worldTransform._43;
            //糸を生成
            if (owner->ActiveGrabWeb(pos, enemyPos) && !owner->shotWebPlayed)
            {
                owner->ShotWeb->Play(false, 0.8f);
                owner->shotWebPlayed = true;
            }
        }

        if (owner->webTimer >= 3.8f && !owner->fallSoundPlayed)
        {
            owner->Fall->Play(false);
            owner->fallSoundPlayed = true; // 音声が再生されたことを記録
        }
    }
}

void PlayerStates::GrabState::Exit()
{
}

void PlayerStates::CrouchIdleState::Enter()
{
    owner->state = Player::State::CrouchIdle;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_CrouchIdle), false);
}

void PlayerStates::CrouchIdleState::Execute(float elapsedTime)
{
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::TitleIdle);
    }
}

void PlayerStates::CrouchIdleState::Exit()
{

}

void PlayerStates::TitleIdleState::Enter()
{
    owner->state = Player::State::TitleIdle;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Crouch), false);
}

void PlayerStates::TitleIdleState::Execute(float elapsedTime)
{

}

void PlayerStates::TitleIdleState::Exit()
{
}

void PlayerStates::SwingToKickState::Enter()
{
    owner->state = Player::State::SwingToKick;

    SwingWeb* swingWebRight = new SwingWeb(&owner->projectileManager, false);
    SceneGame& sceneGame = SceneGame::Instance();
    if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
    {
        //  レンダラーに登録
        sceneGame.RegisterRenderModel(swingWebRight->GetModel());
    }

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_SwingToKick), false);
    owner->IsUseSwingKick = true;

    if (owner->lockonEnemy)
    {
        DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&owner->lockonEnemy->GetPosition());
        DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.0f, 13.0f, 0.0f, 0.0f); // 上方に12.0fのオフセット
        DirectX::XMVECTOR swingPointVec = DirectX::XMVectorAdd(enemyPos, upVec);
        DirectX::XMStoreFloat3(&owner->swingPoint, swingPointVec);
    }
    else
    {
        owner->FindWallSwingPoint();
    }
}

void PlayerStates::SwingToKickState::Execute(float elapsedTime)
{
    //スイングの当たり判定
    owner->SwingCollision(elapsedTime);

    //振り子運動の計算
    owner->HandleSwingPhysics(elapsedTime, 12.0f, 20.0f, 0.0001f);

    // アニメーションの再生時間を取得
    float animationTime = owner->model->GetCurrentAnimationSeconds();

    //　右足の攻撃判定
    owner->CollisionNodeVsEnemies("mixamorig:RightToeBase", owner->attackRadius);

    //　モーションが終わったら待機ステートへ遷移
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
        owner->IsUseSwingKick = false;
    }
}

void PlayerStates::SwingToKickState::Exit()
{
}

void PlayerStates::UltimateState::Enter()
{
    owner->state = Player::State::Ultimate;
    owner->skillTime -= 3;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Ultimate), false);
}

void PlayerStates::UltimateState::Execute(float elapsedTime)
{
    static float timeSinceLastShot = 0.0f;
    static int currentDirectionIndex = 0;
    static int shotsPerDirection = 0;
    static std::vector<DirectX::XMFLOAT3> directions;

    // アニメーションが再生中でない場合、待機ステートに遷移
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
        timeSinceLastShot = 0.0f;
        currentDirectionIndex = 0;
        shotsPerDirection = 0;
        directions.clear();
        return;
    }

    // 初回のみランダムな方向を生成
    if (directions.empty())
    {
        for (int i = 0; i < 8; ++i)
        {
            float x, y, z;
            do
            {
                x = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
                y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
                z = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
            } while (x * x + y * y + z * z > 1.0f || y < 0.0f); // 半球状に制限

            float length = sqrtf(x * x + y * y + z * z);
            x /= length;
            y /= length;
            z /= length;

            directions.push_back({ x, y, z });
        }
    }

    // プレイヤーの位置を取得
    DirectX::XMFLOAT3 playerPos = { owner->position.x,owner->position.y + 1.0f,owner->position.z };

    // 発射間隔を設定
    const float shotInterval = 0.2f;

    // 時間を更新
    timeSinceLastShot += elapsedTime;

    // 発射間隔が経過したら弾丸を発射
    if (timeSinceLastShot >= shotInterval)
    {
        timeSinceLastShot = 0.0f;

        // 現在の方向に弾丸を発射
        if (shotsPerDirection < 2)
        {
            owner->ShotWeb->Play(false, 0.8f);
            // 弾丸を生成して発射
            ProjectileStraight* projectile = new ProjectileStraight(&owner->projectileManager);
            projectile->Launch(directions[currentDirectionIndex], playerPos);

            // モデルをレンダラーに登録
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                sceneGame.RegisterRenderModel(projectile->GetModel());
            }

            shotsPerDirection++;
        }
        else
        {
            // 次の方向に移動
            currentDirectionIndex++;
            shotsPerDirection = 0;

            // 全方向に発射し終わったら終了
            if (currentDirectionIndex >= directions.size())
            {
                owner->SetState(Player::State::Idle);
                return;
            }
        }
    }
    // 攻撃範囲の設定
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();

        // プレイヤーと敵の距離を計算
        float dx = enemyPos.x - owner->position.x;
        float dz = enemyPos.z - owner->position.z;
        float distance = sqrtf(dx * dx + dz * dz);

        // 敵が攻撃範囲内にいる場合、ダメージを与える
        if (distance <= owner->ultimateAttackRadius)
        {
            enemy->ApplyDamage(50, 0.5f); // ダメージ量と無敵時間を設定
        }
    }

}

void PlayerStates::UltimateState::Exit()
{
}
