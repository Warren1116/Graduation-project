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
        Swing,
        Climb,
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


    //　チュトリアル用判定
    bool isPaused = false;
    //  チュトリアルステートの更新処理
    void UpdateTutorialState(float elapsedTime);
    //  チュトリアル始まる時にの設定
    void SceneGame::StartTutorial(TutorialState newState);
    //  チュトリアルステートの変更
    void SceneGame::AdvanceTutorialState(TutorialState newState);
    //  チュトリアルのタイマーチェック
    void SceneGame::CheckTutorialTimeout(float timeout);

    bool GetIsPaused() { return isPaused; }

    bool isTutorial = false;
   
    //  チュトリアル
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
    std::unique_ptr<Sprite> KeyBoardFont;
    bool ControllerButton = false;
    bool KeyBoardButton = false;

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
    int currentWave = 0;
    //  ウェーブの最大数
    int totalWaves = 3;
    //  ウェーブ進行中かどうか
    bool waveInProgress = false;

    //  ウェーブの敵を生成する
    void SpawnEnemiesForWave(int wave);
    //  次のウェーブを開始する
    void StartNextWave();
    //  ウェーブのクリアチェック
    void CheckWaveClear();


private:
    static SceneGame* instance;
    Light* spotLights[3];
    std::unique_ptr<Sprite>		renderSplite;

    //  MetaAIオブジェクト追加
    std::unique_ptr<Meta> meta = nullptr;

    //  チュトリアルステート
    TutorialState tutorialState = TutorialState::First;
    float  tutorialTimer;
    bool canAcceptInput;


};
