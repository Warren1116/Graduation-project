#include "EnemyManager.h"
#include "Collision.h"
#include<imgui.h>
#include "MetaAI.h"

// 更新処理
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	for (Enemy* enemy : enemies)
	{
		// 敵同士の衝突処理
		CollisionEnemyVsEnemies();
	}

	// 破棄処理
	for (Enemy* enemy : removes)
	{
		// std::vectorから要素を削除する場合は
		// イテレーターで削除しなければならない
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(),
			enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// エネミーの破棄処理
		delete enemy;
	}
	// 破棄リストをクリア
	removes.clear();
}

// エネミー登録
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

// デバッグプリミティブ描画
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
			// エネミー情報表示
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

// エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	// 破棄リストに追加
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

// エネミー同士の衝突処理
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
				//	// 押し出し後の位置設定
				//	enemyB->SetPosition(outPosition);
				//}

				if (Collision::IntersectCylinderVsCylinder(
					enemyA->GetPosition(), enemyA->GetRadius(), enemyA->GetHeight(),
					enemyB->GetPosition(), enemyB->GetRadius(), enemyB->GetHeight(),
					outPosition))
				{
					// 押し出し後の位置設定
					enemyB->SetPosition(outPosition);
				}
			}
		}
	}
}
