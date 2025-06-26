#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Enemy.h"
#include "CameraController.h"
#include "Audio/Audio.h"
#include "Renderer\ShadowmapRenderer.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer\PostprocessingRenderer.h"
#include "Player.h"
#include "Graphics\Light.h"
#include "Graphics\Shader\ParticleShader.h"


// �^�C�g���V�[��
class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}
	static SceneTitle& Instance() { return *instance; }

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

	void RegisterRenderModel(Model* model);
	void UnregisterRenderModel(Model* model);


private:
	std::unique_ptr<CameraController> cameraController;
	std::unique_ptr<SceneRenderer>			sceneRenderer;
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
	std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
    std::unique_ptr<ParticleShader> particleShader;

	std::unique_ptr<Sprite> Title;
	std::unique_ptr<Sprite> Start;
	std::unique_ptr<Sprite> Quit;
	std::unique_ptr<Sprite> A_Button;

    // �{�^���̓����x
	float alpha = 1.0f;
	float alphaSpeed = 0.02f;
	bool increasingAlpha = false;

	std::unique_ptr<Player> player;
	DirectX::XMFLOAT3 cameraPos = { 66,75,28 };
	DirectX::XMFLOAT3 cameraAngle = { 2,50,1 };

    DirectX::XMFLOAT2 controllerPos;
    bool UseController;

	//  ���C�����C�g
	Light* mainDirectionalLight = nullptr;

	//BGM
	std::unique_ptr<AudioSource> Bgm = nullptr;

private:
	static SceneTitle* instance;

};

