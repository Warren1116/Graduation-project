#include "CharacterManager.h"
#include "Collision.h"

// �X�V����
void CharacterManager::Update(float elapsedTime)
{
	for (Character* character : characteres)
	{
		if (character != nullptr)
		{
			if (!character->GetAlive())
			{
				removes.emplace_back(character);
			}

		}
	}

	// �j������
	// ��enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Character* character : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Character*>::iterator it = std::find(characteres.begin(), characteres.end(), character);
		if (it != characteres.end())
		{
			characteres.erase(it);
		}

		//// �폜
		//delete character;
	}
	// �j�����X�g���N���A
	removes.clear();

	// �L�����N�^�[���m�̏Փˏ���
	CollisionCharacterVsCharacter();
}

// �f�o�b�O�v���~�e�B�u�`��
void CharacterManager::DrawDebugPrimitive()
{
	for (Character* character : characteres)
	{
		character->DrawDebugPrimitive();
	}
}

// �f�o�b�O�pGUI�`��
void CharacterManager::DrawDebugGUI()
{
	for (Character* character : characteres)
	{
		character->DrawDebugGUI();
	}
}

// �L�����N�^�[�o�^
void CharacterManager::Register(Character* character)
{
	characteres.emplace_back(character);
}

// �L�����N�^�[�S�폜
void CharacterManager::Clear()
{
	for (Character* character : characteres)
	{
		delete character;
	}
	characteres.clear();
}

// �L�����N�^�[�폜
void CharacterManager::Remove(Character* character)
{
	// �j�����X�g�ɂ��łɂ���Βe��
	if (std::find(removes.begin(), removes.end(), character) == removes.end())
	{
		// �j�����X�g�ɒǉ�
		removes.emplace_back(character);
	}

	//for (auto& it : removes)
	//{
	//	if (it == character)
	//		break;
	//}
	//// �j�����X�g�ɒǉ�
	//removes.emplace_back(character);
}

// �L�����N�^�[���m�̏Փˏ���
void CharacterManager::CollisionCharacterVsCharacter()
{
	size_t count = characteres.size();
	for (int i = 0; i < count; ++i)
	{
		Character* characterA = characteres.at(i);
		for (int j = i + 1; j < count; ++j)
		{
			Character* characterB = characteres.at(j);

			DirectX::XMFLOAT3 outPositionA, outPositionB;
			if (Collision::IntersectSphereVsSphere(
				characterA->GetPosition(),
				characterA->GetRadius(),
				characterB->GetPosition(),
				characterB->GetRadius(),
				outPositionA,
				outPositionB))
			{
				characterA->SetPosition(outPositionA);
				characterB->SetPosition(outPositionB);
			}
		}
	}
}
