#include "EnemyManager.h"
#include "Collision.h"
#include<imgui.h>
#include "MetaAI.h"

// �X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	for (Enemy* enemy : enemies)
	{
		// �G���m�̏Փˏ���
		CollisionEnemyVsEnemies();
	}

	// �j������
	for (Enemy* enemy : removes)
	{
		// std::vector����v�f���폜����ꍇ��
		// �C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(),
			enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// �G�l�~�[�̔j������
		delete enemy;
	}
	// �j�����X�g���N���A
	removes.clear();
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	enemy->SetId(identity + static_cast<int>(Meta::Identity::Enemy));
	identity++;
	enemies.emplace_back(enemy);
}

void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies)
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
		for (Enemy* enemy : enemies) {
			// �G�l�~�[���\��
			enemy->DrawDebugGUI();
		}
	}
	ImGui::End();
}


Model* EnemyManager::GetEnemyModel(int index)
{
	if (index < 0 || index >= enemies.size())
	{
		return nullptr;
	}

	Enemy* enemy = enemies.at(index);
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

Enemy* EnemyManager::GetEnemyFromId(int id)
{
	for (Enemy* enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy;
	}
	return nullptr;
}

// �G�l�~�[���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		{
			for (int j = i + 1; j < enemyCount; ++j)
			{
				Enemy* enemyB = enemies.at(j);

				DirectX::XMFLOAT3 outPosition;
				//if (Collision::IntersectSphereVsSphere(
				//	enemyA->GetPosition(), enemyA->GetRadius(),
				//	enemyB->GetPosition(), enemyB->GetRadius(),
				//	outPosition))
				//{
				//	// �����o����̈ʒu�ݒ�
				//	enemyB->SetPosition(outPosition);
				//}

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
