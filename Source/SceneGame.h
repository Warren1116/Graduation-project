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
#include "EnemyThief.h"
#include "Enemy.h"
#include "CharacterManager.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}
    static SceneGame& Instance() { return *instance; }

    // �`���[�g���A���X�e�[�g
    enum class TutorialState {
        First, Move, Jump, Attack, Shot, CameraLock,
        Grab, Swing, Climb, Dodge, Healing, Ultimate,
        Finish,
    };

    //�֐�
    void Initialize() override;                 // ������
    void Finalize() override;                   // �I����
    void Update(float elapsedTime) override;    // �X�V����
    void Render() override;                     // �`�揈��


    //�@�`���g���A��
    void UpdateTutorialState(float elapsedTime);        //  �`���g���A���X�e�[�g�̍X�V����
    void StartTutorial(TutorialState newState);         //  �`���g���A���n�܂鎞�ɂ̐ݒ�
    void AdvanceTutorialState(TutorialState newState);    //  �`���g���A���X�e�[�g�̕ύX
    void CheckTutorialTimeout(float timeout);           //  �`���g���A���̃^�C�}�[�`�F�b�N
    TutorialState GetTutorialState()
    { return tutorialState; }                           //  �`���g���A���̏�Ԃ��擾
    void SetTutorialState(TutorialState newstate)
    { tutorialState = newstate; }                       //  �`���g���A���̏�Ԃ�ݒ�
    bool GetIsTutorialPaused() 
    { return tutorialPause; }                           //  �`���g���A�����ꎞ��~�����ǂ������擾
    void UnpauseGame();
    void PauseGame();

    bool tutorialPause = false;     //�@�`���g���A�����p�̔���
    bool isTutorial = false;        //  pause���̑�����@
    bool isPaused = false;          //  pause

    //  �J�����̏�Ԃ��X�V
    void UpdateCameraState(float elapsedTime);

    //  �R���g���[���[�̎g�p����
    bool GetIsUseController() { return UseController; }

    //���e���̓o�^
    void RegisterRenderModel(Model* model);
    void UnregisterRenderModel(Model* model);

    //  ���C�����C�g�̎擾
    Light* GetLight() { return mainDirectionalLight; }

    //  �E�F�[�u�̎擾
    int GetCurrentWave() { return currentWave; }
    bool GetWaveInProgress() { return waveInProgress; }
    int GetTotalWaves() { return totalWaves; }
    bool IsNextWave() const;

    // �����_���[
    std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
    std::unique_ptr<SceneRenderer>			sceneRenderer;
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

    // �V���h�E���
    std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];    //	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
    DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];                //	���C�g�r���[�v���W�F�N�V�����s��
    float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };      //	�[�x��r�p�̃I�t�Z�b�g�l
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };                     //	�e�̐F
    float shadowDrawRect = 500.0f;

    // �R���W�������b�V��
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMFLOAT3 intersection_position;
    DirectX::XMFLOAT3 intersection_normal;
    std::string intersection_mesh;
    std::string intersection_material;

    //  �`���[�g���A���̊����t���O
    static bool tutorialCompleted;

private:
    //  �v���C���[�ƃJ�����R���g���[���[
    std::unique_ptr<Player> player;
    std::unique_ptr<CameraController> cameraController;

    // UI�X�v���C�g
    std::unique_ptr<Sprite> Pause;
    std::unique_ptr<Sprite> PauseBackGround;
    std::unique_ptr<Sprite> ToTitle;
    std::unique_ptr<Sprite> ControlWay;
    std::unique_ptr<Sprite> ControllerPicture;
    std::unique_ptr<Sprite> ControllerFont;
    std::unique_ptr<Sprite> KeyBoardPicture;
    std::unique_ptr<Sprite> KeyBoardFont;

    //BGM
    std::unique_ptr<AudioSource> Bgm = nullptr;

    bool ControllerButton = false;
    bool KeyBoardButton = false;
    DirectX::XMFLOAT3 cameraPos;
    DirectX::XMFLOAT3 cameraAngle;

    // �{�^���̓����x
    float alpha = 1.0f;
    float alphaSpeed = 0.08f;
    bool increasingAlpha = false;

    //  �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<RenderTarget> renderTarget;

    //  ���C�����C�g
    Light* mainDirectionalLight = nullptr;

    //Wave
    int currentWave = -1;           //  �E�F�[�u�̃J�E���^�[
    int totalWaves = 5;             //  �E�F�[�u�̍ő吔
    bool waveInProgress = false;    //  �E�F�[�u�i�s�����ǂ���
    float nextWaveTimer = 0.0f;     //  ���̃E�F�[�u�̃^�C�}�[
    float restartTimer = 5.0f;    //  ���X�^�[�g�̃^�C�}�[


    //  �E�F�[�u�̓G�𐶐�����
    void SpawnEnemiesForWave(int wave);
    //  ���̃E�F�[�u���J�n����
    void StartNextWave();
    //  �E�F�[�u�̃N���A�`�F�b�N
    void CheckWaveClear();

    //  MetaAI�I�u�W�F�N�g�ǉ�
    std::unique_ptr<Meta> meta = nullptr;

    //  �`���g���A���X�e�[�g
    TutorialState tutorialState = TutorialState::First;
    float  tutorialTimer;
    bool canAcceptInput;
    bool firstTimeGetAttack = true;
    TutorialState lastState;
    bool firstTimeHealing = true;
    bool firstTimeUltimate = true;
    std::vector<TutorialState>  tutorialStack;


    //pause���R���g���[���[�̑I�𔻒�
    DirectX::XMFLOAT2 controllerPos;
    bool UseController;

    //���W�A���u���[
    bool radialBlurActive = false;

    //  �J�����^�C�}�[
    float cameraTimer;

    // �C���X�^���X
    static SceneGame* instance;
};
