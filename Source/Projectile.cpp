#include "Projectile.h"
#include "ProjectileManager.h"
#include "BrokenProjectileManager.h"
#include "Player.h"
#include "Graphics/Graphics.h"
#include "SceneGame.h"

// コンストラクタ
Projectile::Projectile(ProjectileManager* manager)
	: manager(manager)
{
	manager->Register(this);
}

Projectile::Projectile(BrokenProjectileManager* broken_manager)
	: broken_manager(broken_manager)
{
	broken_manager->Register(this);
}

// デバッグプリミティブ描画
void Projectile::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debugRenderer->DrawSphere
	(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// 行列更新処理
void Projectile::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// 前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	// 仮の上ベクトルを算出
	Up = DirectX::XMVectorSet(0.001f, 1.0f, 0.0f, 0.0f);
	Up = DirectX::XMVector3Normalize(Up);

	// 右ベクトルを算出
	Right = DirectX::XMVector3Cross(Up, Front);
	Right = DirectX::XMVector3Normalize(Right);

	// 上ベクトルを算出
	Up = DirectX::XMVector3Cross(Front, Right);

	// 計算結果を取り出し
	DirectX::XMFLOAT3 front, up, right;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	transform._11 = right.x * scale.x;
	transform._12 = right.y * scale.x;
	transform._13 = right.z * scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * scale.y;
	transform._22 = up.y * scale.y;
	transform._23 = up.z * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;
	transform._32 = front.y * scale.z;
	transform._33 = front.z * scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	this->direction = front;
}

// 破棄
void Projectile::Destroy()
{
	if (manager != nullptr)
	{
		manager->Remove(this);
	}
	if (broken_manager != nullptr)
	{
		broken_manager->Remove(this);
	}
}