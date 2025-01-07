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


    std::unique_ptr<DepthStencil> shadowmapDepthStencil;	//シャドウマップ用深度ステンシルバッファ
    DirectX::XMFLOAT4X4 lightViewProjection;				//ライトビュープロジェクション行列
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };		//影の色
    float shadowBias = 0.001f;								//深度比較用のオフセット値

    ////	シャドウマップ用深度ステンシルバッファ
    //std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
    ////	ライトビュープロジェクション行列
    //DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
    ////	深度比較用のオフセット値
    //float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
    ////	影の色
    //DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<CameraController> cameraController;
    HeadUpDisplay* headUpDisplay = nullptr;


    //  オフスクリーンレンダリング用描画ターゲット
    std::unique_ptr<RenderTarget> renderTarget;

    Light* mainDirectionalLight = nullptr;

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
