#include <imgui.h>
#include "Player.h"
#include "Gun.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "StageMain.h"
#include "StageManager.h"
#include "ProjectileStraight.h"
#include "SceneGame.h"


static Player* instance = nullptr;

// インスタンス取得
Player& Player::Instance()
{
	return *instance;
}

// コンストラクタ
Player::Player(bool flag)
{
	// インスタンスポイント設定
	instance = this;

	// モデル読み込み
	model = new Model("Data/Model/Spider-man/spider-man.mdl");
	model->GetNodePoses(nodePoses);

	
	// モデルが大きいのでスケーリング
	if (!flag)
	{
		position = { 0, 0, 0 };
	}

	scale.x = scale.y = scale.z = 0.01f;


	outOfBullets = Audio::Instance().LoadAudioSource("Data/Audio/Tamagire.wav");
	walk = Audio::Instance().LoadAudioSource("Data/Audio/walk.wav");
	//key = Audio::Instance().LoadAudioSource("Data/Audio/keypick.wav");


	// 待機ステートへ遷移
	TransitionIdleState();
	
	if (flag)
		goal = new Goal();
}

// デストラクタ
Player::~Player()
{
	delete model;
	delete hitEffect;
	delete blowEffect;
}

// 更新処理
void Player::Update(float elapsedTime)
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
	}


	Mouse& mouse = Input::Instance().GetMouse();

	float ax = 0;
	float ay = 0;

	ax = (mouse.GetPositionX() - mouse.GetOldPositionX());
	ay = (mouse.GetPositionY() - mouse.GetOldPositionY());

	// カメラの回転速度
	float speed = 0.1f * elapsedTime;

	// スティックの入力値に合わせてX軸とY軸を回転
	angle.x += ay * speed;
	angle.y += ax * speed;

	if (angle.x > maxAngleX) angle.x = maxAngleX;
	if (angle.x < mixAngleX) angle.x = mixAngleX;

	int stage = StageMain::GetStageNum();

	// 階段
	switch (stage)
	{
	case 0:
		if (Collision::IntersectPointVsRectXZ(position.x, position.z, 4.5f, 9.3f, 3.0f, 3.0f, 3.0f, 3.0f)) nearStairs = true;
		else nearStairs = false; 
		break;
	case 1:
		if (Collision::IntersectPointVsRectXZ(position.x, position.z, 48.5f, 32.0f, 3.0f, 3.0f, 3.0f, 3.0f)) nearStairs = true;
		else nearStairs = false;
		break;
	}
	
	//弾丸更新処理
	projectileManager.Update(elapsedTime);

	// 速度処理更新
	UpdateVelocity(elapsedTime);

	// 無敵時間更新処理
	UpdateInvincibleTimer(elapsedTime);

	// プレイヤーとエネミーとの衝突処理
	CollisionPlayerVsEnemies();

	// オブジェクト行列を更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);

	if (goal)
	goal->Update(elapsedTime);
}

// 移動入力処理
bool Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//移動処理

	Move(moveVec.x, moveVec.y, moveVec.z, moveSpeed);


	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);


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

	// 移動ベクトルはXZ平面に水平なベクトルになるようにする

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

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (ax * cameraRightX) + (ay * cameraFrontX);
	vec.z = (ax * cameraRightZ) + (ay * cameraFrontZ);
	// Y軸には移動しない
	vec.y = 0.0f;

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

			// 上から踏んづけた場合は小ジャンプする
			if (normal.y > 0.8)
			{
				Jump(jumpSpeed * 0.5f);
				enemy->ApplyDamage(1, 0.0f);
				DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();
				enemyPos.y += enemy->GetHeight();
				if (enemy->GetHealth() != 0) hitEffect->Play(enemyPos);
				else blowEffect->Play({ enemyPos.x, enemyPos.y -= enemy->GetHeight() * 0.5f, enemyPos.z });
			}
			else
			{
				// 押し出し後の位置設定
				enemy->SetPosition(outPosition);
			}
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
				if (enemy->ApplyDamage(1, 0.5f))
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
						enemyPos.y += enemy->GetHeight() * 0.5f;
						if (enemy->GetHealth() != 0) hitEffect->Play(enemyPos);
						else blowEffect->Play(enemyPos);
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

		SceneGame& sceneGame = SceneGame::Instance();

		if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
		{
			sceneGame.shadowmapRenderer->RegisterRenderModel(projectile->GetModel());
			sceneGame.sceneRenderer->RegisterRenderModel(projectile->GetModel());
			const ModelResource* resource = model->GetResource();
			for (const ModelResource::Material& material : resource->GetMaterials())
			{
				ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
				mat.shaderId = static_cast<int>(ModelShaderId::Phong);
			}
		}

		
	}

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
						//hitEffect->Play(e);
					}
					//弾丸破棄
					projectile->Destroy();
				}
			}
		}
	}

}


// 攻撃入力処理
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

// 待機ステートへ遷移
void Player::TransitionIdleState()
{
	if (attacking)
	{
		if (attackCount >= attackLimit || attackTimer > 120)
		{
			attackTimer = 0;
			attackCount = 0;
			attacking = false;
		}
	}
	state = State::Idle;
	model->PlayAnimation(Anim_Idle, true);
}

// 待機ステート更新処理
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



// ダメージステートへ遷移
void Player::TransitionDamageState()
{
	state = State::Damage;

	// ダメージアニメーション再生
	model->PlayAnimation(Anim_GetHit1, false);
}

// ダメージステート更新処理
void Player::UpdateDamageState(float elapsedTime)
{
	// ダメージアニメーションが終わったら待機ステートへ遷移
	if (!model->IsPlayAnimation())
	{
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

// ジャンプ入力処理
bool Player::InputJump()
{
	// ボタン入力でジャンプ（ジャンプ回数制限付き）
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
	{
		if (jumpCount < jumpLimit)
		{
			jumpCount++;
			Jump(jumpSpeed);

			// ジャンプ入力した
			return true;
		}
	}
	return false;
}

void Player::OnLanding()
{
	jumpCount = 0;

	//下方向の速力が一定以上なら着地ステートへ
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
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

void Player::TransitionSwingState()
{
	state = State::Swing;

}

void Player::UpdateSwingState(float elapsedTime)
{
}



// 描画処理
void Player::Render(const RenderContext& rc, ModelShader* shader)
{
	//projectileManager.Render(rc, shader);

	goal->Render(rc, shader);
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