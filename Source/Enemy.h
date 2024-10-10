#pragma once
#include "Graphics/Shader.h"
#include "Character.h"

// 前方宣言
class EnemyManager;

// エネミー
class Enemy : public Character
{
public:
	Enemy() {}
	~Enemy() override {}
	
	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 破棄
	void Destroy();

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	virtual void DrawDebugGUI() {}

	virtual float PlayerToBoss() { return 0; }

	void SetCameraFlg(bool flg) { camerashake = flg; }
	float GetCameraFlg() { return camerashake; }

	Model* GetModel() const { return model; }

protected:
	Model* model = nullptr;
	bool		camerashake = false;
	EnemyManager* manager = nullptr;
};

