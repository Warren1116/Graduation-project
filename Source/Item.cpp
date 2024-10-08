#include "Item.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "ItemManager.h"
#include "Player.h"
#include "Collision.h"
#include "Camera.h"
#include "SceneGame.h"

Item::Item()
{
}

Item::~Item()
{
}

//void Item::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}

// 破棄
void Item::Destroy()
{
	ItemManager::Instance().Remove(this);
}


// デバッグプリミティブ描画
void Item::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//// 衝突判定用のデバッグ球を描画
	debugRenderer->DrawSphere
	(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder
	(position, itemRange, height, DirectX::XMFLOAT4(1, 0, 0, 1));
}

bool Item::InRange(float range)
{
	Player& player = Player::Instance();

	DirectX::XMVECTOR P1 = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR P2 = DirectX::XMLoadFloat3(&player.GetPosition());
	DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P1, P2);
	DirectX::XMVECTOR L = DirectX::XMVector3Length(V);

	float length = DirectX::XMVectorGetX(L);

	if (length < range) return true;

	return false;
}
