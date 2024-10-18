#include "EnemyPeople.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "StateDerived.h"
#include "SceneGame.h"


#define USESTATEMACHINE3

// コンストラクタ
EnemyPeople::EnemyPeople()
{
	model = std::make_unique<Model>("Data/Model/Slime/Slime.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.5f;
	height = 1.0f;

	// StateMachineを生成し、階層型ステートマシンに対応するように登録ステートを変更していく。
	stateMachine = new StateMachine();

	stateMachine->RegisterState(new SearchState(this));
	stateMachine->RegisterState(new BattleState(this));

	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new WanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new IdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new PursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new AttackState(this));

	// ステートマシンにメッセージを受信したときの一層目のステートを追加登録
	stateMachine->RegisterState(new RecieveState(this));

	// ステートマシンにメッセージを受信したときのサブステートを追加登録
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Recieve), new CalledState(this));

	// 戦闘中攻撃権の持っていない状態での待機ステートを追加登録
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new StandbyState(this));

	stateMachine->SetState(static_cast<int>(State::Search));

	// ステートマシンにメッセージを受信したときの１層目のステートを追加登録
	stateMachine->RegisterState(new RecieveState(this));

	// 各親ステートにサブステートを登録(WanderState以外の2層目のステートも同様の方法で各自追加してください。)
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new IdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Search), new WanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new PursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new AttackState(this));

	// ステートマシンにメッセージを受信したときのサブステートを追加登録
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Recieve), new CalledState(this));

	// 戦闘中攻撃権の持っていない状態での待機ステートを追加登録
	stateMachine->RegisterSubState(static_cast<int>(EnemyPeople::State::Battle), new StandbyState(this));

	// ステートをセット
	stateMachine->SetState(static_cast<int>(State::Search));

}

// デストラクタ
EnemyPeople::~EnemyPeople()
{
	delete stateMachine;
}

void EnemyPeople::Update(float elapsedTime)
{
	stateMachine->Update(elapsedTime);

	// 速力処理更新
	UpdateVelocity(elapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

// 死亡した時に呼ばれる
void EnemyPeople::OnDead()
{
	// 死亡
	Destroy();
	SceneGame::Instance().UnregisterRenderModel(model.get());
}


void EnemyPeople::DrawDebugPrimitive()
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
void EnemyPeople::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}
// ターゲット位置をランダム設定
void EnemyPeople::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// 目的地点へ移動
void EnemyPeople::MoveToTarget(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// 移動処理
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemyPeople::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MsgCallHelp:
		if (!SearchPlayer())
		{	// プレイヤーを見つけていないときに一層目ステートをReceiveに変更する
			stateMachine->ChangeState(static_cast<int>(EnemyPeople::State::Recieve));
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


bool EnemyPeople::SearchPlayer()
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
void EnemyPeople::DrawDebugGUI()
{

	std::string str = "";
	std::string subStr = "";

	switch (static_cast<State>(stateMachine->GetStateIndex()))
	{
	case State::Search:
		str = "Search";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Search::Wander))
		{
			subStr = "Wander";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Search::Idle))
		{
			subStr = "Idle";
		}
		break;
	case State::Battle:
		str = "Battle";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Battle::Pursuit))
		{
			subStr = "Pursuit";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Battle::Attack))
		{
			subStr = "Attack";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyPeople::Battle::Standby))
		{
			subStr = "Standby";
		}
		break;
	case State::Recieve:
		if (stateMachine->GetStateIndex() == static_cast<int>(EnemyPeople::Recieve::Called))
		{
			subStr = "Called";
		}
		break;

	}


	//トランスフォーム
	if (ImGui::CollapsingHeader("EnemyPeople", ImGuiTreeNodeFlags_DefaultOpen))
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
	}
}
