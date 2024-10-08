#include "Item.h"
#include "ItemKey.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "ItemManager.h"
#include "Player.h"
#include "Collision.h"
#include "Camera.h"
#include "SceneGame.h"

ItemKey::ItemKey()
{
	model = new Model("Data/Model/Key/Key.mdl");
	cube = new Model("Data/Model/Cube/Cube.mdl");

	radius = 0.8f;

	scale.x = scale.y = 0.01f;
	scale.z = 0.01f;
}

ItemKey::~ItemKey()
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

void ItemKey::Update(float elapsedTime)
{
	if (okDestroy)
	{
		Destroy();
		return;
	}

	
	if (InRange(itemRange)) CollisionItemVsCrossHair();
	else canPickup = false;

	// オブジェクト行列を更新
	UpdateItemTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
	cube->UpdateTransform(cubeTransform);
}


//void ItemKey::Render(const RenderContext& rc, ModelShader* shader)
//{
//	shader->Draw(rc, model);
//}

// デバッグプリミティブ描画
void ItemKey::DrawDebugPrimitive()
{
	Item::DrawDebugPrimitive();
}

void ItemKey::CollisionItemVsCrossHair()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	RenderContext rc;
	//rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	DirectX::XMFLOAT3 screenPosition;
	screenPosition.x = static_cast<float>(graphics.GetScreenWidth() / 2 + 7.0f);
	screenPosition.y = static_cast<float>(graphics.GetScreenHeight() / 2 + 7.0f);
	screenPosition.z = 0.0f;

	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// スクリーン座標からワールド座標へ変換
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
	if (Collision::IntersectRayVsModel(start, end, cube, hit))
	{
		canPickup = true;
		if (gamePad.GetButtonDown() & GamePad::BTN_F)
		{
			Player& player = Player::Instance();
			player.SetHaveKey(true);
			player.key->Play(false);

			// アイテム破棄
			okDestroy = true;
		}
	}
	else canPickup = false;
}