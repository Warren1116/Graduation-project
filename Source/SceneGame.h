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

// ゲームシーン
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}
    static SceneGame& Instance() { return *instance; }

    // 初期化
    void Initialize() override;

    // 終了化
    void Finalize() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
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


    //	シャドウマップ用深度ステンシルバッファ
    std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
    //	ライトビュープロジェクション行列
    DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
    //	深度比較用のオフセット値
    float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
    //	影の色
    DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };

private:
    // 3D空間の描画
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

    //オフスクリーンレンダリング用描画ターゲット
    std::unique_ptr<RenderTarget> renderTarget;

    Light* mainDirectionalLight = nullptr;

private:
    static SceneGame* instance;
    Light* spotLights[3];
    std::unique_ptr<Sprite>		renderSplite;

    //MetaAIオブジェクト追加
    Meta* meta = nullptr;



};
