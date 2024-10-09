#include "TutorialZombie.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include "StageManager.h"
#include "StageMain.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/ShadowmapRenderer.h"

// コンストラクタ
TutorialZombie::TutorialZombie()
{
	model = new Model("Data/Model/Zombi/Zombi.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.35f;

	// 徘徊ステートへ遷移
	TransitionWanderState();
}

//　デストラクタ
TutorialZombie::~TutorialZombie()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

// 更新処理
void TutorialZombie::Update(float elapsedTime)
{
	if (!isAlive)
	{
		Destroy();
	}

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
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	case State::Title:
		UpdateTitleState(elapsedTime);
	}

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	// 速度処理更新
	UpdateVelocity(elapsedTime);

	// モデルアニメーション更新
	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

// デバッグプリミティブ描画
void TutorialZombie::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

	// 攻撃判定をデバッグ球描画
	Model::Node* node = model->FindNode("EyeBall");
	float animationTime = model->GetCurrentAnimationSeconds();

	if (node != nullptr && attackCollisionFlag)
	{
		// ノード位置取得
		DirectX::XMFLOAT3 nodePosition;
		nodePosition.x = node->worldTransform._41;
		nodePosition.y = node->worldTransform._42;
		nodePosition.z = node->worldTransform._43;
		debugRenderer->DrawSphere(nodePosition, 0.2f, DirectX::XMFLOAT4(0, 0, 1, 1));
	}

}

void TutorialZombie::TransitionTitleState()
{
	state = State::Title;
	model->PlayAnimation(Anim_IdleNormal, true);
}

void TutorialZombie::UpdateTitleState(float elapsedTime)
{
}

// ターゲット位置をランダム設定
void TutorialZombie::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);

	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = 0.0f;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// 目標地点へ移動
void TutorialZombie::MoveToTarget(float elapsedTime, float speedRate)
{
	//ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// 移動処理
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

// プレイヤー索敵
bool TutorialZombie::SearchPlayer()
{
	// プレイヤーとの高低差を考慮して3Dでの距離判定をする
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
		// 前方ベクトル
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// ２つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// ノードとプレイヤーの衝突処理
void TutorialZombie::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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


		// プレイヤーとのあたり判定
		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;

		if (Collision::IntersectSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{
			// ダメージを与える
			if (player.ApplyDamage(1, 0.5f))
			{
				// 敵を吹っ飛ばすベクトルを算出
				DirectX::XMFLOAT3 vec;
				vec.x = outPosition.x - nodePosition.x;
				vec.z = outPosition.z - nodePosition.z;
				float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
				vec.x /= length;
				vec.z /= length;

				// XZ平面に吹っ飛ばす力をかける
				float power = 10.0f;
				vec.x *= power;
				vec.z *= power;
				// Y方向にも力をかける
				vec.y = 5.0f;

				// 吹っ飛ばす
				player.AddImpulse(vec);
			}
		}
	}
}

// 徘徊ステートへ遷移
void TutorialZombie::TransitionWanderState()
{
	state = State::Wander;

	// 目標地点設定
	SetRandomTargetPosition();

	// モーション再生
	model->PlayAnimation(Anim_RunFWD, true);
}

// 徘徊ステート更新処理
void TutorialZombie::UpdateWanderState(float elapsedTime)
{
	// 目標地点までXZ平面での距離判定
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		TransitionIdleState();
	}

	// 目標地点へ移動
	MoveToTarget(elapsedTime, 0.5f);

	if (SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		TransitionPursuitState();
	}
}

// 待機ステートへ遷移
void TutorialZombie::TransitionIdleState()
{
	state = State::Idle;

	// タイマーをランダム設定
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	// 待機アニメーション再生
	model->PlayAnimation(Anim_IdleNormal, true);

}

// 待機ステート更新処理
void TutorialZombie::UpdateIdleState(float elapsedTime)
{
	// タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		// 徘徊ステートへ遷移
		TransitionPursuitState();
	}
}

// 追跡ステートへ遷移
void TutorialZombie::TransitionPursuitState()
{
	state = State::Pursuit;

	// 走りモーション再生
	model->PlayAnimation(Anim_RunFWD, true);
}

// 追跡ステート更新処理
void TutorialZombie::UpdatePursuitState(float elapsedTime)
{
	float vx = Player::Instance().GetPosition().x - position.x;
	float vy = Player::Instance().GetPosition().y - position.y;
	float vz = Player::Instance().GetPosition().z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	targetPosition = Player::Instance().GetPosition();

	// 目標地点へ移動
	MoveToTarget(elapsedTime, 1.0f);

	// プレイヤーに近づくと攻撃ステートへ遷移
	if (dist < attackRange)
	{
		// 攻撃ステートへ遷移
		TransitionAttackState();
	}
}

// 攻撃ステートへ遷移
void TutorialZombie::TransitionAttackState()
{
	state = State::Attack;

	// 攻撃アニメーション再生
	model->PlayAnimation(Anim_Attack1, false);
}

// 攻撃ステート更新処理
void TutorialZombie::UpdateAttackState(float elapsedTime)
{
	// 任意のアニメーション再生区間のみ衝突判定処理をする
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = animationTime >= 0.1f && animationTime <= 0.35f;
	if (attackCollisionFlag)
	{
		// 目玉ノードとプレイヤーの衝突処理
		CollisionNodeVsPlayer("EyeBall", 0.2f);
	}

	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// 戦闘待機ステートへ遷移
void TutorialZombie::TransitionIdleBattleState()
{
	state = State::IdleBattle;

	// 数秒間待機するタイマーをランダム設定
	stateTimer = Mathf::RandomRange(2.0f, 3.0f);

	// 待機アニメーション再生
	model->PlayAnimation(Anim_IdleNormal, true);
}

// 戦闘待機ステート更新処理
void TutorialZombie::UpdateIdleBattleState(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
	targetPosition = Player::Instance().GetPosition();

	// タイマー設定
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		// プレイヤーが攻撃範囲にいた場合は攻撃ステートへ遷移
		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		if (dist < attackRange)
		{
			// 攻撃ステートへ遷移
			TransitionAttackState();
		}
		else
		{
			// 徘徊ステートへ遷移
			TransitionPursuitState();
		}
	}

	MoveToTarget(elapsedTime, 0.0f);
}

// ダメージステートへ遷移
void TutorialZombie::TransitionDamageState()
{
	state = State::Damage;

	// ダメージアニメーション再生
	model->PlayAnimation(Anim_IdleNormal, false);
}

// ダメージステート更新処理
void TutorialZombie::UpdateDamageState(float elapsedTime)
{
	// ダメージアニメーションが終わったら戦闘待機ステートへ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// 死亡ステートへ遷移
void TutorialZombie::TransitionDeathState()
{
	state = State::Death;

	// 死亡アニメーション再生
	model->PlayAnimation(Anim_Death, false);

}

// 死亡ステート更新処理
void TutorialZombie::UpdateDeathState(float elapsedTime)
{
	// 死亡アニメーションが終わったら自分を破棄
	if (!model->IsPlayAnimation())
	{
		isAlive = false;
	}
}

// 死亡したときに呼ばれる
void TutorialZombie::OnDamaged()
{
	//TransitionDamageState();
}

// 死亡したときに呼ばれる
void TutorialZombie::OnDead()
{
	TransitionDeathState();
}
