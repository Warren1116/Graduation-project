#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "CameraController.h"
#include "Audio/Audio.h"
#include "Effect.h"

#define AMMO_MAX	13

// 銃
class Gun
{
public:
	Gun();
	~Gun();

	// 更新処理
	void Update(float elapsedTime);

	// 弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();

	// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader);

	// デバッグ用GUI描画
	void DrawDebugGUI();

	Model* GetModel() { return model; }
	// インスタンス取得
	static Gun& Instance();

	// 回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	const int GetHaveAmmoNum() const { return haveAmmoNum; }
	const int GetReloadTimer() const { return reloadTimer; }
	const bool IsReload() const { return isReload; }

	void Reload();

	bool IsLaunch() const { return isLaunch; }

	DirectX::XMFLOAT4X4 GetTransform() const { return transform; }

private:
	// 弾丸入力処理
	void InputProjectile();

	// ノードとエネミーの衝突処理
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	void UpdateTransform();



private:
	// アニメーション
	enum Animation
	{

	};
	enum class State
	{

	};
private:
	Model* model = nullptr;
	std::unique_ptr<Effect> fireEffect = nullptr;
	std::unique_ptr<AudioSource> reload = nullptr;
	std::unique_ptr<AudioSource> slide = nullptr;
	std::unique_ptr<AudioSource> fire = nullptr;
	std::unique_ptr<AudioSource> hit = nullptr;
	CameraController* cameraController = nullptr;
	ProjectileManager projectileManager;
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, DirectX::XMConvertToRadians(-90), 0 };
	DirectX::XMFLOAT3 scale = { 0.02f, 0.02f, 0.02f };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	int reloadTimer = 0;
	bool isFire = false;
	bool isReload = false;
	bool setReloadTime = false;
	bool isLaunch = false;
	int haveAmmoNum = AMMO_MAX;
};

