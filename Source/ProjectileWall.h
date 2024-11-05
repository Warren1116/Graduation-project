#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// 壁にぶつかった弾丸（仮）
class ProjectileWall : public Projectile
{
public:
	ProjectileWall(BrokenProjectileManager* manager);
	~ProjectileWall() override;

	// 更新処理
	void Update(float elapsedTime) override;

	//// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader) override;

private:
	float lifeTimer = 3.0f;

};

