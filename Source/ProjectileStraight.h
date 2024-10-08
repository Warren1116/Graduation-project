#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// ’¼i’eŠÛ
class ProjectileStraight : public Projectile
{
public:
	ProjectileStraight(ProjectileManager* manager);
	~ProjectileStraight() override;

	// XVˆ—
	void Update(float elapsedTime) override;

	//// •`‰æˆ—
	//void Render(const RenderContext& rc, ModelShader* shader) override;

	// ”­Ë
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
	float speed = 60.0f;
	float lifeTimer = 30.0f;
	bool one = false;

	std::unique_ptr<AudioSource> nohit = nullptr;
};

