#include "BrokenProjectileManager.h"

// �R���X�g���N�^
BrokenProjectileManager::BrokenProjectileManager()
{
}

// �f�X�g���N�^
BrokenProjectileManager::~BrokenProjectileManager()
{
	Clear();
}

// �X�V����
void BrokenProjectileManager::Update(float elapsedTime)
{
	// �X�V����
	for (Projectile* projectile : projectiles)
	{
		projectile->Update(elapsedTime);
	}

	// �j������
	// ��projectile�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Projectile* projectile : removes)
	{
		// std::vector����v�f���폜����ꍇ��
		// �C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(),
			projectiles.end(), projectile);

		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}

		// �e�ۂ̔j������
		delete projectile;
	}
	// �j�����X�g���N���A
	removes.clear();
}

//// �`�揈��
//void BrokenProjectileManager::Render(const RenderContext& rc, ModelShader* shader)
//{
//	for (Projectile* projectile : projectiles)
//	{
//		projectile->Render(rc, shader);
//	}
//}

// �f�o�b�O�v���~�e�B�u�`��
void BrokenProjectileManager::DrawDebugPrimitive()
{
	for (Projectile* projectile : projectiles)
	{
		projectile->DrawDebugPrimitive();

	}
}

// �e�ۓo�^
void BrokenProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

// �e�ۑS�폜
void BrokenProjectileManager::Clear()
{
	for (Projectile* projectile : projectiles)
	{
		delete projectile;
	}
	projectiles.clear();
}

// �e�ۍ폜
void  BrokenProjectileManager::Remove(Projectile* projectile)
{
	// �j�����X�g�ɒǉ�
	removes.insert(projectile);
}