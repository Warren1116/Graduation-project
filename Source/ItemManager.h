#pragma once

#include <vector>
#include "Item.h"
#include <set>

// �A�C�e���}�l�[�W���[
class ItemManager
{
private:
	ItemManager() {}
	~ItemManager() {}

public:
	// �B��̃C���X�^���X�擾
	static ItemManager& Instance()
	{
		static ItemManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �G�l�~�[�o�^
	void Register(Item* item);

	//// �`�揈��
	//void Render(const RenderContext& rc, ModelShader* shader);

	// �G�l�~�[�S�폜
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �G�l�~�[���擾
	int GetItemCount() const { return static_cast<int>(items.size()); }

	// �G�l�~�[�擾
	Item* GetItem(int index) { return items.at(index); }

	// �G�l�~�[�폜
	void Remove(Item* enemy);

private:
	// �G�l�~�[���m�̏Փˏ���
	void CollisionEnemyVsEnemies();
	std::vector<Item*> items;
	std::set<Item*> removes;
};