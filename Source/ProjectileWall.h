#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// ���i�e��
class ProjectileWall : public Projectile
{
public:
	ProjectileWall(ProjectileManager* manager);
	~ProjectileWall() override;

	// �X�V����
	void Update(float elapsedTime) override;

	//// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader) override;

private:
	float lifeTimer = 3.0f;
	bool one = false;


	std::unique_ptr<AudioSource> nohit = nullptr;

};

