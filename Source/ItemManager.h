#pragma once

#include <vector>
#include "Item.h"
#include <set>

// アイテムマネージャー
class ItemManager
{
private:
	ItemManager() {}
	~ItemManager() {}

public:
	// 唯一のインスタンス取得
	static ItemManager& Instance()
	{
		static ItemManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// エネミー登録
	void Register(Item* item);

	//// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader);

	// エネミー全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// エネミー数取得
	int GetItemCount() const { return static_cast<int>(items.size()); }

	// エネミー取得
	Item* GetItem(int index) { return items.at(index); }

	// エネミー削除
	void Remove(Item* enemy);

private:
	// エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();
	std::vector<Item*> items;
	std::set<Item*> removes;
};