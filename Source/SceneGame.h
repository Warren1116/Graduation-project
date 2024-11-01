#pragma once

#include "Player.h"
#include "Zombie.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Graphics/Light.h"
#include "Renderer/ShadowmapRenderer.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/PostprocessingRenderer.h"
#include "MetaAI.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}
    static SceneGame& Instance() { return *instance; }

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render() override;

    void RenderEnemyGauge(ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);

    void RegisterRenderModel(Model* model);
    void UnregisterRenderModel(Model* model);

    Light* GetLight() { return mainDirectionalLight; }

    std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
    std::unique_ptr<SceneRenderer>			sceneRenderer;
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;


    //	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
    std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
    //	���C�g�r���[�v���W�F�N�V�����s��
    DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
    //	�[�x��r�p�̃I�t�Z�b�g�l
    float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
    //	�e�̐F
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };

private:
    // 3D��Ԃ̕`��
    //void Render3DScene();


private:
    std::unique_ptr<Player> player;

    std::unique_ptr<Sprite> gauge;
    std::unique_ptr<Sprite> crossHair;
    std::unique_ptr<Sprite> ammo;
    std::unique_ptr<Sprite> dead;
    std::unique_ptr<Sprite> clear;
    std::unique_ptr<Sprite> text1;
    std::unique_ptr<Sprite> text2;
    std::unique_ptr<AudioSource> bgm = nullptr;
    std::unique_ptr<AudioSource> heri = nullptr;

    std::unique_ptr<CameraController> cameraController;

    float a = 0;
    float deadAlpha = 0;
    float clearAlpha = 0;
    int clearTimer = 0;

    //�I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<RenderTarget> renderTarget;

    Light* mainDirectionalLight = nullptr;

private:
    static SceneGame* instance;
    Light* spotLights[3];
    std::unique_ptr<Sprite>		renderSplite;

    //MetaAI�I�u�W�F�N�g�ǉ�
    Meta* meta = nullptr;



};
