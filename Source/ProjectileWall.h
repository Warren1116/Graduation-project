#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// ���i�e��
class ProjectileWall : public Projectile
{
public:
	ProjectileWall(BrokenProjectileManager* manager);
	~ProjectileWall() override;

	//static ProjectileWall& Instance()
	//{
	//	static ProjectileWall instance;
	//	return instance;
	//}

	// �X�V����
	void Update(float elapsedTime) override;

	//// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader) override;

private:
	float lifeTimer = 3.0f;


	std::unique_ptr<AudioSource> nohit = nullptr;

};

