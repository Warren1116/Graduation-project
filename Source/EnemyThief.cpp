#include "EnemyThief.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "EnemyStateDerived.h"
#include "SceneGame.h"
#include "MessageData.h"
#include "Camera.h"
#include "CharacterManager.h"

// コンストラクタ
EnemyThief::EnemyThief()
{
    // モデル読み込み
    model = std::make_unique<Model>("Data/Model/Enemy/Thief.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;

    // モデルの中心位置を設定
    radius = 0.5f;
    height = 1.0f;

    // StateMachineを生成し、階層型ステートマシンに対応するように登録ステートを変更していく。
    stateMachine = new EnemyStateMachine();

    stateMachine->RegisterState(new EnemyStates::SearchState(this));
    stateMachine->RegisterState(new EnemyStates::BattleState(this));

    // ステートマシンにメッセージを受信したときの１層目のステートを追加登録
    stateMachine->RegisterState(new EnemyStates::RecieveState(this));

    stateMachine->RegisterState(new EnemyStates::BodageState(this));

    // 各親ステートにサブステートを登録
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Search), new EnemyStates::WanderState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Search), new EnemyStates::IdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::PursuitState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::AttackState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::PunchState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::ShotState(this));
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::DamageState(this));

    // ステートマシンにメッセージを受信したときのサブステートを追加登録
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Recieve), new EnemyStates::CalledState(this));

    // 戦闘中攻撃権の持っていない状態での待機ステートを追加登録
    stateMachine->RegisterSubState(static_cast<int>(EnemyThief::State::Battle), new EnemyStates::StandbyState(this));

    // ステートをセット
    stateMachine->SetState(static_cast<int>(State::Search));

    GetThrowFlag = false;

}

// デストラクタ
EnemyThief::~EnemyThief()
{
    delete stateMachine;
}

void EnemyThief::Update(float elapsedTime)
{
    if (!isAlive)
    {
        deadTimer++;
        if (deadTimer > deadTimerMax)
        {
            SceneGame::Instance().UnregisterRenderModel(model.get());
            Destroy();
        }
    }

    //　PlayerのGrabに攻撃される時
    if (Player::Instance().GetIsUseGrab() && IsLockedOn)
    {
        GetThrowFlag = true;
    }

    // 投げ技される時の処理
    if (GetThrowFlag)
    {
        webTimer += elapsedTime;

        // 3秒後に投げ技される
        if (webTimer > 3.0f)
        {
            IsLockedOn = false;

            // 速力を初期化
            DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&position);

            // 速力を計算する
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&Player::Instance().GetPosition());
            // プレイヤーの後ろ方向を計算
            DirectX::XMVECTOR back = DirectX::XMLoadFloat3(&Player::Instance().GetFront());
            back = DirectX::XMVector3Normalize(back);
            back = DirectX::XMVectorScale(back, -20.0f); 
            // プレイヤーの上方向を計算
            DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&Player::Instance().GetUp());
            up = DirectX::XMVector3Normalize(up);
            up = DirectX::XMVectorScale(up, 20.0f); 

            DirectX::XMVECTOR target = DirectX::XMVectorAdd(playerPos, DirectX::XMVectorAdd(back, up));

            // 飛ばされる時間
            float flightTime = 1.2f;

            // 重力ベクトル
            DirectX::XMVECTOR g = DirectX::XMVectorSet(0, -9.8f, 0, 0);

            // 投げる方向のベクトルを計算
            DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(target, start);
            DirectX::XMVECTOR halfGT2 = DirectX::XMVectorScale(g, 0.5f * flightTime * flightTime);
            DirectX::XMVECTOR v0 = DirectX::XMVectorDivide(DirectX::XMVectorSubtract(delta, halfGT2), DirectX::XMVectorReplicate(flightTime));

            // 速力を更新
            DirectX::XMStoreFloat3(&velocity, v0);

            GetThrowFlag = false;
        }
    }

    if(GetIsBodage())
    {
        //  縄で縛られている時の処理
        stateMachine->ChangeState(static_cast<int>(State::Bodage));
    }

    //  投げ技されてないならステージマシン更新
    if (!Player::Instance().GetIsUseGrab())
    {
        stateMachine->Update(elapsedTime);
        model->ResumeAnimation();
    }
    else
    {
        model->PauseAnimation();
    }

    // 速力処理更新
    UpdateVelocity(elapsedTime);

    // 無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    // オブジェクト行列更新
    UpdateTransform();

    // アニメーション更新
    model->UpdateAnimation(elapsedTime);

    // モデル行列更新
    model->UpdateTransform(transform);
}

// 死亡した時に呼ばれる
void EnemyThief::OnDead()
{
    isAlive = false;
    CharacterManager::Instance().Remove(this);
    Player::Instance().SetgetShotSoon(false);
    model->PlayAnimation(static_cast<int>(EnemyAnimation::Die), false);

    Meta::Instance().SendMessaging(
        GetId(),
        static_cast<int>(Meta::Identity::Meta),
        MESSAGE_TYPE::MsgChangeAttackRight);
}

void EnemyThief::OnDamaged()
{
    stateMachine->ChangeSubState(static_cast<int>(EnemyThief::Battle::Damage));
}

void EnemyThief::DrawDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

    // ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

    // 索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}

// 縄張り設定
void EnemyThief::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

// ターゲット位置をランダム設定
void EnemyThief::SetRandomTargetPosition()
{
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta) * range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// 目的地点へ移動
void EnemyThief::MoveToTarget(float elapsedTime, float speedRate)
{
    // ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz + vy * vy);
    vx /= dist;
    vz /= dist;
    vy /= dist;

    // 移動処理
    Move(vx, vy, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}


// メッセージ受信処理
bool EnemyThief::OnMessage(const Telegram& telegram)
{
    switch (telegram.msg)
    {
    case MESSAGE_TYPE::MsgCallHelp:
        if (!SearchPlayer())
        {	// プレイヤーを見つけていないときに一層目ステートをReceiveに変更する
            stateMachine->SetState(static_cast<int>(EnemyThief::State::Recieve));
        }
        return true;
        //	メタAIから攻撃権を与えられたとき
    case MESSAGE_TYPE::MsgGiveAttackRight:
        // 攻撃フラグをtrueに設定
        SetAttackFlg(true);
        return true;
    }
    return false;
}

// プレイヤーを探す
bool EnemyThief::SearchPlayer()
{
    // プレイヤーとの高低差を考慮して3Dで距離判定をする
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        // 単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;

        // 方向ベクトル化
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        // 2つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}


// デバッグエネミー情報表示
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

        break;
    case State::Recieve:
        if (stateMachine->GetStateIndex() == static_cast<int>(EnemyThief::Recieve::Called))
        {
            subStr = "Called";
        }
        break;
    case State::Bodage:
        str = "Bodage";
        break;

    }

    //トランスフォーム
    if (ImGui::CollapsingHeader("EnemyThief", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // 位置
        ImGui::InputFloat3("Position", &position.x);
        // 回転
        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        // スケール
        ImGui::InputFloat3("Scale", &scale.x);

        ImGui::Text(u8"State　%s", str.c_str());
        ImGui::Text(u8"SubState 　%s", subStr.c_str());
        ImGui::Checkbox("lockon", &IsLockedOn);
        ImGui::InputFloat("webTimer", &webTimer);
        ImGui::InputInt("Hp", &health);
        ImGui::Checkbox("Alive", &isAlive);

    }


}
