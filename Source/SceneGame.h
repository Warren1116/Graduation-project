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
#include "EnemyThief.h"
#include "Enemy.h"
#include "Renderer\ShadowmapCasterRenderer.h"

// ゲームシーン
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
        Grab,
        Swing,
        Climb,
        Dodge,
        Finish,
    };

    // 初期化
    void Initialize() override;

    // 終了化
    void Finalize() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render() override;


    //　チュトリアル時用の判定
    bool tutorialPause = false;
    bool GetIsTutorialPaused() { return tutorialPause; }
    //  チュトリアルステートの更新処理
    void UpdateTutorialState(float elapsedTime);
    //  チュトリアル始まる時にの設定
    void SceneGame::StartTutorial(TutorialState newState);
    //  チュトリアルステートの変更
    void SceneGame::AdvanceTutorialState(TutorialState newState);
    //  チュトリアルのタイマーチェック
    void SceneGame::CheckTutorialTimeout(float timeout);

    //  チュトリアル
    TutorialState GetTutorialState() { return tutorialState; }
    void SetTutorialState(TutorialState newstate) { tutorialState = newstate; }
    void UnpauseGame();
    void PauseGame();

    //  pause中の操作方法
    bool isTutorial = false;
    //  pause
    bool isPaused = false;


    //  コントローラーの使用判定
    bool GetIsUseController() { return UseController; }

    //モテルの登録
    void RegisterRenderModel(Model* model);
    void UnregisterRenderModel(Model* model);

    //  メインライトの取得
    Light* GetLight() { return mainDirectionalLight; }

    //  ウェーブの取得
    int GetCurrentWave() { return currentWave; }
    bool GetWaveInProgress() { return waveInProgress; }
    int GetTotalWaves() { return totalWaves; }
    bool IsNextWave() const;

    void SetRadialBlurActive(bool active) { radialBlurActive = active; }
    bool IsRadialBlurActive() const { return radialBlurActive; }


    std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
    std::unique_ptr<SceneRenderer>			sceneRenderer;
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
    //std::unique_ptr<ShadowmapCasterRenderer> shadowmapCasterRenderer;

    //	シャドウマップ用情報
    //std::unique_ptr<DepthStencil> shadowmapDepthStencil;	//シャドウマップ用深度ステンシルバッファ
    //float shadowDrawRect = 500.0f;
    //DirectX::XMFLOAT4X4 lightViewProjection;				//ライトビュープロジェクション行列
    //DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };		//影の色
    //float shadowBias = 0.001f;								//深度比較用のオフセット値

    //	シャドウマップ用深度ステンシルバッファ
    std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
    //	ライトビュープロジェクション行列
    DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
    //	深度比較用のオフセット値
    float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
    //	影の色
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };

    float shadowDrawRect = 500.0f;



private:
    std::unique_ptr<Player> player;
    std::unique_ptr<CameraController> cameraController;
    HeadUpDisplay* headUpDisplay = nullptr;
    std::unique_ptr<Sprite> Pause;
    std::unique_ptr<Sprite> PauseBackGround;
    std::unique_ptr<Sprite> ToTitle;
    std::unique_ptr<Sprite> ControlWay;
    std::unique_ptr<Sprite> ControllerPicture;
    std::unique_ptr<Sprite> ControllerFont;
    std::unique_ptr<Sprite> KeyBoardPicture;
    std::unique_ptr<Sprite> KeyBoardFont;
    bool ControllerButton = false;
    bool KeyBoardButton = false;

    //BGM
    std::unique_ptr<AudioSource> Bgm = nullptr;


    // ボタンの透明度
    float alpha = 1.0f;
    float alphaSpeed = 0.08f;
    bool increasingAlpha = false;

    //  オフスクリーンレンダリング用描画ターゲット
    std::unique_ptr<RenderTarget> renderTarget;

    //  メインライト
    Light* mainDirectionalLight = nullptr;

private:
    //  ウェーブのカウンター
    int currentWave = -1;
    //  ウェーブの最大数
    int totalWaves = 5;
    //  ウェーブ進行中かどうか
    bool waveInProgress = false;
    //  次のウェーブのタイマー
    float nextWaveTimer = 0.0f;

    //  ウェーブの敵を生成する
    void SpawnEnemiesForWave(int wave);
    //  次のウェーブを開始する
    void StartNextWave();
    //  ウェーブのクリアチェック
    void CheckWaveClear();


private:
    static SceneGame* instance;
    Light* spotLights[3];

    //  MetaAIオブジェクト追加
    std::unique_ptr<Meta> meta = nullptr;

    //  チュトリアルステート
    TutorialState tutorialState = TutorialState::First;
    float  tutorialTimer;
    bool canAcceptInput;
    bool firstTimeGetAttack = true;
    TutorialState lastState;


    //pause時コントローラーの選択判定
    DirectX::XMFLOAT2 controllerPos;
    bool UseController;

    //ラジアルブラー
    bool radialBlurActive = false;


};
