#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    for (auto it = removes.begin(); it != removes.end(); ++it)
    {
        Enemy* enemy = *it;

        auto enemyIt = std::find(enemies.begin(), enemies.end(), enemy);
        if (enemyIt != enemies.end())
        {
            enemies.erase(enemyIt);
        }
        delete enemy;
    }
    removes.clear();


    CollisionEnemyVsEnemies();
}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

//エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}


void EnemyManager::Remove(Enemy* enemy)
{
    removes.insert(enemy);
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = enemies.size();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemyA = enemies.at(i);
        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = enemies.at(j);
            DirectX::XMFLOAT3 outPosition;
            //if (Collision::IntersectSphereVsSphere(
            //enemyA->GetPosition(),
            //    enemyA->GetRadius(),
            //    enemyB->GetPosition(),
            //    enemyB->GetRadius(),
            //    outPosition))
            //{
            //    enemyB->SetPosition(outPosition);
            //}
            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyA->GetHeight(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                enemyB->GetHeight(),
                outPosition))
            {
                enemyB->SetPosition(outPosition);
            }
        }
    }

}


