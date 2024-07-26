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
    //�B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�G�l�~�[�S�폜
    void Clear();

    //�G�l�~�[�o�^
    void Register(Enemy* enemy);

    void DrawDebugPrimitive();


    //�G�l�~�[���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    //�G�l�~�[�폜    
    void Remove(Enemy* enemy);
private:
    std::vector<Enemy*> enemies;
    void CollisionEnemyVsEnemies();
    std::set<Enemy*>  removes;
};
