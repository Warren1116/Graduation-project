#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Zombie.h"
#include "CameraController.h"
#include "Audio/Audio.h"
#include "Renderer\ShadowmapRenderer.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer\PostprocessingRenderer.h"
#include "Player.h"
#include "Gun.h"

class Task
{
public:
	Task()
	{
		for (bool content : contents)
		{
			content = false;
		}
	}
	~Task() {}

	static Task& Instance()
	{
		static Task* instance;
		return *instance;
	}
	
public:
	enum class Contents
	{
		Move,
		Shot,
		Pickup,

		Max
	};

	bool AllClear()
	{
		for (bool content : contents)
		{
			if (!contents) return false;
		}
		return true;
	}

public:
	bool contents[static_cast<size_t>(Contents::Max)] = { false };
};

// チュートリアルシーン
class SceneTutorial : public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;


	bool CollisionScopeVsEnemy(Enemy* enemy);

	void RenderEnemyGauge(ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

	void MoveTask();
	void ShotTask();
	void PickupTask();
	void ChangeStage();

private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> scope;
	std::unique_ptr<AudioSource> fire = nullptr;
	std::unique_ptr<CameraController> cameraController;
	bool debug = false;

	std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
	std::unique_ptr<SceneRenderer>			sceneRenderer;
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

	std::unique_ptr<Player> player;
	std::unique_ptr<Gun> gun;
	std::unique_ptr<Sprite> gauge;
	std::unique_ptr<Sprite> crossHair;
	std::unique_ptr<Sprite> ammo;
	std::unique_ptr<Sprite> text;

	std::unique_ptr<Task> task;
	DirectX::XMFLOAT3 targetPosition;
	float targetRange;

	DirectX::XMFLOAT3 nextStagePosition;
};