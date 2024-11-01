#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "StageManager.h"
#include "StageMain.h"
#include "SceneClear.h"
#include "SceneTitle.h"
#include "System.h"
#include "Graphics\LightManager.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "ProjectileStraight.h"
#include "ProjectileManager.h"
#include "EnemyThief.h"
#include "CharacterManager.h"
#include "Messenger.h"

#include "Graphics/geometric_primitive.h"

SceneGame* SceneGame::instance = nullptr;


// 初期化
void SceneGame::Initialize()
{
    instance = this;
    Graphics& graphics = Graphics::Instance();
    // カメラ初期設定
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 10, -10),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        1000.0f
    );



    //	各種レンダラー生成
    {
        UINT width = static_cast<UINT>(graphics.GetScreenWidth());
        UINT height = static_cast<UINT>(graphics.GetScreenHeight());

        //	シャドウマップレンダラー
        shadowmapRenderer = std::make_unique<ShadowmapRenderer>(2048);

        //	シーンレンダラー
        sceneRenderer = std::make_unique<SceneRenderer>(width, height);

        //	ポストプロセスレンダラー
        postprocessingRenderer = std::make_unique<PostprocessingRenderer>(width / 2, height / 2);
        const SRVHandleList	list = sceneRenderer->GetRenderTargetShaderResourceViews();
        SRVHandle	depth = sceneRenderer->GetDepthStencilShaderResourceView();
        postprocessingRenderer->SetSceneData({ list[0], depth, width, height });

    }


    // ステージ初期化
    StageManager& stageManager = StageManager::Instance();

    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);


    //　プレイヤー生成
    player = std::make_unique<Player>(true);
    player->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(45), 0));

    // エネミー初期化
    EnemyManager& enemyManager = EnemyManager::Instance();
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // スライム（ステートマシン用）
    EnemyThief* people = new EnemyThief();
    people->SetPosition(DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f));
    people->SetTerritory(people->GetPosition(), 10.0f);
    enemyManager.Register(people);

    //	通信相手用に１匹増やす
    EnemyThief* people2 = new EnemyThief();
    people2->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 25.0f));
    people2->SetTerritory(people2->GetPosition(), 10.0f);
    enemyManager.Register(people2);



    //switch (stageMain->GetStageNum())
    //{
    //case 0:
    //{
    //    Zombie* zombies[11];
    //    for (int i = 0; i < 11; ++i)
    //    {
    //        zombies[i] = new Zombie();
    //        enemyManager.Register(zombies[i]);
    //    }
    //    zombies[0]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 4.0f));
    //    zombies[1]->SetPosition(DirectX::XMFLOAT3(22.0f, 0.0f, 4.0f));
    //    zombies[2]->SetPosition(DirectX::XMFLOAT3(30.0f, 0.0f, 4.0f));
    //    zombies[3]->SetPosition(DirectX::XMFLOAT3(37.0f, 0.0f, 4.0f));
    //    zombies[4]->SetPosition(DirectX::XMFLOAT3(45.0f, 0.0f, 4.0f));
    //    zombies[5]->SetPosition(DirectX::XMFLOAT3(44.0f, 0.0f, 20.0f));
    //    zombies[6]->SetPosition(DirectX::XMFLOAT3(37.0f, 0.0f, 20.0f));
    //    zombies[7]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 15.0f));
    //    zombies[8]->SetPosition(DirectX::XMFLOAT3(20.0f, 0.0f, 15.0f));
    //    zombies[9]->SetPosition(DirectX::XMFLOAT3(25.0f, 0.0f, 15.0f));
    //    zombies[10]->SetPosition(DirectX::XMFLOAT3(35.0f, 0.0f, 15.0f));
    //    for (int i = 0; i < 11; ++i)
    //    {
    //        zombies[i]->AddStart(zombies[i]);
    //    }
    //    ItemKey* items[3];
    //    for (int i = 0; i < 3; ++i)
    //    {
    //        items[i] = new ItemKey();
    //        items[i]->SetAngle({ DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(90) });
    //        itemManager.Register(items[i]);
    //    }
    //    items[0]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 15.0f));
    //    items[1]->SetPosition(DirectX::XMFLOAT3(44.0f, 0.0f, 4.0f));
    //    items[2]->SetPosition(DirectX::XMFLOAT3(3.0f, 0.0f, 13.5f));
    //    break;
    //}
    //for (int i = 0; i < 3; ++i)
    //{
    //    spotLights[i] = new Light(LightType::Spot);
    //    DirectX::XMFLOAT3 targetPosition;
    //    targetPosition = { itemManager.GetItem(i)->GetPosition().x, itemManager.GetItem(i)->GetPosition().y + 5.0f, itemManager.GetItem(i)->GetPosition().z };
    //    spotLights[i]->SetPosition(targetPosition);
    //    spotLights[i]->SetRange(10);
    //    spotLights[i]->SetDirection({ 0,-1,0 });
    //    spotLights[i]->SetColor({ 1,1,1,1 });
    //    spotLights[i]->SetOuterCorn(0.99f);
    //    LightManager::Instance().Register(spotLights[i]);
    //}
    //}

    //	モデルを各レンダラーに登録
    if (stageMain->GetStageNum() == 0 || stageMain->GetStageNum() == 1)
    {
        Model* list[] =
        {
            player->model.get(),
            stageMain->GetModel(),
            people->GetModel(),
            people2->GetModel(),
        };

        for (Model* model : list)
        {
            RegisterRenderModel(model);
            //shadowmapRenderer->RegisterRenderModel(model);
            //sceneRenderer->RegisterRenderModel(model);
            //const ModelResource* resource = model->GetResource();
            //for (const ModelResource::Material& material : resource->GetMaterials())
            //{
            //    ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
            //    mat.shaderId = static_cast<int>(ModelShaderId::Phong);
            //}
        }
    }

    // キャラクター生成処理
    CharacterManager& characterManager = CharacterManager::Instance();
    {
        // プレイヤー
        characterManager.Register(player.get());

        // カメラコントローラー初期化
        cameraController = std::make_unique<CameraController>();
       
        // エネミー初期化
        characterManager.Register(people);
        characterManager.Register(people2);

    }

    ammo = std::make_unique<Sprite>("Data/Sprite/ammo.png");
    dead = std::make_unique<Sprite>("Data/Sprite/youdead.png");
    clear = std::make_unique<Sprite>("Data/Sprite/black.png");
    text1 = std::make_unique<Sprite>("Data/Font/font1.png");
    text2 = std::make_unique<Sprite>("Data/Font/font6.png");

    // ゲージスプライト
    gauge = std::make_unique<Sprite>();
    crossHair = std::make_unique<Sprite>();

    // 平行光源を追加
    {

        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetDirection({ -0.5, -1, -1 });
        mainDirectionalLight->SetColor({ 1,1,1,1 });
        mainDirectionalLight->SetPosition({ 0,3,0 });
        LightManager::Instance().Register(mainDirectionalLight);

    }

    meta = new Meta(player.get(), &enemyManager);


    //bgm = Audio::Instance().LoadAudioSource("Data/Audio/zombie.wav");
    //heri = Audio::Instance().LoadAudioSource("Data/Audio/heri1.wav");
    //bgm->Play(true);
}

// 終了化
void SceneGame::Finalize()
{
    // エネミー終了化
    EnemyManager::Instance().Clear();

    // ステージ終了化
    StageManager::Instance().Clear();

    LightManager::Instance().Clear();

    EffectManager::Instance().Finalize();
    instance = nullptr;

    shadowmapRenderer->ClearRenderModel();
    sceneRenderer->ClearRenderModel();
    
    mainDirectionalLight = nullptr;

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // カメラコントローラー更新処理
    Camera& camera = Camera::Instance();
    cameraController->Update(elapsedTime);

    // ステージ更新処理
    StageManager::Instance().Update(elapsedTime);

    //DirectX::XMFLOAT3 target = player->GetPosition();
    //target.y += 0.5f;
    //CameraController::Instance().SetTarget(target);
    //CameraController::Instance().Update(elapsedTime);


    // プレイヤー更新処理
    player->Update(elapsedTime);

    //if (player->GetHealth() <= 0)  deadAlpha += 0.015f;
    //if (deadAlpha >= 1.5f)  SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));


    // エネミー更新処理
    EnemyManager::Instance().Update(elapsedTime);

    // エフェクト更新処理
    EffectManager::Instance().Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
    dc->OMSetRenderTargets(1, &rtv, dsv);
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



    // 描画処理
    RenderContext rc;
    rc.deviceContext = dc;

    // シャドウマップの描画
    shadowmapRenderer->Render(rc.deviceContext);

    // シーンの描画
    sceneRenderer->SetShadowmapData(shadowmapRenderer->GetShadowMapData());
    sceneRenderer->Render(rc.deviceContext);

    postprocessingRenderer->Render(rc.deviceContext);

    LightManager::Instance().PushRenderContext(rc);

    // カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();


    //2Dスプライト描画
    {
        //gauge->Render(dc,
        //    100, 750,
        //    15 * (player->GetMaxHealth() * player->GetMaxHealth()), 25,
        //    0, 0,
        //    static_cast<float>(gauge->GetTextureWidth()), static_cast<float>(gauge->GetTextureHeight()),
        //    0,
        //    0, 0, 0, 1);

        //gauge->Render(dc,
        //    100, 750,
        //    15 * (player->GetHealth() * player->GetMaxHealth()), 25,
        //    0, 0,
        //    static_cast<float>(gauge->GetTextureWidth()), static_cast<float>(gauge->GetTextureHeight()),
        //    0,
        //    0, 1, 0, 1);

        //crossHair->Render(dc,
        //    graphics.GetScreenWidth() / 2.0f + 5.0f, graphics.GetScreenHeight() / 2.0f + 5.0f,
        //    3.5, 3.5,
        //    0, 0,
        //    static_cast<float>(crossHair->GetTextureWidth()), static_cast<float>(crossHair->GetTextureHeight()),
        //    0,
        //    1, 1, 1, 1);

        //ammo->Render(dc,
        //    1265, 735,
        //    75, 75,
        //    0, 0,
        //    static_cast<float>(ammo->GetTextureWidth()), static_cast<float>(ammo->GetTextureHeight()),
        //    0,
        //    1, 1, 1, 1);
    }

    // 2DデバッグGUI描画
    {
        player->DrawDebugGUI();
        EnemyManager::Instance().DrawDebugGUI();

        //StageManager::Instance().GetStage(0)->DrawDebugGUI();
    }


    // デバッグ情報の表示
    {
        ImGui::Separator();
        LightManager::Instance().DrawDebugGUI();
        ImGui::Separator();
        shadowmapRenderer->DrawDebugGUI();
        ImGui::Separator();
        sceneRenderer->DrawDebugGUI();
        ImGui::Separator();
        postprocessingRenderer->DrawDebugGUI();

    }

    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(dead->GetTextureWidth());
        float textureHeight = static_cast<float>(dead->GetTextureHeight());

        dead->Render(dc,
            0, 0,
            screenWidth, screenHeight,
            0, 0,
            textureWidth, textureHeight,
            0,
            1, 1, 1, deadAlpha);

        clear->Render(dc,
            0, 0,
            screenWidth, screenHeight,
            0, 0,
            screenWidth, screenHeight,
            0,
            1, 1, 1, clearAlpha);
    }
}

// エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    // ビューポート
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // すべての敵の頭上にHPゲージを表示
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMVECTOR enemyPosition = DirectX::XMVectorSet(enemy->GetPosition().x,
            enemy->GetPosition().y + (enemy->GetHeight()),
            enemy->GetPosition().z, 0.0f);

        // 模範解答
        /*DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
        worldPosition.y += enemy->GetHeight();

        DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);*/

        // ワールド座標からスクリーン座標へ変換
        DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
            enemyPosition,
            viewport.TopLeftX,
            viewport.TopLeftY,
            viewport.Width,
            viewport.Height,
            viewport.MinDepth,
            viewport.MaxDepth,
            Projection,
            View,
            World);

        DirectX::XMFLOAT3 screenPosition;
        DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);

        if (screenPosition.z > 1.0f) continue;

        float textureWidth = static_cast<float>(gauge->GetTextureWidth());
        float textureHeight = static_cast<float>(gauge->GetTextureHeight());

        const float gaugeWidth = 30.0f;
        const float gaugeHeight = 5.0f;

        float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

        gauge->Render(dc,
            screenPosition.x - gaugeWidth * 0.5f, screenPosition.y,
            1 * gaugeWidth, gaugeHeight,
            0, 0,
            static_cast<float>(gauge->GetTextureWidth()), static_cast<float>(gauge->GetTextureHeight()),
            0,
            0, 0, 0, 1);

        gauge->Render(dc,
            screenPosition.x - gaugeWidth * 0.5f, screenPosition.y,
            healthRate * gaugeWidth, gaugeHeight,
            0, 0,
            static_cast<float>(gauge->GetTextureWidth()), static_cast<float>(gauge->GetTextureHeight()),
            0,
            1, 0, 0, 1);
    }
}

void SceneGame::RegisterRenderModel(Model* model)
{
    shadowmapRenderer->RegisterRenderModel(model);
    sceneRenderer->RegisterRenderModel(model);
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Material& material : resource->GetMaterials())
    {
        ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
        mat.shaderId = static_cast<int>(ModelShaderId::Phong);
        //mat.shaderId = static_cast<int>(ModelShaderId::Lambert);
    }
}

void SceneGame::UnregisterRenderModel(Model* model)
{
    shadowmapRenderer->UnregisterRenderModel(model);
    sceneRenderer->UnregisterRenderModel(model);

}


