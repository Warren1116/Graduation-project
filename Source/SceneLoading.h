#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <thread>

class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() override {}

	// ‰Šú‰»
	void Initialize() override;

	// I—¹‰»
	void Finalize() override;

	// XVˆ—
	void Update(float elapsedTime) override;

	// •`‰æˆ—
	void Render() override;
private:
	static void LoadingThread(SceneLoading* scene);

private:
	Sprite* sprite = nullptr;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
	float angle = 0.0f;
	int loadTimer = 0;
};

