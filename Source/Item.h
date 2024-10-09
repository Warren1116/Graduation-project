#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Audio/Audio.h"

// 前方宣言
class ItemManager;

// エネミー
class Item : public Character
{
public:
	Item();
	~Item() override;

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 破棄
	void Destroy();

	// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader);

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	Model* GetModel() const { return model; }
	bool OkDestroy() const { return okDestroy; }

	bool CanPickup() const { return canPickup; }

protected:
	
	virtual void CollisionItemVsCrossHair() = 0;

	bool InRange(float range);

	// 行列更新処理
	void UpdateItemTransform()
	{
		// スケール行列を作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX CS = DirectX::XMMatrixScaling(cubeScale.x, cubeScale.y, cubeScale.z);
		// 回転行列を作成
		DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
		DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
		DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

		DirectX::XMMATRIX R = Y * X * Z;;

		// 位置行列を作成
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		// ３つの行列を組み合わせ、ワールド行列を作成
		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMMATRIX CW = CS * R * T;
		// 計算したワールド行列を取り出す
		DirectX::XMStoreFloat4x4(&transform, W);
		DirectX::XMStoreFloat4x4(&cubeTransform, CW);
	}


protected:
	ItemManager* manager = nullptr;
	Model* model = nullptr;
	Model* cube = nullptr;
	DirectX::XMFLOAT3 cubeScale = { 1.0f, 1.5f, 0.5f };
	DirectX::XMFLOAT4X4 cubeTransform = transform;
	float itemRange = 5.0f;
	bool okDestroy = false;
	bool canPickup = false;
};

