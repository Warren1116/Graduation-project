#pragma once

#include <vector>
#include "Enemy.h"
#include <set>

class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    //唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //エネミー全削除
    void Clear();

    //エネミー登録
    void Register(Enemy* enemy);

    void DrawDebugPrimitive();


    //エネミー数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    //エネミー削除    
    void Remove(Enemy* enemy);
private:
    std::vector<Enemy*> enemies;
    void CollisionEnemyVsEnemies();
    std::set<Enemy*>  removes;
};
