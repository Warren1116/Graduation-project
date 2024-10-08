#pragma once

#include "Player.h"
#include "Zombie.h"
#include "Gun.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "Item.h"
#include "ItemKey.h"
#include "ItemAmmo.h"
#include "ItemManager.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"
#include "Object.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Graphics/Light.h"
#include "Renderer/ShadowmapRenderer.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/PostprocessingRenderer.h"

static bool Scenelock = false;

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	void RenderEnemyGauge(ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

	
	
	static SceneGame& Instance()
	{
		static SceneGame instance;
		return instance;
	}

private:
	std::unique_ptr<Player> player;
	Item* item = nullptr;
	ItemManager& itemManager = ItemManager::Instance();
	std::unique_ptr<Gun> gun;
	std::unique_ptr<Sprite> gauge;
	std::unique_ptr<Sprite> crossHair;
	std::unique_ptr<Sprite> ammo;
	std::unique_ptr<Sprite> dead;
	std::unique_ptr<Sprite> clear;
	std::unique_ptr<Sprite> text1;
	std::unique_ptr<Sprite> text2;
	std::unique_ptr<AudioSource> bgm = nullptr;
	std::unique_ptr<AudioSource> heri = nullptr;
	//std::unique_ptr<CameraController> cameraController;
	float a = 0;
	float deadAlpha = 0;
	float clearAlpha = 0;
	int clearTimer = 0;

private:

	std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
	std::unique_ptr<SceneRenderer>			sceneRenderer;
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

	Light* spotLights[3];
	std::unique_ptr<Sprite>		renderSplite;
};
