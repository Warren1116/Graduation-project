#pragma once

#include <vector>
#include "Enemy.h"
#include <set>

// エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    // 唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    int identity = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)

    //　関数
    void Update(float elapsedTime);	// 更新処理
    void Register(Enemy* enemy);	// エネミー登録
    void Clear();	// エネミー全削除
    void DrawDebugPrimitive();	// デバッグプリミティブ描画
    void DrawDebugGUI();    // デバッグGUI描画
    void Remove(Enemy* enemy);    // エネミー削除

    ///////////////////////////////////////////////////////////////////////////////////
    //                              ゲッター・セッター                               //
    ///////////////////////////////////////////////////////////////////////////////////
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }   // エネミー数取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }    // エネミー取得
    Model* GetEnemyModel(int index);    // エネミーモデル取得
    Enemy* GetEnemyFromId(int id);    // 付与したIDからエネミーを取得

private:
    std::vector<Enemy*> enemies;
    std::set<Enemy*> removes;

    // エネミー同士の衝突処理
    void CollisionEnemyVsEnemies();
};

