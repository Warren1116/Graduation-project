#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

// 破棄
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}

// デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder
	(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

//　メッセージ受信したときの処理
bool Enemy::OnMessage(const Telegram& msg)
{
	return false;
}
