#include "ItemManager.h"
#include "Collision.h"

// 更新処理
void ItemManager::Update(float elapsedTime)
{
	for (Item* enemy : items)
	{
		enemy->Update(elapsedTime);
	}

	// 破棄処理
	for (Item* item : removes)
	{
		// std::vectorから要素を削除する場合は
		// イテレーターで削除しなければならない
		std::vector<Item*>::iterator it = std::find(items.begin(),
			items.end(), item);

		if (it != items.end())
		{
			items.erase(it);
		}

		// エネミーの破棄処理
		delete item;
	}
	// 破棄リストをクリア
	removes.clear();
}

// エネミー登録
void ItemManager::Register(Item* item)
{
	items.emplace_back(item);
}

//// 描画処理
//void ItemManager::Render(const RenderContext& rc, ModelShader* shader)
//{
//	for (Item* item : items)
//	{
//		item->Render(rc, shader);
//	}
//}

void ItemManager::Clear()
{
	for (Item* item : items)
	{
		delete item;
	}
	items.clear();
}

// デバッグプリミティブ描画
void ItemManager::DrawDebugPrimitive()
{
	for (Item* item : items)
	{
		item->DrawDebugPrimitive();
	}
}

// エネミー削除
void ItemManager::Remove(Item* item)
{
	// 破棄リストに追加
	removes.insert(item);
}
