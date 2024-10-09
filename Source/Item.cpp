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

// �j��
void Item::Destroy()
{
	ItemManager::Instance().Remove(this);
}


// �f�o�b�O�v���~�e�B�u�`��
void Item::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//// �Փ˔���p�̃f�o�b�O����`��
	debugRenderer->DrawSphere
	(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// �Փ˔���p�̃f�o�b�O�~����`��
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
