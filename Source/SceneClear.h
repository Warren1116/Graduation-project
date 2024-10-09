#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"

// タイトルシーン
class SceneClear : public Scene
{
public:
	SceneClear() {}
	~SceneClear() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	std::unique_ptr<Sprite> clear;
	std::unique_ptr<Sprite> black;
	std::unique_ptr<Sprite> text;
	std::unique_ptr<AudioSource> heri = nullptr;

	float blackAlpha = 1.5f;
	int tenmetu = 0;
};