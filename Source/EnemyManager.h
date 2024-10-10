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

	// 更新処理
	void Update(float elapsedTime);

	// エネミー登録
	void Register(Enemy* enemy);

	// エネミー全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	void DrawDebugGUI();

	// エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }
	
	Model* GetEnemyModel(int index);

	// エネミー削除
	void Remove(Enemy* enemy);

	// 付与したIDからエネミーを取得
	Enemy* GetEnemyFromId(int id);

private:
	// エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();
	std::vector<Enemy*> enemies;
	std::set<Enemy*> removes;

public:
	int identity = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)

};

