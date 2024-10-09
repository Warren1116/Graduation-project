#pragma once

#include <vector>
#include <set>
#include "Projectile.h"

// �e�ۃ}�l�[�W���[
class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	static ProjectileManager& Instance()
	{
		static ProjectileManager instance;
		return instance;
	}
	// �X�V����
	void Update(float elapsedTime);

	////// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �e�ۓo�^
	void Register(Projectile* projectile);

	// �e�ۑS�폜
	void Clear();

	// �e�ۍ폜
	void Remove(Projectile* projectile);

	// �e�ې��擾
	int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

	// �e�ۏ���
	Projectile* GetProjectile(int index) { return projectiles.at(index); }

private:
	std::vector<Projectile*> projectiles;
	std::set<Projectile*>  removes;
};

