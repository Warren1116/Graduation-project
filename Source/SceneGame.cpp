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
#include "UI.h"

#include "Graphics/geometric_primitive.h"
#include "EventScripter.h"
#include "EventPointManager.h"

SceneGame* SceneGame::instance = nullptr;
//�V���h�E�}�b�v�̃T�C�Y
static const UINT SHADOWMAP_SIZE = 2048;


#define TUTORIAL
//#define DEBUG

// ������
void SceneGame::Initialize()
{
    instance = this;
    Graphics& graphics = Graphics::Instance();

    //�V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
    {

        //shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
        for (auto& it : shadowmapDepthStencil)
        {
            it = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
        }
    }

    // �C�x���g�X�N���v�g������
    EventScripter::Instance().Initialize();

    // �C�x���g�X�N���v�g�|�C���g�ǂݍ���
    //EventPointManager::Instance().LoadPoints("Data/Script/EventScript.txt");


    //	�e�탌���_���[����
    {
        UINT width = static_cast<UINT>(graphics.GetScreenWidth());
        UINT height = static_cast<UINT>(graphics.GetScreenHeight());

        //	�V���h�E�}�b�v�����_���[
        shadowmapRenderer = std::make_unique<ShadowmapRenderer>(2048);
        //shadowmapCasterRenderer = std::make_unique<ShadowmapCasterRenderer>(2048);
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

    // �X�e�[�W������
    StageManager& stageManager = StageManager::Instance();

    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);

    //�@�v���C���[����
    player = std::make_unique<Player>(true);
    player->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(-90), 0));
    player->SetPosition({ 66,0,42 });
    //player->SetPosition({ 0,0,0 });
    player->SetIdleState();

    // �G�l�~�[������
    EnemyManager& enemyManager = EnemyManager::Instance();
    ProjectileManager& projectileManager = ProjectileManager::Instance();



    // �G��������
    StartNextWave();



    //	���f�����e�����_���[�ɓo�^
    Model* list[] =
    {
        player->model.get(),
        stageMain->GetCityModel(),
        stageMain->GetGroundModel(),


    };
    for (Model* model : list)
    {
        RegisterRenderModel(model);
    }


    //// �L�����N�^�[��������
    CharacterManager& characterManager = CharacterManager::Instance();
    {
        // �v���C���[
        characterManager.Register(player.get());

        // �J�����R���g���[���[������
        cameraController = std::make_unique<CameraController>();

        ////// �G�l�~�[������
        //for (int i = 0; i < enemyManager.GetEnemyCount(); i++) {
        //    characterManager.Register(enemyManager.GetEnemy(i));
        //}

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

    meta = make_unique<Meta>(player.get(), &enemyManager);

    Pause = std::make_unique<Sprite>();
    PauseBackGround = std::make_unique<Sprite>();
    ToTitle = std::make_unique<Sprite>("Data/Sprite/ToTitle.png");
    ControlWay = std::make_unique<Sprite>("Data/Sprite/Control Way.png");
    ControllerPicture = std::make_unique<Sprite>("Data/Sprite/Controller1.png");
    ControllerFont = std::make_unique<Sprite>("Data/Sprite/Controller_Font.png");
    KeyBoardPicture = std::make_unique<Sprite>("Data/Sprite/KeyBoard.png");
    KeyBoardFont = std::make_unique<Sprite>("Data/Sprite/KeyBoard_Font.png");


    //  UI�̏�����
    UI::Instance().Initialize();

    // HUD����
    //headUpDisplay = new HeadUpDisplay();

    UseController = false;
    controllerPos = { 245,540 };

}

// �I����
void SceneGame::Finalize()
{
    // HUD�I����
    //if (headUpDisplay != nullptr)
    //{
    //    delete headUpDisplay;
    //    headUpDisplay = nullptr;
    //}

    // �G�l�~�[�I����
    EnemyManager::Instance().Clear();

    // �X�e�[�W�I����
    StageManager::Instance().Clear();

    LightManager::Instance().Clear();
    EffectManager::Instance().Finalize();
    instance = nullptr;

    //shadowmapCasterRenderer->ClearRenderModel();
    shadowmapRenderer->ClearRenderModel();
    sceneRenderer->ClearRenderModel();

    mainDirectionalLight = nullptr;
    //  UI�I����
    UI::Instance().Clear();

    //// �C�x���g�X�N���v�g�|�C���g�N���A
    //EventPointManager::Instance().Clear();

    //// �C�x���g�X�N���v�g�I����
    //EventScripter::Instance().Finalize();

}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    CheckWaveClear();

    //// �C�x���g�X�N���v�g������
    //EventScripter::Instance().Update(elapsedTime);

    //  UI�X�V����
    UI::Instance().Update(elapsedTime);

    if (increasingAlpha)
    {
        alpha += alphaSpeed * 0.7f;
        if (alpha >= 1.0f)
        {
            alpha = 1.0f;
            increasingAlpha = false;
        }
    }
    else
    {
        alpha -= alphaSpeed * 0.7f;
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


    //Pause�̏���
    if (gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_ESC || gamePad.GetButtonDown() & GamePad::BTN_START)
    {
        isPaused = !isPaused;
    }
    if (isPaused)
    {
        if (UseController)
        {
            if (gamePad.GetButtonDown() & GamePad::BTN_UP)
            {
                controllerPos.y = 300;
            }
            if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
            {
                controllerPos.y = 490;
            }

        }

        Mouse& mouse = Input::Instance().GetMouse();
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());

        if (mouse.GetPositionX() > screenWidth * 0.1f && mouse.GetPositionX() < screenWidth * 0.1f + 250
            && mouse.GetPositionY() > screenHeight * 0.2f + 50 && mouse.GetPositionY() < screenHeight * 0.2f + 170
            && mouse.GetButtonDown() & Mouse::BTN_LEFT ||
            controllerPos.y == 300 && gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            isTutorial = !isTutorial;
            ControllerButton = true;
            KeyBoardButton = false;
        }

        if (mouse.GetPositionX() > screenWidth * 0.1f && mouse.GetPositionX() < screenWidth * 0.1f + 300
            && mouse.GetPositionY() > screenHeight * 0.4f + 50 && mouse.GetPositionY() < screenHeight * 0.4f + 170
            && mouse.GetButtonDown() & Mouse::BTN_LEFT ||
            controllerPos.y == 490 && gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            SceneManager::Instance().ChangeScene(new SceneTitle);
        }

        if (isTutorial)
        {
            if (mouse.GetPositionX() > screenWidth * 0.35f && mouse.GetPositionX() < screenWidth * 0.35f + 250
                && mouse.GetPositionY() > screenHeight * 0.1f + 50 && mouse.GetPositionY() < screenHeight * 0.1f + 170
                && mouse.GetButtonDown() & Mouse::BTN_LEFT ||
                UseController && gamePad.GetButtonDown() & GamePad::BTN_LEFT)
            {
                ControllerButton = true;
                KeyBoardButton = false;
            }

            if (mouse.GetPositionX() > screenWidth * 0.65f && mouse.GetPositionX() < screenWidth * 0.65f + 300
                && mouse.GetPositionY() > screenHeight * 0.1f + 50 && mouse.GetPositionY() < screenHeight * 0.1f + 170
                && mouse.GetButtonDown() & Mouse::BTN_LEFT ||
                UseController && gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
            {
                KeyBoardButton = true;
                ControllerButton = false;
            }


        }

        return;
    }
#ifdef TUTORIAL
    //  �`���g���A���̃^�C�}�[
    tutorialTimer += elapsedTime;

    if (tutorialTimer > 5.0f && tutorialState == SceneGame::TutorialState::First)
    {
        StartTutorial(SceneGame::TutorialState::Move);
    }

    UpdateTutorialState(elapsedTime);

    if (tutorialPause)
    {
        return;
    }
#endif // TUTORIAL

    // �X�e�[�W�X�V����
    StageManager::Instance().Update(elapsedTime);

    // �v���C���[�X�V����
    player->Update(elapsedTime);

    // �G�l�~�[�X�V����
    EnemyManager::Instance().Update(elapsedTime);

    // �G�t�F�N�g�X�V����
    EffectManager::Instance().Update(elapsedTime);

    //// HUD�X�V
    //headUpDisplay->Update(elapsedTime);

    // �J�����R���g���[���[�X�V����
    Camera& camera = Camera::Instance();

    cameraController->Update(elapsedTime);


#ifdef DEBUG
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_ESC)
    //{
    //    isPaused = !isPaused;
    //}

    if (gamePad.GetButton() & GamePad::BTN_KEYBOARD_SHIFT && gamePad.GetButton() & GamePad::BTN_KEYBOARD_R)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
    //if (isPaused)
    //{
    //    return;
    //}
#endif // 

}

// �`�揈��
void SceneGame::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
    dc->OMSetRenderTargets(1, &rtv, dsv);
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // �`�揈��
    RenderContext rc;
    rc.deviceContext = dc;
    rc.shadowMapData = shadowmapRenderer->GetShadowMapData();
    //rc.shadowMapData = shadowmapCasterRenderer->GetShadowMapData();

    // �V���h�E�}�b�v�̕`��
    shadowmapRenderer->Render(rc.deviceContext);
    //shadowmapCasterRenderer->Render(rc.deviceContext);

    // �V�[���̕`��
    sceneRenderer->SetShadowmapData(rc.shadowMapData);
    sceneRenderer->Render(rc.deviceContext);

    postprocessingRenderer->Render(rc.deviceContext);

    LightManager::Instance().PushRenderContext(rc);

    // �J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    //2D�X�v���C�g�`��
    {
        UI::Instance().DrawUI(dc, rc.view, rc.projection);

        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());

        GamePad& gamePad = Input::Instance().GetGamePad();
        if (isPaused)
        {
            //  �|�[�Y�w�i�F�̕`��
            Pause->Render(dc, 0, 0, screenWidth, screenHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0.5f);


            Mouse& mouse = Input::Instance().GetMouse();
            //  ������@�{�^���̕`��
            if (mouse.GetPositionX() > screenWidth * 0.1f && mouse.GetPositionX() < screenWidth * 0.1f + 250
                && mouse.GetPositionY() > screenHeight * 0.2f + 50 && mouse.GetPositionY() < screenHeight * 0.2f + 170 || 
                UseController && controllerPos.y == 300)
            {
                ControlWay->Render(dc, screenWidth * 0.1f, screenHeight * 0.2f, 350, 350, 0, 0, static_cast<float>(ControlWay->GetTextureWidth()), static_cast<float>(ControlWay->GetTextureHeight())
                    , 0, 1, 1, 1, 1);
            }
            else
            {
                ControlWay->Render(dc, screenWidth * 0.1f, screenHeight * 0.2f, 250, 250, 0, 0, static_cast<float>(ControlWay->GetTextureWidth()), static_cast<float>(ControlWay->GetTextureHeight())
                    , 0, 1, 1, 1, 1);
            }

            //  �^�C�g����ʂ֖߂�{�^���̕`��
            if (mouse.GetPositionX() > screenWidth * 0.1f && mouse.GetPositionX() < screenWidth * 0.1f + 300
                && mouse.GetPositionY() > screenHeight * 0.4f + 50 && mouse.GetPositionY() < screenHeight * 0.4f + 170 ||
                UseController && controllerPos.y == 490)
            {
                ToTitle->Render(dc, screenWidth * 0.1f, screenHeight * 0.4f, 350, 350, 0, 0, static_cast<float>(ToTitle->GetTextureWidth()), static_cast<float>(ToTitle->GetTextureHeight())
                    , 0, 1, 1, 1, 1);
            }
            else
            {
                ToTitle->Render(dc, screenWidth * 0.1f, screenHeight * 0.4f, 250, 250, 0, 0, static_cast<float>(ToTitle->GetTextureWidth()), static_cast<float>(ToTitle->GetTextureHeight())
                    , 0, 1, 1, 1, 1);
            }

            //  ������@�{�^����
            if (isTutorial)
            {
                //  ������@�g�̕`��
                PauseBackGround->Render(dc, 450, 170, 950, 600, 0, 0, 0, 0, 0, 0.2f, 0.4f, 0.8f, 0.5f);

                //  �R���g���[���[�����̕`��
                if (ControllerButton)
                {
                    ControllerFont->Render(dc, screenWidth * 0.35f, screenHeight * 0.1f, 350, 350, 0, 0, static_cast<float>(ControllerFont->GetTextureWidth()), static_cast<float>(ControllerFont->GetTextureHeight())
                        , 0, 1, 1, 1, alpha);

                    ControllerPicture->Render(dc,
                        screenWidth * 0.35f, screenHeight * 0.25f, 750, 650, 0, 0, static_cast<float>(ControllerPicture->GetTextureWidth()), static_cast<float>(ControllerPicture->GetTextureHeight())
                        , 0, 1, 1, 1, 1);
                }
                else
                {
                    ControllerFont->Render(dc, screenWidth * 0.35f, screenHeight * 0.1f, 250, 250, 0, 0, static_cast<float>(ControllerFont->GetTextureWidth()), static_cast<float>(ControllerFont->GetTextureHeight())
                        , 0, 1, 1, 1, 1);
                }

                //  �L�[�{�[�h�����̕`��
                if (KeyBoardButton)
                {
                    KeyBoardFont->Render(dc, screenWidth * 0.65f, screenHeight * 0.1f, 350, 350, 0, 0, static_cast<float>(KeyBoardFont->GetTextureWidth()), static_cast<float>(KeyBoardFont->GetTextureHeight())
                        , 0, 1, 1, 1, alpha);

                    KeyBoardPicture->Render(dc,
                        screenWidth * 0.35f, screenHeight * 0.32f, 870, 480, 0, 0, static_cast<float>(KeyBoardPicture->GetTextureWidth()), static_cast<float>(KeyBoardPicture->GetTextureHeight())
                        , 0, 1, 1, 1, 1);
                }
                else
                {
                    KeyBoardFont->Render(dc, screenWidth * 0.65f, screenHeight * 0.1f, 250, 250, 0, 0, static_cast<float>(KeyBoardFont->GetTextureWidth()), static_cast<float>(KeyBoardFont->GetTextureHeight())
                        , 0, 1, 1, 1, 1);
                }

            }

        }

        //headUpDisplay->Render(dc);

    }

#ifdef DEBUG
    // 2D�f�o�b�OGUI�`��
    {
        //headUpDisplay->DrawDebugGUI();


        player->DrawDebugGUI();
        EnemyManager::Instance().DrawDebugGUI();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);


        if (ImGui::Begin("Tutorial", nullptr, ImGuiWindowFlags_None))
        {
            ImGui::InputFloat("Tutorial", &tutorialTimer);
            std::string str = "";
            switch (tutorialState)
            {
            case TutorialState::Move:
                str = "Tutorial Move";
                break;
            case TutorialState::Jump:
                str = "Tutorial Jump";
                break;
            case TutorialState::Attack:
                str = "Tutorial Attack";
                break;
            case TutorialState::Shot:
                str = "Tutorial Shot";
                break;
            case TutorialState::CameraLock:
                str = "Tutorial CameraLock";
                break;
            case TutorialState::LockAttack:
                str = "Tutorial LockAttack";
                break;
            case TutorialState::LockShot:
                str = "Tutorial LockShot";
                break;
            case TutorialState::Swing:
                str = "Tutorial Swing";
                break;
            case TutorialState::Finish:
                str = "Tutorial Finish";
                break;
            }
            ImGui::Text(u8"State�@%s", str.c_str());

        }
        ImGui::End();

    }

    // �f�o�b�O���̕\��
    {
        ImGui::Separator();
        LightManager::Instance().DrawDebugGUI();
        ImGui::Separator();
        shadowmapRenderer->DrawDebugGUI();
        //shadowmapCasterRenderer->DrawDebugGUI();
        ImGui::Separator();
        sceneRenderer->DrawDebugGUI();
        ImGui::Separator();
        postprocessingRenderer->DrawDebugGUI();

    }
#endif // DEBUG


}

void SceneGame::UpdateTutorialState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //  �`���g���A���̐i�s
    switch (tutorialState)
    {
        //  �ړ��̃`���g���A��
    case SceneGame::TutorialState::Move:
        CheckTutorialTimeout(0.0f);
        if (Player::Instance().InputMove(elapsedTime) && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Jump);
        break;
        //  �W�����v�̃`���g���A��
    case SceneGame::TutorialState::Jump:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputJump() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Attack);
        break;
        //  �U���̃`���g���A��
    case SceneGame::TutorialState::Attack:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputAttack() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Shot);
        break;
        //  �e�ۂ̃`���g���A��
    case SceneGame::TutorialState::Shot:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputProjectile() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::CameraLock);
        break;
        //  �J�������b�N�̃`���g���A��
    case SceneGame::TutorialState::CameraLock:
        CheckTutorialTimeout(3.5f);
        if (gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_TAB || gamePad.GetButtonDown() & GamePad::BTN_RIGHT_THUMB && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::LockAttack);
        break;
        //  ���b�N�I���U���̃`���g���A��
    case SceneGame::TutorialState::LockAttack:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputAttack() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::LockShot);
        break;
        //  ���b�N�I���e�ۂ̃`���g���A��
    case SceneGame::TutorialState::LockShot:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputProjectile() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Swing);
        break;
        //  �X�C���O�̃`���g���A��
    case SceneGame::TutorialState::Swing:
        tutorialTimer = 0;
        break;
        //  �N���C�~���O�̃`���g���A��
    case SceneGame::TutorialState::Climb:
        tutorialTimer = 0;
        break;
        //  �`���g���A���I��
    case SceneGame::TutorialState::Finish:
        tutorialTimer = 0;
        break;
    }
}

//  �`���g���A���n�܂鎞�ɂ̐ݒ�
void SceneGame::StartTutorial(TutorialState newState)
{
    PauseGame();
    tutorialState = newState;
    tutorialTimer = 0;
}

//  �`���g���A���X�e�[�g�̕ύX
void SceneGame::AdvanceTutorialState(TutorialState newState)
{
    UnpauseGame();
    tutorialState = newState;
    tutorialTimer = 0;
}

//  �`���g���A���̃^�C�}�[�`�F�b�N
void SceneGame::CheckTutorialTimeout(float timeout)
{
    if (tutorialTimer > timeout)
    {
        PauseGame();
        canAcceptInput = true;
    }
}

//  �`���g���A���̍ĊJ
void SceneGame::UnpauseGame()
{
    tutorialPause = false;
    canAcceptInput = false;
}

//  �`���g���A���̈ꎞ��~
void SceneGame::PauseGame()
{
    tutorialPause = true;
}

//  ���f���������_���[�ɓo�^
void SceneGame::RegisterRenderModel(Model* model)
{
    shadowmapRenderer->RegisterRenderModel(model);
    //shadowmapCasterRenderer->RegisterRenderModel(model);
    sceneRenderer->RegisterRenderModel(model);
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Material& material : resource->GetMaterials())
    {
        ModelResource::Material& mat = const_cast<ModelResource::Material&>(material);
        mat.shaderId = static_cast<int>(ModelShaderId::Phong);
    }
}

//  ���f���������_���[�ɍ폜
void SceneGame::UnregisterRenderModel(Model* model)
{
    shadowmapRenderer->UnregisterRenderModel(model);
    //shadowmapCasterRenderer->UnregisterRenderModel(model);
    sceneRenderer->UnregisterRenderModel(model);

}


//  Wave�ɂ���ēG�𐶐�
void SceneGame::SpawnEnemiesForWave(int wave)
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //    �G�̐���
    switch (wave)
    {
    case 1:
    {
        // �G��������
        EnemyThief* thief = new EnemyThief();
        thief->SetPosition(DirectX::XMFLOAT3(40.0f, 0.0f, 40.0f));
        thief->SetTerritory(thief->GetPosition(), 10.0f);
        thief->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(0), 0));
        EnemyManager::Instance().Register(thief);
        ////	�ʐM����p�ɂP�C���₷
        EnemyThief* thief2 = new EnemyThief();
        thief2->SetPosition(DirectX::XMFLOAT3(41.0f, 0.0f, 39.0f));
        thief2->SetTerritory(thief2->GetPosition(), 10.0f);
        thief2->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(90), 0));
        EnemyManager::Instance().Register(thief2);

        //	���f�����e�����_���[�ɓo�^
        Model* list[] =
        {
            thief->GetModel(),
            thief2->GetModel(),

        };
        for (Model* model : list)
        {
            RegisterRenderModel(model);
        }

        // �L�����N�^�[��������
        CharacterManager& characterManager = CharacterManager::Instance();
        {
            characterManager.Register(thief);
            characterManager.Register(thief2);

        }
        //RegisterEnemies({ thief, thief2 });

    }
    break;
    case 2:
    {
        EnemyThief* thief = new EnemyThief();
        thief->SetPosition(DirectX::XMFLOAT3(40.0f, 0.0f, 40.0f));
        thief->SetTerritory(thief->GetPosition(), 10.0f);
        thief->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(0), 0));
        EnemyManager::Instance().Register(thief);

        //	���f�����e�����_���[�ɓo�^
        Model* list[] =
        {
            thief->GetModel(),

        };
        for (Model* model : list)
        {
            RegisterRenderModel(model);
        }

        // �L�����N�^�[��������
        CharacterManager& characterManager = CharacterManager::Instance();
        {
            characterManager.Register(thief);

        }

        //RegisterEnemies({ thief });
    }
    break;
    case 3:
    {
        EnemyThief* thief = new EnemyThief();
        thief->SetPosition(DirectX::XMFLOAT3(40.0f, 0.0f, 40.0f));
        thief->SetTerritory(thief->GetPosition(), 10.0f);
        thief->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(0), 0));
        EnemyManager::Instance().Register(thief);
        //	���f�����e�����_���[�ɓo�^
        Model* list[] =
        {
            thief->GetModel(),

        };
        for (Model* model : list)
        {
            RegisterRenderModel(model);
        }

        // �L�����N�^�[��������
        CharacterManager& characterManager = CharacterManager::Instance();
        {
            characterManager.Register(thief);

        }
    }

    break;

    }

}

//  ����Wave���J�n
void SceneGame::StartNextWave()
{
    if (currentWave < totalWaves)
    {
        currentWave++;
        waveInProgress = true;
        SpawnEnemiesForWave(currentWave);
    }


}

//  Wave���N���A���ꂽ���ǂ����̃`�F�b�N
void SceneGame::CheckWaveClear()
{
    if (waveInProgress && EnemyManager::Instance().GetEnemyCount() == 0)
    {
        waveInProgress = false;
        StartNextWave();
    }
}



