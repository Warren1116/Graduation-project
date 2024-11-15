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


//#define TUTORIAL
#define DEBUG

// ������
void SceneGame::Initialize()
{
    instance = this;
    Graphics& graphics = Graphics::Instance();

    //�V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
    {
        shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
    }

    // �C�x���g�X�N���v�g������
    EventScripter::Instance().Initialize();

    // �C�x���g�X�N���v�g�|�C���g�ǂݍ���
    EventPointManager::Instance().LoadPoints("Data/Script/EventScript.txt");


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
    player->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(45), 0));
    player->SetPosition({ 0,0,25 });
    // �G�l�~�[������
    EnemyManager& enemyManager = EnemyManager::Instance();
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // �X���C���i�X�e�[�g�}�V���p�j
    EnemyThief* people = new EnemyThief();
    people->SetPosition(DirectX::XMFLOAT3(10.0f, 0.0f, -25.0f));
    people->SetTerritory(people->GetPosition(), 10.0f);
    enemyManager.Register(people);

    ////	�ʐM����p�ɂP�C���₷
    EnemyThief* people2 = new EnemyThief();
    people2->SetPosition(DirectX::XMFLOAT3(3.0f, 0.0f, -25.0f));
    people2->SetTerritory(people2->GetPosition(), 10.0f);
    enemyManager.Register(people2);

    EnemyThief* people3 = new EnemyThief();
    people3->SetPosition(DirectX::XMFLOAT3(10.0f, 0.0f, -25.0f));
    people3->SetTerritory(people3->GetPosition(), 10.0f);
    enemyManager.Register(people3);

    //	���f�����e�����_���[�ɓo�^
    Model* list[] =
    {
        player->model.get(),
        stageMain->GetModel(),
        people->GetModel(),
        people2->GetModel(),
        people3->GetModel(),

    };
    for (Model* model : list)
    {
        RegisterRenderModel(model);

    }


    // �L�����N�^�[��������
    CharacterManager& characterManager = CharacterManager::Instance();
    {
        // �v���C���[
        characterManager.Register(player.get());

        // �J�����R���g���[���[������
        cameraController = std::make_unique<CameraController>();

        // �G�l�~�[������
        characterManager.Register(people);
        characterManager.Register(people2);
        characterManager.Register(people3);


    }


    // ���s������ǉ�
    {
        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetDirection({ -0.5, -1, -1 });
        mainDirectionalLight->SetColor({ 1,1,1,1 });
        mainDirectionalLight->SetPosition({ 0,3,0 });
        LightManager::Instance().Register(mainDirectionalLight);
        LightManager::Instance().SetShadowmapLight(mainDirectionalLight);
    }

    meta = make_unique<Meta>(player.get(), &enemyManager);

    //  UI�̏�����
    UI::Instance().Initialize();

    // HUD����
    headUpDisplay = new HeadUpDisplay();


}

// �I����
void SceneGame::Finalize()
{
    // HUD�I����
    if (headUpDisplay != nullptr)
    {
        delete headUpDisplay;
        headUpDisplay = nullptr;
    }

    // �G�l�~�[�I����
    EnemyManager::Instance().Clear();

    // �X�e�[�W�I����
    StageManager::Instance().Clear();

    LightManager::Instance().Clear();
    EffectManager::Instance().Finalize();
    instance = nullptr;

    shadowmapRenderer->ClearRenderModel();
    sceneRenderer->ClearRenderModel();

    mainDirectionalLight = nullptr;
    //  UI�I����
    UI::Instance().Clear();

    // �C�x���g�X�N���v�g�|�C���g�N���A
    EventPointManager::Instance().Clear();

    // �C�x���g�X�N���v�g�I����
    EventScripter::Instance().Finalize();

}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
    // �C�x���g�X�N���v�g������
    EventScripter::Instance().Update(elapsedTime);


    Graphics& graphics = Graphics::Instance();
    //  UI�X�V����
    UI::Instance().Update(elapsedTime);


#ifdef TUTORIAL
    //  �`���g���A���̃^�C�}�[
    tutorialTimer += elapsedTime;

    if (tutorialTimer > 5.0f && tutorialState == SceneGame::TutorialState::First)
    {
        StartTutorial(SceneGame::TutorialState::Move);
    }

    UpdateTutorialState(elapsedTime);

    if (isPaused)
    {
        return;
    }
#endif // TUTORIAL

    // �J�����R���g���[���[�X�V����
    Camera& camera = Camera::Instance();
    cameraController->Update(elapsedTime);


    // �X�e�[�W�X�V����
    StageManager::Instance().Update(elapsedTime);

    // �v���C���[�X�V����
    player->Update(elapsedTime);

    // �G�l�~�[�X�V����
    EnemyManager::Instance().Update(elapsedTime);

    // �G�t�F�N�g�X�V����
    EffectManager::Instance().Update(elapsedTime);


    // HUD�X�V
    headUpDisplay->Update(elapsedTime);


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
    // �V���h�E�}�b�v�̕`��
    shadowmapRenderer->Render(rc.deviceContext);

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

        headUpDisplay->Render(dc);

    }

#ifdef DEBUG
    // 2D�f�o�b�OGUI�`��
    {
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

    switch (tutorialState)
    {
    case SceneGame::TutorialState::Move:
        CheckTutorialTimeout(0.0f);
        if (Player::Instance().InputMove(elapsedTime) && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Jump);
        break;

    case SceneGame::TutorialState::Jump:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputJump() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Attack);
        break;

    case SceneGame::TutorialState::Attack:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputAttack() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Shot);
        break;

    case SceneGame::TutorialState::Shot:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputProjectile() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::CameraLock);
        break;

    case SceneGame::TutorialState::CameraLock:
        CheckTutorialTimeout(3.5f);
        if (gamePad.GetButtonDown() & GamePad::BTN_TAB && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::LockAttack);
        break;

    case SceneGame::TutorialState::LockAttack:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputAttack() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::LockShot);
        break;

    case SceneGame::TutorialState::LockShot:
        CheckTutorialTimeout(3.5f);
        if (Player::Instance().InputProjectile() && canAcceptInput)
            AdvanceTutorialState(SceneGame::TutorialState::Swing);
        break;
    case SceneGame::TutorialState::Swing:
        tutorialTimer = 0;
        break;
    case SceneGame::TutorialState::Climb:
        tutorialTimer = 0;
        break;
    case SceneGame::TutorialState::Finish:
        tutorialTimer = 0;
        break;
    }
}

void SceneGame::StartTutorial(TutorialState newState)
{
    PauseGame();
    tutorialState = newState;
    tutorialTimer = 0;
}

void SceneGame::AdvanceTutorialState(TutorialState newState)
{
    UnpauseGame();
    tutorialState = newState;
    tutorialTimer = 0;
}

void SceneGame::CheckTutorialTimeout(float timeout)
{
    if (tutorialTimer > timeout)
    {
        PauseGame();
        canAcceptInput = true;
    }
}

void SceneGame::UnpauseGame()
{
    isPaused = false;
    canAcceptInput = false;
}

void SceneGame::PauseGame()
{
    isPaused = true;
}

//  ���f���������_���[�ɓo�^
void SceneGame::RegisterRenderModel(Model* model)
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
void SceneGame::UnregisterRenderModel(Model* model)
{
    shadowmapRenderer->UnregisterRenderModel(model);
    sceneRenderer->UnregisterRenderModel(model);

}


