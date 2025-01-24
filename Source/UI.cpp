#include "UI.h"
#include "Player.h"
#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\GamePad.h"
#include "Input\Input.h"
#include "EnemyManager.h"

#define TUTORIAL

void UI::Initialize()
{
    // �摜�ǂݍ���
    LockOnScope = std::make_unique<Sprite>("Data/Sprite/LockOnScope.png");
    TutorialMove = std::make_unique<Sprite>("Data/Sprite/Tutorial_Move.png");
    TutorialJump = std::make_unique<Sprite>("Data/Sprite/Tutorial_Jump.png");
    TutorialAttack = std::make_unique<Sprite>("Data/Sprite/Tutorial_Attack.png");
    TutorialShot = std::make_unique<Sprite>("Data/Sprite/Tutorial_Shot.png");
    TutorialCameraLock = std::make_unique<Sprite>("Data/Sprite/Tutorial_CameraLock.png");
    TutorialGrab = std::make_unique<Sprite>("Data/Sprite/Tutorial_Grab.png");
    TutorialDodge = std::make_unique<Sprite>("Data/Sprite/Tutorial_Dodge.png");
    TutorialSwing = std::make_unique<Sprite>("Data/Sprite/Tutorial_Swing.png");
    TextFont = std::make_unique<Sprite>("Data/Font/font4.png");
    Font = std::make_unique<Sprite>("Data/Sprite/Font.png");
    Font2 = std::make_unique<Sprite>("Data/Sprite/Font2.png");
    SpiderSense = std::make_unique<Sprite>("Data/Sprite/SpiderSense.png");
    SpiderSense2 = std::make_unique<Sprite>("Data/Sprite/SpiderSense2.png");
    FocusingLineB = std::make_unique<Sprite>("Data/Sprite/FocusingLineBlack.png");
    FocusingLineW = std::make_unique<Sprite>("Data/Sprite/FocusingLineWhite.png");
    HpBox = std::make_unique<Sprite>("Data/Sprite/HpBox1.png");
    Number = std::make_unique<Sprite>("Data/Sprite/Numbers.png");
    WaveCounterBox = std::make_unique<Sprite>(("Data/Sprite/WaveCounterBox.png"));
    HpBar = std::make_unique<Sprite>();
    SkillBar = std::make_unique<Sprite>();

}

void UI::Update(float elapsedtime)
{
    if (SceneGame::Instance().GetIsTutorialPaused() || Player::Instance().GetOnSwing())
    {
        if (increasingAlpha)
        {
            alpha += alphaSpeed;
            if (alpha >= 1.0f)
            {
                alpha = 1.0f;
                increasingAlpha = false;
            }
        }
        else
        {
            alpha -= alphaSpeed;
            if (alpha <= 0.3f)
            {
                alpha = 0.3f;
                increasingAlpha = true;
            }
        }

    }
    else
    {
        if (fadeOut)
        {
            alpha += alphaSpeed;
            alphaColor -= alphaSpeed;
            if (alpha >= 1.0f)
            {
                alpha = 1.0f;
                alphaColor = 0.3f;
                fadeOut = false;
            }
        }
        else
        {
            alpha -= alphaSpeed;
            alphaColor += alphaSpeed;
            if (alpha <= 0.3f)
            {
                alphaColor = 1.0f;
                alpha = 0.3f;
                fadeOut = true;
            }
        }
    }

}


void UI::DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //  �U�����󂯂鎞��SpiderSense��`�悷��
    if (Player::Instance().GetAttackSoon())
    {
        RenderSpiderSense(dc, view, projection);
    }

    //  �G����e�����󂯂鎞�ɑ_����
    if (Player::Instance().GetShotSoon())
    {
        LineRenderer* lineRender = Graphics::Instance().GetLineRenderer();

        auto enemyModel = Player::Instance().GetAttackEnemy()->GetModel();
        Model::Node* GunPos = enemyModel->FindNode("slide");

        DirectX::XMFLOAT3 attackEnemyPos = Player::Instance().GetAttackEnemy()->GetPosition();
        DirectX::XMFLOAT3 position = Player::Instance().GetPosition();

        lineRender->DrawLine({ GunPos->worldTransform._41 ,GunPos->worldTransform._42 ,GunPos->worldTransform._43 },
            DirectX::XMFLOAT3(position.x, position.y + 1.0f, position.z),
            DirectX::XMFLOAT4(1, 0, 0, 1));
    }

    // �@Wave�J�E���^�[�̕`��
    RenderWaveCounter(dc, view, projection);

    //  HP�o�[��Skill�o�[�̕`��
    RenderHpBar(dc, view, projection);

    //�@�W�����̕`��
    RenderFocusingLine(dc, view, projection);

#ifdef TUTORIAL
    //  �`���g���A���̕`��
    RenderTutorial(dc, view, projection);
#endif // !TUTORIAL


}

void UI::RenderSpiderSense(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


    // �v���C���[�̓��̈ʒu���擾
    Model::Node* PlayerHeadPos = Player::Instance().model->FindNode("mixamorig:HeadTop_End");
    DirectX::XMVECTOR playerHeadPos = DirectX::XMVectorSet(
        PlayerHeadPos->worldTransform._41,
        PlayerHeadPos->worldTransform._42,
        PlayerHeadPos->worldTransform._43,
        1.0f);

    // 3D���W��2D���W�ɕϊ�
    DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
        playerHeadPos,
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

    SpiderSense->Render(dc,
        screenPosition.x - 55.0f, screenPosition.y - 40.0f,
        120.0f, 75.0f,
        0, 0,
        static_cast<float>(SpiderSense->GetTextureWidth()), static_cast<float>(SpiderSense->GetTextureHeight()),
        0,
        1, 1, 1, alpha);

    SpiderSense2->Render(dc,
        screenPosition.x - 55.0f, screenPosition.y - 40.0f,
        120.0f, 75.0f,
        0, 0,
        static_cast<float>(SpiderSense2->GetTextureWidth()), static_cast<float>(SpiderSense2->GetTextureHeight()),
        0,
        1, 1, 1, alphaColor);

}


void UI::RenderTutorialSprite(std::unique_ptr<Sprite>& sprite, ID3D11DeviceContext* dc, float screenWidth, float screenHeight, float alpha, bool isController, float offsetX, float offsetY)
{
    float x = screenWidth + offsetX;
    float y = screenHeight + offsetY;
    float width = 250.0f;
    float height = 250.0f;
    float srcX = isController ? sprite->GetTextureWidth() * 0.5f : 0.0f;
    float srcY = 0.0f;
    float srcWidth = sprite->GetTextureWidth() * 0.5f;
    float srcHeight = sprite->GetTextureHeight();

    sprite->Render(dc, x, y, width, height, srcX, srcY, srcWidth, srcHeight, 0, 1, 1, 1, alpha);
}

void UI::RenderWaveCounter(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();

    // �E�F�[�u�����擾
    int wave = SceneGame::Instance().GetCurrentWave();
    // �ő�E�F�[�u�����擾
    int totalWaves = SceneGame::Instance().GetTotalWaves();
    // �E�F�[�u���̕�����
    std::string waveText = "Wave " + std::to_string(wave) + "/" + std::to_string(totalWaves);

    // ���̃E�F�[�u�̕�����\��
    if (SceneGame::Instance().IsNextWave())
    {
        WaveCounterBox->Render(dc,
            screenWidth * 0.06f, screenHeight * 0.215f,
            180, 30,
            0, 0,
            static_cast<float>(WaveCounterBox->GetTextureWidth()), static_cast<float>(WaveCounterBox->GetTextureHeight()),
            0,
            1.0f, 1.0f, 1.0f, 0.3f);
        TextFont->textout(dc, waveText, screenWidth * 0.08f, screenHeight * 0.22f, 15, 18, { 1,1,1,1 });


        if (wave > 1)
        {
            waveText = "Next Wave";
        }
        else if (wave == 1)
        {
            waveText = "Start Wave";
        }
        else if (wave == 0)
        {
            waveText = "Tutorial";
        }


        WaveCounterBox->Render(dc,
            screenWidth * 0.5f - static_cast<float>(WaveCounterBox->GetTextureWidth()) * 0.5f, screenHeight * 0.1f,
            250, 65,
            0, 0,
            static_cast<float>(WaveCounterBox->GetTextureWidth()), static_cast<float>(WaveCounterBox->GetTextureHeight()),
            0,
            1.0f, 1.0f, 1.0f, 0.3f);
        TextFont->textout(dc, waveText, screenWidth * 0.45f, screenHeight * 0.12f, 17, 24, { 1,1,1,1 });
    }

    if (EnemyManager::Instance().GetEnemyCount() == 0 && SceneGame::Instance().GetCurrentWave() == 5)
    {
        WaveCounterBox->Render(dc,
            screenWidth * 0.5f - static_cast<float>(WaveCounterBox->GetTextureWidth()) * 0.5f, screenHeight * 0.1f,
            250, 65,
            0, 0,
            static_cast<float>(WaveCounterBox->GetTextureWidth()), static_cast<float>(WaveCounterBox->GetTextureHeight()),
            0,
            1.0f, 1.0f, 1.0f, 0.3f);
        TextFont->textout(dc, "Complete", screenWidth * 0.45f, screenHeight * 0.12f, 17, 24, { 1,1,1,1 });

    }
}

void UI::RenderFocusingLine(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    if (Player::Instance().GetOnSwing())
    {
        float screenWidth = Graphics::Instance().GetScreenWidth();
        float screenHeight = Graphics::Instance().GetScreenHeight();

        float srcWidth = FocusingLineB->GetTextureWidth();
        float srcHeight = FocusingLineB->GetTextureHeight();

        FocusingLineB->Render(dc, 0, 0, screenWidth, screenHeight, 0, 0, srcWidth, srcHeight, 0, 1, 1, 1, alpha);

    }

}

//  HP��UI�\��
void UI::RenderHpBar(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    {
        HpBox->Render(dc,
            30, 20,
            100, 50,
            0, 0,
            static_cast<float>(HpBox->GetTextureWidth()), static_cast<float>(HpBox->GetTextureHeight()),
            1.5f,
            1, 1, 1, 0.8f);
        Number->RenderNumber(dc, Player::Instance().GetHealth(), 50, 30, 25, 25, 1, 1, 1, 1);

        float gaugeWidth = Player::Instance().GetMaxHealth() * Player::Instance().GetHealth() * 5.0f;

        //  HP�o�[���̘g
        HpBar->Render(dc,
            130, 35,
            375, 25,
            0, 0,
            250, 20,
            1.5f,
            1, 1, 1, 0.5f);

        // HP�o�[
        HpBar->Render(dc,
            130, 35,
            gaugeWidth, 25,
            0, 0,
            250, 20,
            1.5f,
            1, 1, 1, 1);
    }

    //�X�L���o�[
    {
        float skillTime = Player::Instance().GetSkillTime();
        int maxSkillTime = 5; // �ő�X�L���^�C����5�Ɖ���
        float cellWidth = 40.0f; // �e�i�q�̕�
        float cellHeight = 25.0f; // �e�i�q�̍���
        float startX = 130.0f; // �X�L���o�[�̊J�n�ʒuX
        float startY = 75.0f; // �X�L���o�[�̊J�n�ʒuY

        for (int i = 0; i < maxSkillTime; ++i)
        {
            if (i < static_cast<int>(skillTime))
            {
                // �X�L���^�C�����c���Ă���ꍇ�͖��邢�F�ŕ`��
                SkillBar->Render(dc,
                    startX + i * (cellWidth + 5.0f), startY,
                    cellWidth, cellHeight,
                    0, 0,
                    250, 20,
                    1.5f,
                    1, 1, 1, 1);
            }
            else if (i == static_cast<int>(skillTime))
            {
                // �X�L���^�C���������I�Ɏc���Ă���ꍇ�͕����I�ɕ`��
                float partialWidth = (skillTime - static_cast<int>(skillTime)) * cellWidth;
                SkillBar->Render(dc,
                    startX + i * (cellWidth + 5.0f), startY,
                    partialWidth, cellHeight,
                    0, 0,
                    250, 20,
                    1.5f,
                    1, 1, 1, 1);

                // �c��̕������Â��F�ŕ`��
                SkillBar->Render(dc,
                    startX + i * (cellWidth + 5.0f) + partialWidth, startY,
                    cellWidth - partialWidth, cellHeight,
                    0, 0,
                    250, 20,
                    1.5f,
                    0.5f, 0.5f, 0.5f, 0.5f);
            }
            else
            {
                // �X�L���^�C�����c���Ă��Ȃ��ꍇ�͈Â��F�ŕ`��
                SkillBar->Render(dc,
                    startX + i * (cellWidth + 5.0f), startY,
                    cellWidth, cellHeight,
                    0, 0,
                    250, 20,
                    1.5f,
                    0.5f, 0.5f, 0.5f, 0.5f);
            }
        }
    }
}

void UI::RenderTutorial(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();
    bool isController = SceneGame::Instance().GetIsUseController();

    //  �ŏ��̃`���g���A��
    if (SceneGame::Instance().GetIsTutorialPaused())
    {
        switch (SceneGame::Instance().GetTutorialState())
        {
        case SceneGame::TutorialState::Move:
            RenderTutorialSprite(TutorialMove, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::Jump:
            RenderTutorialSprite(TutorialJump, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::Attack:
            RenderTutorialSprite(TutorialAttack, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::Shot:
            RenderTutorialSprite(TutorialShot, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::CameraLock:
            RenderTutorialSprite(TutorialCameraLock, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::LockAttack:
            RenderTutorialSprite(TutorialAttack, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::LockShot:
            RenderTutorialSprite(TutorialShot, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::Grab:
            RenderTutorialSprite(TutorialGrab, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;
        case SceneGame::TutorialState::Dodge:
            RenderTutorialSprite(TutorialDodge, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
            break;

        }
    }
    switch (SceneGame::Instance().GetTutorialState())
    {
    case SceneGame::TutorialState::Swing:
        Font->Render(dc, screenWidth * 0.43f, screenHeight * 0.8f, 220.0f, 220.0f, 0, 0, static_cast<float>(Font->GetTextureWidth()), static_cast<float>(Font->GetTextureHeight()), 0, 1, 1, 1, 1);
        RenderTutorialSprite(TutorialSwing, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, -250, -300);

        TextFont->textout(dc, "Hold", screenWidth * 0.38f, screenHeight * 0.68f, 30, 30, { 0,0,0,1 });
        TextFont->textout(dc, "+", screenWidth * 0.48f, screenHeight * 0.75f, 40, 40, { 1,1,1,1 });

        RenderTutorialSprite(TutorialMove, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, 0, -300);
        break;
    case SceneGame::TutorialState::Climb:
        Font2->Render(dc, screenWidth * 0.43f, screenHeight * 0.8f, 220.0f, 220.0f, 0, 0, static_cast<float>(Font2->GetTextureWidth()), static_cast<float>(Font2->GetTextureHeight()), 0, 1, 1, 1, 1);
        RenderTutorialSprite(TutorialMove, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, -250, -300);
        TextFont->textout(dc, "+", screenWidth * 0.48f, screenHeight * 0.75f, 40, 40, { 1,1,1,1 });
        RenderTutorialSprite(TutorialJump, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, 0, -300);
        break;
    }


    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    //�J�������b�N���鎞��LockOnScope
    if (Player::Instance().GetlockonState() == Player::LockonState::Locked && Player::Instance().GetLockonEnemy())
    {
        // �ϊ��s��
        DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
        DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
        DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
        Enemy* enemy = Player::Instance().GetLockonEnemy();
        DirectX::XMVECTOR enemyPosition = DirectX::XMVectorSet(
            enemy->GetPosition().x,
            enemy->GetPosition().y + 0.5f,
            enemy->GetPosition().z,
            1.0f);

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

        LockOnScope->Render(dc,
            screenPosition.x - 15.0f, screenPosition.y - 60.0f,
            30.0f, 30.0f,
            0, 0,
            static_cast<float>(LockOnScope->GetTextureWidth()), static_cast<float>(LockOnScope->GetTextureHeight()),
            0,
            1, 1, 1, 1);
    }
}



void UI::Clear()
{
}
