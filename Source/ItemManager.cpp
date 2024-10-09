#include "ItemManager.h"
#include "Collision.h"

// �X�V����
void ItemManager::Update(float elapsedTime)
{
	for (Item* enemy : items)
	{
		enemy->Update(elapsedTime);
	}

	// �j������
	for (Item* item : removes)
	{
		// std::vector����v�f���폜����ꍇ��
		// �C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Item*>::iterator it = std::find(items.begin(),
			items.end(), item);

		if (it != items.end())
		{
			items.erase(it);
		}

		// �G�l�~�[�̔j������
		delete item;
	}
	// �j�����X�g���N���A
	removes.clear();
}

// �G�l�~�[�o�^
void ItemManager::Register(Item* item)
{
	items.emplace_back(item);
}

//// �`�揈��
//void ItemManager::Render(const RenderContext& rc, ModelShader* shader)
//{
//	for (Item* item : items)
//	{
//		item->Render(rc, shader);
//	}
//}

void ItemManager::Clear()
{
	for (Item* item : items)
	{
		delete item;
	}
	items.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void ItemManager::DrawDebugPrimitive()
{
	for (Item* item : items)
	{
		item->DrawDebugPrimitive();
	}
}

// �G�l�~�[�폜
void ItemManager::Remove(Item* item)
{
	// �j�����X�g�ɒǉ�
	removes.insert(item);
}
