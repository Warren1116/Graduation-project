#include "CharacterManager.h"
#include "Collision.h"

// �X�V����
void CharacterManager::Update(float elapsedTime)
{
    for (Character* character : characters)
    {
        if (character != nullptr && !character->GetAlive())
        {
            removes.emplace_back(character);

        }
    }

    // �j������
    // ��enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    // �@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
    for (Character* character : removes)
    {
        // std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Character*>::iterator it = std::find(characters.begin(), characters.end(), character);
        if (it != characters.end())
        {
            characters.erase(it);
        }

    }

    // �j�����X�g���N���A
    removes.clear();

    // �L�����N�^�[���m�̏Փˏ���
    CollisionCharacterVsCharacter();
}

// �f�o�b�O�v���~�e�B�u�`��
void CharacterManager::DrawDebugPrimitive()
{
    for (Character* character : characters)
    {
        character->DrawDebugPrimitive();
    }
}

// �f�o�b�O�pGUI�`��
void CharacterManager::DrawDebugGUI()
{
    for (Character* character : characters)
    {
        character->DrawDebugGUI();
    }
}

// �L�����N�^�[�o�^
void CharacterManager::Register(Character* character)
{
    characters.emplace_back(character);
}

// �L�����N�^�[�S�폜
void CharacterManager::Clear()
{
    characters.clear();
    removes.clear();
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


}

// �L�����N�^�[���m�̏Փˏ���
void CharacterManager::CollisionCharacterVsCharacter()
{
    size_t count = characters.size();
    for (int i = 0; i < count; ++i)
    {
        Character* characterA = characters.at(i);
        for (int j = i + 1; j < count; ++j)
        {
            Character* characterB = characters.at(j);

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
