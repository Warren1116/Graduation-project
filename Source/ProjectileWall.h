#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// ’¼i’eŠÛ
class ProjectileWall : public Projectile
{
public:
	ProjectileWall(ProjectileManager* manager);
	~ProjectileWall() override;

	// XVˆ—
	void Update(float elapsedTime) override;

	//// •`‰æˆ—
	//void Render(const RenderContext& rc, ModelShader* shader) override;

private:
	float lifeTimer = 3.0f;
	bool one = false;


	std::unique_ptr<AudioSource> nohit = nullptr;

};

