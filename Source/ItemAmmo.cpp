#include "ItemAmmo.h"
#include "ItemManager.h"
#include "Player.h"
#include "Input/Input.h"
#include "Graphics/Graphics.h"
#include "Camera.h"
#include "Collision.h"

ItemAmmo::ItemAmmo()
{
	model = new Model("Data/Model/Gun/tama.mdl");
	cube = new Model("Data/Model/Cube/Cube.mdl");

	radius = 0.8f;
	
	scale.x = scale.y = 0.03f;
	scale.z = 0.03f;
}

ItemAmmo::~ItemAmmo()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
	if (cube != nullptr)
	{
		delete cube;
		cube = nullptr;
	}
}

void ItemAmmo::Update(float elapsedTime)
{
	if (okDestroy)
	{
		Destroy();
		return;
	}


	if (InRange(itemRange)) CollisionItemVsCrossHair();
	else canPickup = false;

	// �I�u�W�F�N�g�s����X�V
	UpdateItemTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
	cube->UpdateTransform(cubeTransform);
}

void ItemAmmo::DrawDebugPrimitive()
{
	Item::DrawDebugPrimitive();
}

void ItemAmmo::CollisionItemVsCrossHair()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	RenderContext rc;
	//rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// �r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	DirectX::XMFLOAT3 screenPosition;
	screenPosition.x = static_cast<float>(graphics.GetScreenWidth() / 2 + 7.0f);
	screenPosition.y = static_cast<float>(graphics.GetScreenHeight() / 2 + 7.0f);
	screenPosition.z = 0.0f;

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// �X�N���[�����W���烏�[���h���W�֕ϊ�
	DirectX::XMVECTOR StartPosition = DirectX::XMVector3Unproject(
		DirectX::XMLoadFloat3(&screenPosition),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0.0f,
		1.0f,
		Projection,
		View,
		World);

	screenPosition.z = 1.0f;

	DirectX::XMVECTOR EndPosition = DirectX::XMVector3Unproject(
		DirectX::XMLoadFloat3(&screenPosition),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0.0f,
		1.0f,
		Projection,
		View,
		World);

	DirectX::XMFLOAT3 start;
	DirectX::XMStoreFloat3(&start, StartPosition);

	DirectX::XMFLOAT3 end;
	DirectX::XMStoreFloat3(&end, EndPosition);

	HitResult hit;
	if (Collision::IntersectRayVsModel(start, end, model, hit))
	{
		canPickup = true;
		if (gamePad.GetButtonDown() & GamePad::BTN_F)
		{
			Player& player = Player::Instance();
			player.SetHaveAmmoNum(player.GetHaveAmmoNum() + 7);
			//player.ammo->Play(false);

			// �A�C�e���j��
			okDestroy = true;
		}
	}
	else canPickup = false;
}