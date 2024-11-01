//#pragma once
//
//#include "Graphics/Sprite.h"
//#include "Scene.h"
//#include "Enemy.h"
//#include "CameraController.h"
//#include "Audio/Audio.h"
//#include "Renderer\ShadowmapRenderer.h"
//#include "Renderer/SceneRenderer.h"
//#include "Renderer\PostprocessingRenderer.h"
//
//
//// �^�C�g���V�[��
//class SceneTitle : public Scene
//{
//public:
//	SceneTitle() {}
//	~SceneTitle() override {}
//
//	// ������
//	void Initialize() override;
//
//	// �I����
//	void Finalize() override;
//
//	// �X�V����
//	void Update(float elapsedTime) override;
//
//	// �`�揈��
//	void Render() override;
//
//
//	bool CollisionScopeVsEnemy(Enemy* enemy);
//private:
//	std::unique_ptr<Sprite> title1;
//	std::unique_ptr<Sprite> scope;
//	std::unique_ptr<Sprite> select;
//	std::unique_ptr<AudioSource> fire = nullptr;
//	std::unique_ptr<AudioSource> title = nullptr;
//	std::unique_ptr<CameraController> cameraController;
//	bool debug = false;
//
//	std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
//	std::unique_ptr<SceneRenderer>			sceneRenderer;
//	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
//
//
//};
//
