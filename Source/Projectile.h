#pragma once

#include "Graphics/Shader.h"
#include "Effect.h"

// 前方宣言
class ProjectileManager;
class BrokenProjectileManager;

// 弾丸
class Projectile
{
public:
	Projectile(ProjectileManager* manager);
	Projectile(BrokenProjectileManager* broken_manager);
	virtual ~Projectile() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	void Destroy();

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// 位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 方向取得
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }
	
	// スケール取得
	const DirectX::XMFLOAT3& GetScale()  const { return scale; }

	Model* GetModel() const { return model.get(); }

	// 半径取得
	float GetRadius() const { return radius; }
	
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	void SetDirection(const DirectX::XMFLOAT3& direction) { this->direction = direction; }

protected:
	// 行列更新処理
	void UpdateTransform();

protected:
	std::unique_ptr<Model> model;
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 direction = { 0, 0, 1 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform =
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	float radius = 0.166f;
	ProjectileManager* manager = nullptr;
	BrokenProjectileManager* broken_manager = nullptr;
};

