#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Audio/Audio.h"
#include "Item.h"

// エネミー
class ItemKey : public Item
{
public:
	ItemKey();
	~ItemKey() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	//void Render(const RenderContext& rc, ModelShader* shader);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	Model* GetModel() const { return model; }
	bool OkDestroy() const { return okDestroy; }

	bool CanPickup() const { return canPickup; }

protected:

	void CollisionItemVsCrossHair() override;

protected:
	
};

