#pragma once

#include "Item.h"
#include "ItemAmmo.h"

class ItemAmmo : public Item
{
public:
	ItemAmmo();
	~ItemAmmo() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

private:
	

	void CollisionItemVsCrossHair() override;
};

