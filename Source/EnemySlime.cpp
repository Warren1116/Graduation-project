#include "EnemySlime.h"
#include "Graphics\Graphics.h"
#include "Player.h"
#include "Mathf.h"
#include "Collision.h"

EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
    radius = 0.5f;
    height = 1.0f;

    TransitionWanderState();
}

EnemySlime::~EnemySlime()
{
    delete model;
}

void EnemySlime::Update(float elapsedTime)
{
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;    
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;

    }

    //速力処理更新
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

bool EnemySlime::SearchPlayer()
{
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;

    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        //単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;
        //前方ベクトル
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //２つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;

    //数秒間追跡するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //歩きアニメーション再生
    model->PlayAnimation(Anim_RunFWD, true);
}

void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    //目標地点をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //目標地点へ移動
    MoveToTarget(elapsedTime, 1.0f);

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;

    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        TransitionAttackState();
    }

}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 0.35f)
    {
        //目玉ノードとプレイヤーの衝突処理
        CollisionNodeVsPlayer("EyeBall",0.2f);
    }
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        //ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //当たり判定表示
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1));

        //プレイヤーと当たり判定
        Player& player = Player::Instance();
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player.GetPosition(),
            player.GetRadius(),
            player.GetHealth(),
            outPosition))
        {
            //ダメージを与える
            if (player.ApplyDamage(1, 1.5f))
            {
                //敵を吹っ飛ばす力ベクトルを算出
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;

                //XZ平面に吹っ飛ばす力をかける
                float power = 10.0f;
                vec.x *= power;
                vec.z *= power;

                //Y方向にも力をかける
                vec.y = 5.0f;

                //吹っ飛ばす
                player.AddImpulse(vec);

            }
        }

    }
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //数秒間待機するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);
    //戦闘待機アニメーション再生
    model->PlayAnimation(Anim_IdleBattle, true);

}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    //目標地点をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //プレイヤーが攻撃範囲にいた場合は攻撃ステートへ遷移
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;

        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            TransitionAttackState();
        }
        else
        {
            TransitionWanderState();
        }

    }
    MoveToTarget(elapsedTime, 0.0f);
}

void EnemySlime::TransitionDamageState()
{
    state = State::Damage;
    model->PlayAnimation(Anim_GetHit, false);
}

void EnemySlime::UpdateDamageState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::TransitionDeathState()
{
    state = State::Death;
    model->PlayAnimation(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}

void EnemySlime::SetRandomTargetPosition()
{   
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f,territoryRange);

    targetPosition.x = territoryOrigin.x + sinf(theta) * range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;

    //targetPosition.x = Mathf::RandomRange(position.x - territoryRange,position.x+ territoryRange);
    //targetPosition.y = position.y;
    //targetPosition.z = Mathf::RandomRange(position.z - territoryRange, position.z + territoryRange);
}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    SetRandomTargetPosition();

    model->PlayAnimation(Anim_WalkFWD, true);
}

void EnemySlime::UpdateWanderState(float elapsedTime)
{
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //次の目標地点設定
        //SetRandomTargetPosition();

        //待機ステートへ遷移
        TransitionIdleState();

    }
    MoveToTarget(elapsedTime, 0.5f);
    if (SearchPlayer())
    {
        TransitionPursuitState();
    }
}

void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    //タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //待機アニメーション再生
    model->PlayAnimation(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
{
    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        TransitionWanderState();
    }
    //プレイヤー索敵
    if (SearchPlayer())
    {
        //見つかったら追跡ステートへ遷移
        TransitionPursuitState();
    }
}

void EnemySlime::OnDead()
{
    //Destroy();
    TransitionDeathState();
}

void EnemySlime::OnDamaged()
{
    TransitionDamageState();
}
