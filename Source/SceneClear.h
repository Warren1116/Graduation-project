#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"

// �^�C�g���V�[��
class SceneClear : public Scene
{
public:
	SceneClear() {}
	~SceneClear() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	std::unique_ptr<Sprite> clear;
	std::unique_ptr<Sprite> black;
	std::unique_ptr<Sprite> text;
	std::unique_ptr<AudioSource> heri = nullptr;

	float blackAlpha = 1.5f;
	int tenmetu = 0;
};