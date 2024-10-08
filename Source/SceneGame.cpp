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
#include "SearchAlgorithm.h"
#include "Graphics\LightManager.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "ProjectileStraight.h"
#include "ProjectileManager.h"
#include "PoisonZombie.h"
#include "ItemKey.h"
#include "ItemAmmo.h"

// ������
void SceneGame::Initialize()
{
    Graphics& graphics = Graphics::Instance();

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

    // �X�e�[�W������
    StageManager& stageManager = StageManager::Instance();

    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);


    if (stageMain->GetStageNum() == 2)
        player = std::make_unique<Player>(false);
    else
        player = std::make_unique<Player>(true);
    player->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(45), 0));
    gun = std::make_unique<Gun>();

    // �G�l�~�[������
    EnemyManager& enemyManager = EnemyManager::Instance();

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

    //case 1:
    //{
    //    PoisonZombie* poisonZombie = new PoisonZombie();
    //    poisonZombie->SetPosition(DirectX::XMFLOAT3(30.0f, 0.0f, 1.5f));
    //    enemyManager.Register(poisonZombie);

    //    Zombie* zombies[10];
    //    for (int i = 0; i < 10; ++i)
    //    {
    //        zombies[i] = new Zombie();
    //        enemyManager.Register(zombies[i]);
    //    }

    //    zombies[0]->SetPosition(DirectX::XMFLOAT3(38.0f, 0.0f, 26.0f));
    //    zombies[1]->SetPosition(DirectX::XMFLOAT3(20.0f, 0.0f, 25.0f));
    //    zombies[2]->SetPosition(DirectX::XMFLOAT3(6.0f, 0.0f, 25.0f));
    //    zombies[3]->SetPosition(DirectX::XMFLOAT3(8.0f, 0.0f, 18.0f));
    //    zombies[4]->SetPosition(DirectX::XMFLOAT3(25.0f, 0.0f, 9.0f));
    //    zombies[5]->SetPosition(DirectX::XMFLOAT3(34.0f, 0.0f, 9.0f));
    //    zombies[6]->SetPosition(DirectX::XMFLOAT3(49.0f, 0.0f, 13.5f));
    //    zombies[7]->SetPosition(DirectX::XMFLOAT3(15.0f, 0.0f, 15.0f));
    //    zombies[8]->SetPosition(DirectX::XMFLOAT3(36.0f, 0.0f, 1.5f));
    //    zombies[9]->SetPosition(DirectX::XMFLOAT3(33.0f, 0.0f, 1.5f));

    //    for (int i = 0; i < 10; ++i)
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
    //    items[0]->SetPosition(DirectX::XMFLOAT3(33.0f, 0.0f, 3.0f));
    //    items[1]->SetPosition(DirectX::XMFLOAT3(8.0f, 0.0f, 18.0f));
    //    items[2]->SetPosition(DirectX::XMFLOAT3(38.0f, 0.0f, 26.0f));
    //    break;
    //}
    //case 2:
    //{
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

    //	���f�����e�����_���[�ɓo�^
    if (stageMain->GetStageNum() == 0 || stageMain->GetStageNum() == 1)
    {
        Model* list[] =
        {
            player->model,
            stageMain->GetModel(),
            //gun->GetModel(),
            //enemyManager.GetEnemy(0)->GetModel(),
            //enemyManager.GetEnemy(1)->GetModel(),
            //enemyManager.GetEnemy(2)->GetModel(),
            //enemyManager.GetEnemy(3)->GetModel(),
            //enemyManager.GetEnemy(4)->GetModel(),
            //enemyManager.GetEnemy(5)->GetModel(),
            //enemyManager.GetEnemy(6)->GetModel(),
            //enemyManager.GetEnemy(7)->GetModel(),
            //enemyManager.GetEnemy(8)->GetModel(),
            //enemyManager.GetEnemy(9)->GetModel(),
            //enemyManager.GetEnemy(10)->GetModel(),
            //itemManager.GetItem(0)->GetModel(),
            //itemManager.GetItem(1)->GetModel(),
            //itemManager.GetItem(2)->GetModel(),
        };

        for (Model* model : list)
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
    }
    else
    {
        Model* list[] =
        {
            player->model,
            stageMain->GetModel(),
            gun->GetModel(),
        };

        for (Model* model : list)
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

    //// �J�����R���g���[���[������
    //cameraController = std::make_unique<CameraController>();

    ammo = std::make_unique<Sprite>("Data/Sprite/ammo.png");
    dead = std::make_unique<Sprite>("Data/Sprite/youdead.png");
    clear = std::make_unique<Sprite>("Data/Sprite/black.png");
    text1 = std::make_unique<Sprite>("Data/Font/font1.png");
    text2 = std::make_unique<Sprite>("Data/Font/font6.png");

    // �Q�[�W�X�v���C�g
    gauge = std::make_unique<Sprite>();
    crossHair = std::make_unique<Sprite>();

    // ���s������ǉ�
    {
        Light* light = new Light(LightType::Directional);
        light->SetDirection({ 1, -1, -1 });
        if (stageMain->GetStageNum() == 0 || stageMain->GetStageNum() == 1) light->SetColor({ 1,1,1,1 });
        else light->SetColor({ 1,0.4f,0.4f,1 });
        light->SetPosition({ 0,3,0 });
        LightManager::Instance().Register(light);
    }

    bgm = Audio::Instance().LoadAudioSource("Data/Audio/zombie.wav");
    heri = Audio::Instance().LoadAudioSource("Data/Audio/heri1.wav");
    bgm->Play(true);
}

// �I����
void SceneGame::Finalize()
{
    // �G�l�~�[�I����
    EnemyManager::Instance().Clear();

    ItemManager::Instance().Clear();

    // �X�e�[�W�I����
    StageManager::Instance().Clear();

    LightManager::Instance().Clear();


    shadowmapRenderer->ClearRenderModel();
    sceneRenderer->ClearRenderModel();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // �J�����R���g���[���[�X�V����
    Camera& camera = Camera::Instance();

    // �X�e�[�W�X�V����
    StageManager::Instance().Update(elapsedTime);

    //DirectX::XMVECTOR V = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&player->GetFront()), 27.08f);
    //DirectX::XMFLOAT3 v;
    //DirectX::XMStoreFloat3(&v, V);

    DirectX::XMFLOAT3 target = player->GetPosition();
    target.y += 0.5f;
    CameraController::Instance().SetTarget(target);
    CameraController::Instance().Update(elapsedTime);


    //cameraController->SetTarget(target);
    //cameraController->Update(elapsedTime);

    // �v���C���[�X�V����
    player->Update(elapsedTime);

    //if (player->GetHealth() <= 0)  deadAlpha += 0.015f;
    //if (deadAlpha >= 1.5f)  SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));


    // �e�X�V����
    gun->Update(elapsedTime);

    // �G�l�~�[�X�V����
    EnemyManager::Instance().Update(elapsedTime);

    ItemManager::Instance().Update(elapsedTime);

    //int enemyCount = EnemyManager::Instance().GetEnemyCount();
    //for (int i = 0; i < enemyCount; ++i)
    //{
    //    Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //    if (!enemy->GetAlive())
    //    {
    //        sceneRenderer->UnregisterRenderModel(enemy->GetModel());
    //        shadowmapRenderer->UnregisterRenderModel(enemy->GetModel());
    //    }
    //}

    //for (int i = 0; i < itemManager.GetItemCount(); ++i)
    //{
    //    Item* tem = itemManager.GetItem(i);

    //    if (!tem->OkDestroy()) continue;
    //    {
    //        LightManager::Instance().Remove(spotLights[i]);
    //        sceneRenderer->UnregisterRenderModel(tem->GetModel());
    //        shadowmapRenderer->UnregisterRenderModel(tem->GetModel());
    //    }
    //}

    // �G�t�F�N�g�X�V����
    EffectManager::Instance().Update(elapsedTime);
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



    // �V���h�E�}�b�v�̕`��
    shadowmapRenderer->Render(dc);

    // �V�[���̕`��
    sceneRenderer->SetShadowmapData(shadowmapRenderer->GetShadowMapData());
    sceneRenderer->Render(dc);

    postprocessingRenderer->Render(dc);

    // �`�揈��
    RenderContext rc;
    rc.deviceContext = dc;
    LightManager::Instance().PushRenderContext(rc);

    // �J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();




    //2D�X�v���C�g�`��
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

    // 2D�f�o�b�OGUI�`��
    {
         player->DrawDebugGUI();
         //EnemyManager::Instance().DrawDebugGUI();

         //StageManager::Instance().GetStage(0)->DrawDebugGUI();
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

// �G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    // �r���[�|�[�g
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // ���ׂĂ̓G�̓����HP�Q�[�W��\��
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMVECTOR enemyPosition = DirectX::XMVectorSet(enemy->GetPosition().x,
            enemy->GetPosition().y + (enemy->GetHeight()),
            enemy->GetPosition().z, 0.0f);

        // �͔͉�
        /*DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
        worldPosition.y += enemy->GetHeight();

        DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);*/

        // ���[���h���W����X�N���[�����W�֕ϊ�
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


