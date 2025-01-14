#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "Graphics\LightManager.h"
#include "StageMain.h"
#include "StageManager.h"

// 初期化
void SceneTitle::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    // カメラコントローラー初期化
    cameraController = std::make_unique<CameraController>();

    //	各種レンダラー生成
    {
        UINT width = static_cast<UINT>(graphics.GetScreenWidth());
        UINT height = static_cast<UINT>(graphics.GetScreenHeight());

        //	シャドウマップレンダラー
        shadowmapRenderer = std::make_unique<ShadowmapRenderer>(2048);
        //shadowmapCasterRenderer = std::make_unique<ShadowmapCasterRenderer>(2048);

        //	シーンレンダラー
        sceneRenderer = std::make_unique<SceneRenderer>(width, height);

        //	ポストプロセスレンダラー
        postprocessingRenderer = std::make_unique<PostprocessingRenderer>(width / 2, height / 2);
        const SRVHandleList	list = sceneRenderer->GetRenderTargetShaderResourceViews();
        SRVHandle	depth = sceneRenderer->GetDepthStencilShaderResourceView();
        postprocessingRenderer->SetSceneData({ list[0], depth, width, height });

    }

    // カメラ初期設定
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        cameraPos,
        cameraAngle,
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        1000.0f
    );
    // ステージ初期化
    StageManager& stageManager = StageManager::Instance();

    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);

    //　プレイヤー生成

    player = std::make_unique<Player>(true);
    player->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(-125), 0));
    player->SetPosition({ 62.5f,72.0f,27.0f });



    //	モデルを各レンダラーに登録
    Model* list[] =
    {
        stageMain->GetModel(),
        player->model.get(),
    };
    for (Model* model : list)
    {
        RegisterRenderModel(model);
    }

    // 平行光源を追加
    {
        Light* light = new Light(LightType::Directional);
        light->SetDirection({ -0.4f, -0.7f, -0.4f });
        light->SetColor({ 1,1,1,1 });
        light->SetPosition({ 0,0,0 });
        LightManager::Instance().Register(light);
    }

    StageMain& stage = StageMain::Instance();

    Font = std::make_unique<Sprite>("Data/Sprite/Font3.png");
    TextFont = std::make_unique<Sprite>("Data/Font/font4.png");

    Title = std::make_unique<Sprite>("Data/Sprite/Title.png");
    Start = std::make_unique<Sprite>("Data/Sprite/Start.png");
    Quit = std::make_unique<Sprite>("Data/Sprite/Quit.png");

}

// 終了化
void SceneTitle::Finalize()
{
    EnemyManager::Instance().Clear();

    // ステージ終了化
    StageManager::Instance().Clear();

    LightManager::Instance().Clear();

    shadowmapRenderer->ClearRenderModel();
    //shadowmapCasterRenderer->ClearRenderModel();
    sceneRenderer->ClearRenderModel();


}

//更新処理
void SceneTitle::Update(float elapsedTime)
{

    // カメラコントローラー更新処理
    Camera::Instance().SetLookAt(cameraPos, cameraAngle, DirectX::XMFLOAT3(0, 1, 0));
    // プレイヤー更新処理
    player->Update(elapsedTime);

    if (increasingAlpha)
    {
        alpha += alphaSpeed * 0.3f;
        if (alpha >= 1.0f)
        {
            alpha = 1.0f;
            increasingAlpha = false;
        }
    }
    else
    {
        alpha -= alphaSpeed * 0.3f;
        if (alpha <= 0.3f)
        {
            alpha = 0.3f;
            increasingAlpha = true;
        }
    }

    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();


    if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 450 && mouse.GetPositionY() < 600 && mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }


    if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 600 && mouse.GetPositionY() < 750 && mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        PostQuitMessage(1);
    }

    EnemyManager::Instance().Update(elapsedTime);
}



//描画処理
void SceneTitle::Render()
{

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 1.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 描画処理
    RenderContext rc;
    rc.deviceContext = dc;
    LightManager::Instance().PushRenderContext(rc);

    // カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    ////// カメラパラメータ設定
    //Camera& camera = Camera::Instance();
    //rc.view = camera.GetView();
    //rc.projection = camera.GetProjection();

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    // 3D
    {
        //シャドウマップの描画
        shadowmapRenderer->Render(dc);
        //shadowmapCasterRenderer->Render(dc);

        //シーンの描画
        sceneRenderer->SetShadowmapData(shadowmapRenderer->GetShadowMapData());
        //sceneRenderer->SetShadowmapData(shadowmapCasterRenderer->GetShadowMapData());
        sceneRenderer->Render(dc);

        postprocessingRenderer->Render(dc);


    }

    // 2D
    {
        Title->Render(dc,
            screenWidth / 3.5f, -100,
            650.0f, 650.0f,
            0, 0,
            static_cast<float>(Title->GetTextureWidth()), static_cast<float>(Title->GetTextureHeight()),
            0,
            1, 1, 1, 1);

        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 450 && mouse.GetPositionY() < 600)
        {
            Start->Render(dc,
                150, 450,
                200.0f, 200.0f,
                0, 0,
                static_cast<float>(Start->GetTextureWidth()), static_cast<float>(Start->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
        }
        else
        {
            Start->Render(dc,
                150, 450,
                150.0f, 150.0f,
                0, 0,
                static_cast<float>(Start->GetTextureWidth()), static_cast<float>(Start->GetTextureHeight()),
                0,
                1, 1, 1, 1);
        }

        if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 600 && mouse.GetPositionY() < 750)
        {
            Quit->Render(dc,
                150, 600,
                200.0f, 200.0f,
                0, 0,
                static_cast<float>(Quit->GetTextureWidth()), static_cast<float>(Quit->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
        }
        else
        {
            Quit->Render(dc,
                150, 600,
                150.0f, 150.0f,
                0, 0,
                static_cast<float>(Quit->GetTextureWidth()), static_cast<float>(Quit->GetTextureHeight()),
                0,
                1, 1, 1, 1);
        }

        //TextFont->textout(dc, "Spider X", screenWidth / 3, screenHeight / 3, 50, 50, {1,1,1,1});

        //Font->Render(dc,
        //    screenWidth / 2, screenHeight / 2,
        //    180.0f, 180.0f,
        //    0, 0,
        //    static_cast<float>(Font->GetTextureWidth()), static_cast<float>(Font->GetTextureHeight()),
        //    0,
        //    1, 1, 1, alpha);
    }

    // デバッグ情報の表示
    {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("cameraPos", nullptr, ImGuiWindowFlags_None))
        {
            ImGui::SliderFloat3("cameraPos", &cameraPos.x, 0.0f, 100.0f);
            ImGui::SliderFloat3("cameraAngle", &cameraAngle.x, 0.0f, 50.0f);

        }
        ImGui::End();

        LightManager::Instance().DrawDebugGUI();
    }

}

//  モデルをレンダラーに登録
void SceneTitle::RegisterRenderModel(Model* model)
{
    //shadowmapCasterRenderer->RegisterRenderModel(model);
    shadowmapRenderer->RegisterRenderModel(model);
    sceneRenderer->RegisterRenderModel(model);
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Material& material : resource->GetMaterials())
    {
        ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
        mat.shaderId = static_cast<int>(ModelShaderId::Phong);
    }
}

//  モデルをレンダラーに削除
void SceneTitle::UnregisterRenderModel(Model* model)
{
    shadowmapRenderer->UnregisterRenderModel(model);
    //shadowmapCasterRenderer->UnregisterRenderModel(model);
    sceneRenderer->UnregisterRenderModel(model);


}
