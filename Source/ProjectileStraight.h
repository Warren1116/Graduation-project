#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"
#include "ProjectileWall.h"
#include "BrokenProjectileManager.h"

// 直進弾丸
class ProjectileStraight : public Projectile
{
public:
	ProjectileStraight(ProjectileManager* manager);
	~ProjectileStraight() override;

	static ProjectileStraight& Instance() { return *instance; }

	// 更新処理
	void Update(float elapsedTime) override;

	//// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader) override;

	// 発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
	

private:

	float speed = 10.0f;
	float lifeTimer = 3.0f;
	bool one = false;
	static ProjectileStraight* instance;


	//　サウンド
	std::unique_ptr<AudioSource> stickWall = nullptr;

};

