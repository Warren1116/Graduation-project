#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"

void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    //衝突判定用のデバッグ球を描画
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //衝突判定用のデバッグ円柱を描画
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    debugRender->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    debugRender->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void Enemy::Destroy()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Remove(this);
    
}
