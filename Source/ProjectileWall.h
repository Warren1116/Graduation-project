#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// �ǂɂԂ������e�ہi���j
class ProjectileWall : public Projectile
{
public:
	ProjectileWall(BrokenProjectileManager* manager);
	~ProjectileWall() override;

	// �X�V����
	void Update(float elapsedTime) override;

	//// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader) override;

private:
	float lifeTimer = 3.0f;

};

