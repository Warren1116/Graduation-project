#pragma once

#include <vector>
#include <set>
#include "Projectile.h"

// 弾丸マネージャー
class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	static ProjectileManager& Instance()
	{
		static ProjectileManager instance;
		return instance;
	}
	// 更新処理
	void Update(float elapsedTime);

	////// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// 弾丸登録
	void Register(Projectile* projectile);

	// 弾丸全削除
	void Clear();

	// 弾丸削除
	void Remove(Projectile* projectile);

	// 弾丸数取得
	int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

	// 弾丸所得
	Projectile* GetProjectile(int index) { return projectiles.at(index); }

private:
	std::vector<Projectile*> projectiles;
	std::set<Projectile*>  removes;
};

