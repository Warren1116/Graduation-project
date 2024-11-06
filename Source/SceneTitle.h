#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Enemy.h"
#include "CameraController.h"
#include "Audio/Audio.h"
#include "Renderer\ShadowmapRenderer.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer\PostprocessingRenderer.h"


// �^�C�g���V�[��
class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;


private:
	std::unique_ptr<CameraController> cameraController;
	std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
	std::unique_ptr<SceneRenderer>			sceneRenderer;
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

	std::unique_ptr<Sprite> Font;
	std::unique_ptr<Sprite> TextFont;

	float alpha = 1.0f;
	float alphaSpeed = 0.02f;
	bool increasingAlpha = false;
};

