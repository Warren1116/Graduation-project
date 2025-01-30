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

	// 発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
	

private:
	//基本情報
	float speed = 10.0f;
	float lifeTimer = 3.0f;
	bool one = false;
	static ProjectileStraight* instance;

	std::unique_ptr<Effect> destroyEffect = nullptr;


};

