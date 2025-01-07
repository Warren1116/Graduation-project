#pragma once

#include "Player.h"
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
#include "HeadUpDisplay.h"


// �Q�[���V�[��
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}
    static SceneGame& Instance() { return *instance; }

    enum class TutorialState
    {
        First,
        Move,
        Jump,
        Attack,
        Shot,
        CameraLock,
        LockAttack,
        LockShot,
        Swing,
        Climb,
        Finish,
    };

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render() override;


    //�@�`���g���A���p����
    bool isPaused = false;
    //  �`���g���A���X�e�[�g�̍X�V����
    void UpdateTutorialState(float elapsedTime);
    //  �`���g���A���n�܂鎞�ɂ̐ݒ�
    void SceneGame::StartTutorial(TutorialState newState);
    //  �`���g���A���X�e�[�g�̕ύX
    void SceneGame::AdvanceTutorialState(TutorialState newState);
    //  �`���g���A���̃^�C�}�[�`�F�b�N
    void SceneGame::CheckTutorialTimeout(float timeout);

    bool GetIsPaused() { return isPaused; }

    TutorialState GetTutorialState() { return tutorialState; }
    void SetTutorialState(TutorialState newstate) { tutorialState = newstate; }
    void UnpauseGame();
    void PauseGame();

    void RegisterRenderModel(Model* model);
    void UnregisterRenderModel(Model* model);


    Light* GetLight() { return mainDirectionalLight; }

    std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
    std::unique_ptr<SceneRenderer>			sceneRenderer;
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;


    std::unique_ptr<DepthStencil> shadowmapDepthStencil;	//�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
    DirectX::XMFLOAT4X4 lightViewProjection;				//���C�g�r���[�v���W�F�N�V�����s��
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };		//�e�̐F
    float shadowBias = 0.001f;								//�[�x��r�p�̃I�t�Z�b�g�l

    ////	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
    //std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
    ////	���C�g�r���[�v���W�F�N�V�����s��
    //DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
    ////	�[�x��r�p�̃I�t�Z�b�g�l
    //float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
    ////	�e�̐F
    //DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<CameraController> cameraController;
    HeadUpDisplay* headUpDisplay = nullptr;


    //  �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<RenderTarget> renderTarget;

    Light* mainDirectionalLight = nullptr;

private:
    static SceneGame* instance;
    Light* spotLights[3];
    std::unique_ptr<Sprite>		renderSplite;

    //  MetaAI�I�u�W�F�N�g�ǉ�
    std::unique_ptr<Meta> meta = nullptr;

    //  �`���g���A���X�e�[�g
    TutorialState tutorialState = TutorialState::First;
    float  tutorialTimer;
    bool canAcceptInput;


};
