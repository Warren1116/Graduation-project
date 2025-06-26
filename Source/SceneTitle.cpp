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
#include "CharacterManager.h"

#define DEBUG

SceneTitle* SceneTitle::instance = nullptr;

// ������
void SceneTitle::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    // �J�����R���g���[���[������
    cameraController = std::make_unique<CameraController>();


    particleShader = std::make_unique<ParticleShader>(graphics.GetDevice());


    //	�e�탌���_���[����
    {
        UINT width = static_cast<UINT>(graphics.GetScreenWidth());
        UINT height = static_cast<UINT>(graphics.GetScreenHeight());

        //	�V���h�E�}�b�v�����_���[
        shadowmapRenderer = std::make_unique<ShadowmapRenderer>(2048);

        //	�V�[�������_���[
        sceneRenderer = std::make_unique<SceneRenderer>(width, height);

        //	�|�X�g�v���Z�X�����_���[
        postprocessingRenderer = std::make_unique<PostprocessingRenderer>(width / 2, height / 2);
        const SRVHandleList	list = sceneRenderer->GetRenderTargetShaderResourceViews();
        SRVHandle	depth = sceneRenderer->GetDepthStencilShaderResourceView();
        postprocessingRenderer->SetSceneData({ list[0], depth, width, height });

    }

    // �J���������ݒ�
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
    // �X�e�[�W������
    StageManager& stageManager = StageManager::Instance();

    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);

    //�@�v���C���[����

    player = std::make_unique<Player>(true);
    player->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(-125), 0));
    player->SetPosition({ 62.5f,72.0f,27.0f });

    CharacterManager& characterManager = CharacterManager::Instance();
    {
        // �v���C���[
        characterManager.Register(player.get());
    }

    //	���f�����e�����_���[�ɓo�^
    Model* list[] =
    {
        stageMain->GetCityModel(),
        stageMain->GetGroundModel(),
        player->model.get(),
    };
    for (Model* model : list)
    {
        RegisterRenderModel(model);
    }


    // ���s������ǉ�
    {
        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetDirection({ -0.3, -0.8, -0.3 });
        mainDirectionalLight->SetColor({ 1,1,1,1 });
        mainDirectionalLight->SetPosition({ 0,3,0 });
        LightManager::Instance().Register(mainDirectionalLight);
        LightManager::Instance().SetShadowmapLight(mainDirectionalLight);
    }

    StageMain& stage = StageMain::Instance();


    Title = std::make_unique<Sprite>("Data/Sprite/Title.png");
    Start = std::make_unique<Sprite>("Data/Sprite/Start.png");
    Quit = std::make_unique<Sprite>("Data/Sprite/Quit.png");
    A_Button = std::make_unique<Sprite>("Data/Sprite/A_Button.png");

    Bgm = Audio::Instance().LoadAudioSource("Data/Audio/TitleBgm.wav");


    UseController = true;
    controllerPos = { 0,540 };

}

// �I����
void SceneTitle::Finalize()
{
    EnemyManager::Instance().Clear();

    // �X�e�[�W�I����
    StageManager::Instance().Clear();

    LightManager::Instance().Clear();

    shadowmapRenderer->ClearRenderModel();

    sceneRenderer->ClearRenderModel();

}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
    RenderContext rc;
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();
    rc.viewPosition = { camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 1.0f };



    particleShader->UpdateParticles(elapsedTime);


    Bgm->Play(true, 0.1f);
    // �J�����R���g���[���[�X�V����
    Camera::Instance().SetLookAt(cameraPos, cameraAngle, DirectX::XMFLOAT3(0, 1, 0));
    // �v���C���[�X�V����
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

    GamePad& gamePad = Input::Instance().GetGamePad();
    const GamePadButton controllerButton =
        GamePad::BTN_UP
        | GamePad::BTN_DOWN
        | GamePad::BTN_LEFT
        | GamePad::BTN_RIGHT
        | GamePad::BTN_X
        | GamePad::BTN_Y
        | GamePad::BTN_A
        | GamePad::BTN_B
        ;

    if (gamePad.GetButtonDown() & controllerButton)
    {
        UseController = true;
    }

    Mouse& mouse = Input::Instance().GetMouse();
    float mouseLength = sqrtf((mouse.GetPositionX() - mouse.GetOldPositionX()) * (mouse.GetPositionX() - mouse.GetOldPositionX()) + (mouse.GetPositionY() - mouse.GetOldPositionY()) * (mouse.GetPositionY() - mouse.GetOldPositionY()));

    if (mouseLength > 0.0f)
    {
        UseController = false;
    }

    if (UseController)
    {
        if (gamePad.GetButtonDown() & GamePad::BTN_UP)
        {
            controllerPos.y = 540;
        }
        if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
        {
            controllerPos.y = 690;
        }

    }

    if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 450 && mouse.GetPositionY() < 600 && mouse.GetButtonDown() & Mouse::BTN_LEFT ||
        controllerPos.y == 540 && gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

    }


    if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 600 && mouse.GetPositionY() < 750 && mouse.GetButtonDown() & Mouse::BTN_LEFT ||
        controllerPos.y == 690 && gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        PostQuitMessage(1);
    }

    EnemyManager::Instance().Update(elapsedTime);
}



//�`�揈��
void SceneTitle::Render()
{

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �`�揈��
    RenderContext rc;
    rc.deviceContext = dc;
    LightManager::Instance().PushRenderContext(rc);

    // �J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());




    // 3D
    {
        particleShader->Begin(rc);
        particleShader->Draw(rc);
        particleShader->End(rc);

        //�V���h�E�}�b�v�̕`��
        shadowmapRenderer->Render(dc);

        //�V�[���̕`��
        sceneRenderer->SetShadowmapData(shadowmapRenderer->GetShadowMapData());
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


        if (!UseController)
        {
            Mouse& mouse = Input::Instance().GetMouse();
            if (mouse.GetPositionX() > 150 && mouse.GetPositionX() < 300 && mouse.GetPositionY() > 450 && mouse.GetPositionY() < 600)
            {
                Start->Render(dc,
                    125, 425,
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
                    125, 575,
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
        }
        else
        {
            if (controllerPos.y == 540)
            {
                Start->Render(dc,
                    125, 425,
                    200.0f, 200.0f,
                    0, 0,
                    static_cast<float>(Start->GetTextureWidth()), static_cast<float>(Start->GetTextureHeight()),
                    0,
                    1, 1, 1, alpha);

                A_Button->Render(dc,
                    300, 510,
                    80.0f, 80.0f,
                    0, 0,
                    static_cast<float>(A_Button->GetTextureWidth()), static_cast<float>(A_Button->GetTextureHeight()),
                    0,
                    1, 1, 1, 1);
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

            if (controllerPos.y == 690)
            {
                Quit->Render(dc,
                    125, 575,
                    200.0f, 200.0f,
                    0, 0,
                    static_cast<float>(Quit->GetTextureWidth()), static_cast<float>(Quit->GetTextureHeight()),
                    0,
                    1, 1, 1, alpha);

                A_Button->Render(dc,
                    300, 660,
                    80.0f, 80.0f,
                    0, 0,
                    static_cast<float>(A_Button->GetTextureWidth()), static_cast<float>(A_Button->GetTextureHeight()),
                    0,
                    1, 1, 1, 1);
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
        }

    }

#ifdef DEBUG    
    // �f�o�b�O���̕\��
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
#endif

}

//  ���f���������_���[�ɓo�^
void SceneTitle::RegisterRenderModel(Model* model)
{
    shadowmapRenderer->RegisterRenderModel(model);
    sceneRenderer->RegisterRenderModel(model);
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Material& material : resource->GetMaterials())
    {
        ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
        mat.shaderId = static_cast<int>(ModelShaderId::Phong);
    }
}

//  ���f���������_���[�ɍ폜
void SceneTitle::UnregisterRenderModel(Model* model)
{
    shadowmapRenderer->UnregisterRenderModel(model);
    sceneRenderer->UnregisterRenderModel(model);


}
