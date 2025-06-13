#include "EnemyManager.h"
#include "Collision.h"
#include <imgui.h>
#include "MetaAI.h"

// �X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (auto& enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	// �G���m�̏Փˏ���
	CollisionEnemyVsEnemies();

	// �j������
	for (Enemy* enemy : removes)
	{
		// std::vector����v�f���폜����ꍇ��
		// �C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		auto it = std::find_if(
			enemies.begin(), enemies.end(),
			[enemy](const std::unique_ptr<Enemy>& p) { return p.get() == enemy; });

		if (it != enemies.end())
		{
			enemies.erase(it);
		}
	}
	// �j�����X�g���N���A
	removes.clear();
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	if (std::find_if(enemies.begin(), enemies.end(),
		[enemy](const std::unique_ptr<Enemy>& p) { return p.get() == enemy; })
		== enemies.end())
	{
		enemy->SetId(identity + static_cast<int>(Meta::Identity::Enemy));
		identity++;
		enemies.emplace_back(std::unique_ptr<Enemy>(enemy));
	}

}

// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
	enemies.clear();
	removes.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

void EnemyManager::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(980, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Enemy", nullptr, ImGuiWindowFlags_None))
	{
		for (auto& enemy : enemies)
		{
			// �G�l�~�[���\��
			enemy->DrawDebugGUI();
		}
	}
	ImGui::End();
}

// �G�l�~�[�擾
Model* EnemyManager::GetEnemyModel(int index)
{
	if (index < 0 || index >= enemies.size())
	{
		return nullptr;
	}

	auto& enemy = enemies.at(index);
	if (enemy == nullptr)
	{
		return nullptr;
	}

	return enemy->GetModel();
}

// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	// �j�����X�g�ɒǉ�
	removes.insert(enemy);
}

// ID����G�l�~�[���擾
Enemy* EnemyManager::GetEnemyFromId(int id)
{
	for (auto& enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy.get();
	}
	return nullptr;
}

// �G�l�~�[���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		auto& enemyA = enemies.at(i);
		{
			for (int j = i + 1; j < enemyCount; ++j)
			{
				auto& enemyB = enemies.at(j);

				DirectX::XMFLOAT3 outPosition;
				if (Collision::IntersectCylinderVsCylinder(
					enemyA->GetPosition(), enemyA->GetRadius(), enemyA->GetHeight(),
					enemyB->GetPosition(), enemyB->GetRadius(), enemyB->GetHeight(),
					outPosition))
				{
					// �����o����̈ʒu�ݒ�
					enemyB->SetPosition(outPosition);
				}
			}
		}
	}
}
