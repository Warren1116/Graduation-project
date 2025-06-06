#include "BrokenProjectileManager.h"

// コンストラクタ
BrokenProjectileManager::BrokenProjectileManager()
{
}

// デストラクタ
BrokenProjectileManager::~BrokenProjectileManager()
{
	Clear();
}

// 更新処理
void BrokenProjectileManager::Update(float elapsedTime)
{
	// 更新処理
	for (Projectile* projectile : projectiles)
	{
		projectile->Update(elapsedTime);
	}

	// 破棄処理
	// ※projectileの範囲for文中でerase()すると不具合が発生してしまうため、
	// 更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (Projectile* projectile : removes)
	{
		// std::vectorから要素を削除する場合は
		// イテレーターで削除しなければならない
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(),
			projectiles.end(), projectile);

		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}

		// 弾丸の破棄処理
		delete projectile;
	}
	// 破棄リストをクリア
	removes.clear();
}

// デバッグプリミティブ描画
void BrokenProjectileManager::DrawDebugPrimitive()
{
	for (Projectile* projectile : projectiles)
	{
		projectile->DrawDebugPrimitive();

	}
}

// 弾丸登録
void BrokenProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

// 弾丸全削除
void BrokenProjectileManager::Clear()
{
	for (Projectile* projectile : projectiles)
	{
		delete projectile;
	}
	projectiles.clear();
}

// 弾丸削除
void  BrokenProjectileManager::Remove(Projectile* projectile)
{
	// 破棄リストに追加
	removes.insert(projectile);
}