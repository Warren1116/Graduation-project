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

// ゲームシーン
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}
    static SceneGame& Instance() { return *instance; }

    // チュートリアルステート
    enum class TutorialState {
        First, Move, Jump, Attack, Shot, CameraLock,
        Grab, Swing, Climb, Dodge, Healing, Ultimate,
        Finish,
    };

    //関数
    void Initialize() override;                 // 初期化
    void Finalize() override;                   // 終了化
    void Update(float elapsedTime) override;    // 更新処理
    void Render() override;                     // 描画処理


    //　チュトリアル
    void UpdateTutorialState(float elapsedTime);        //  チュトリアルステートの更新処理
    void StartTutorial(TutorialState newState);         //  チュトリアル始まる時にの設定
    void AdvanceTutorialState(TutorialState newState);    //  チュトリアルステートの変更
    void CheckTutorialTimeout(float timeout);           //  チュトリアルのタイマーチェック
    TutorialState GetTutorialState()
    { return tutorialState; }                           //  チュトリアルの状態を取得
    void SetTutorialState(TutorialState newstate)
    { tutorialState = newstate; }                       //  チュトリアルの状態を設定
    bool GetIsTutorialPaused() 
    { return tutorialPause; }                           //  チュトリアルが一時停止中かどうかを取得
    void UnpauseGame();
    void PauseGame();

    bool tutorialPause = false;     //　チュトリアル時用の判定
    bool isTutorial = false;        //  pause中の操作方法
    bool isPaused = false;          //  pause

    //  カメラの状態を更新
    void UpdateCameraState(float elapsedTime);

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

    // レンダラー
    std::unique_ptr<ShadowmapRenderer>		shadowmapRenderer;
    std::unique_ptr<SceneRenderer>			sceneRenderer;
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

    // シャドウ情報
    std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];    //	シャドウマップ用深度ステンシルバッファ
    DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];                //	ライトビュープロジェクション行列
    float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };      //	深度比較用のオフセット値
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };                     //	影の色
    float shadowDrawRect = 500.0f;

    // コリジョンメッシュ
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMFLOAT3 intersection_position;
    DirectX::XMFLOAT3 intersection_normal;
    std::string intersection_mesh;
    std::string intersection_material;

    //  チュートリアルの完了フラグ
    static bool tutorialCompleted;

private:
    //  プレイヤーとカメラコントローラー
    std::unique_ptr<Player> player;
    std::unique_ptr<CameraController> cameraController;

    // UIスプライト
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

    // ボタンの透明度
    float alpha = 1.0f;
    float alphaSpeed = 0.08f;
    bool increasingAlpha = false;

    //  オフスクリーンレンダリング用描画ターゲット
    std::unique_ptr<RenderTarget> renderTarget;

    //  メインライト
    Light* mainDirectionalLight = nullptr;

    //Wave
    int currentWave = -1;           //  ウェーブのカウンター
    int totalWaves = 5;             //  ウェーブの最大数
    bool waveInProgress = false;    //  ウェーブ進行中かどうか
    float nextWaveTimer = 0.0f;     //  次のウェーブのタイマー
    float restartTimer = 5.0f;    //  リスタートのタイマー


    //  ウェーブの敵を生成する
    void SpawnEnemiesForWave(int wave);
    //  次のウェーブを開始する
    void StartNextWave();
    //  ウェーブのクリアチェック
    void CheckWaveClear();

    //  MetaAIオブジェクト追加
    std::unique_ptr<Meta> meta = nullptr;

    //  チュトリアルステート
    TutorialState tutorialState = TutorialState::First;
    float  tutorialTimer;
    bool canAcceptInput;
    bool firstTimeGetAttack = true;
    TutorialState lastState;
    bool firstTimeHealing = true;
    bool firstTimeUltimate = true;
    std::vector<TutorialState>  tutorialStack;


    //pause時コントローラーの選択判定
    DirectX::XMFLOAT2 controllerPos;
    bool UseController;

    //ラジアルブラー
    bool radialBlurActive = false;

    //  カメラタイマー
    float cameraTimer;

    // インスタンス
    static SceneGame* instance;
};
