#pragma once

#include <vector>
#include "Graphics/Shader.h"
#include "Character.h"

// �L�����N�^�[�}�l�[�W���[
class CharacterManager
{
private:
	CharacterManager() {}
	~CharacterManager() {}

public:
	// �B��̃C���X�^���X�擾
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �L�����N�^�[�o�^
	void Register(Character* enemy);

	// �L�����N�^�[�S�폜
	void Clear();

	// �L�����N�^�[�폜
	void Remove(Character* enemy);

	// �L�����N�^�[���擾
	int GetCharacterCount() const { return static_cast<int>(characteres.size()); }

	// �L�����N�^�[�擾
	Character* GetCharacter(int index) { return characteres.at(index); }

private:
	// �L�����N�^�[���m�̏Փˏ���
	void CollisionCharacterVsCharacter();

private:
	std::vector<Character*>	characteres;
	std::vector<Character*>	removes;
};
