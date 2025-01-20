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
    LockOnScope = std::make_unique<Sprite>("Data/Sprite/LockOnScope.png");
    TutorialMove = std::make_unique<Sprite>("Data/Sprite/Tutorial_Move.png");
    TutorialJump = std::make_unique<Sprite>("Data/Sprite/Tutorial_Jump.png");
    TutorialAttack = std::make_unique<Sprite>("Data/Sprite/Tutorial_Attack.png");
    TutorialShot = std::make_unique<Sprite>("Data/Sprite/Tutorial_Shot.png");
    TutorialCameraLock = std::make_unique<Sprite>("Data/Sprite/Tutorial_CameraLock.png");
    TutorialGrab = std::make_unique<Sprite>("Data/Sprite/Tutorial_Grab.png");
    TextFont = std::make_unique<Sprite>("Data/Font/font4.png");
    Font = std::make_unique<Sprite>("Data/Sprite/Font.png");
    Font2 = std::make_unique<Sprite>("Data/Sprite/Font2.png");
    spiderSense = std::make_unique<Sprite>("Data/Sprite/SpiderSense.png");
    spiderSense2 = std::make_unique<Sprite>("Data/Sprite/SpiderSense2.png");


    HpBox = std::make_unique<Sprite>("Data/Sprite/HpBox1.png");
    Number = std::make_unique<Sprite>("Data/Sprite/Numbers.png");

    HpBar = std::make_unique<Sprite>();
    SkillBar = std::make_unique<Sprite>();

    ShiftKey = std::make_unique<Sprite>("Data/Sprite/ShiftKey.png");
    TutorialCounter = 0;

}

void UI::Update(float elapsedtime)
{
    if (SceneGame::Instance().GetIsTutorialPaused())
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
    //  攻撃を受ける時にSpiderSenseを描画する
    if (Player::Instance().GetAttackSoon())
    {
        RenderSpiderSense(dc, view, projection);
    }

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

#ifdef TUTORIAL
    //  チュトリアルの描画
    RenderTutorial(dc, view, projection);
#endif // !TUTORIAL

    //  HPのUI表現
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

        //  HPバー下の枠
        HpBar->Render(dc,
            130, 35,
            375, 25,
            0, 0,
            250, 20,
            1.5f,
            1, 1, 1, 0.5f);

        // HPバー
        HpBar->Render(dc,
            130, 35,
            gaugeWidth, 25,
            0, 0,
            250, 20,
            1.5f,
            1, 1, 1, 1);
    }

    //スキルバー
    {
        float skillTime = Player::Instance().GetSkillTime();
        int maxSkillTime = 5; // 最大スキルタイムを5と仮定
        float cellWidth = 40.0f; // 各格子の幅
        float cellHeight = 25.0f; // 各格子の高さ
        float startX = 130.0f; // スキルバーの開始位置X
        float startY = 75.0f; // スキルバーの開始位置Y

        for (int i = 0; i < maxSkillTime; ++i)
        {
            if (i < static_cast<int>(skillTime))
            {
                // スキルタイムが残っている場合は明るい色で描画
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
                // スキルタイムが部分的に残っている場合は部分的に描画
                float partialWidth = (skillTime - static_cast<int>(skillTime)) * cellWidth;
                SkillBar->Render(dc,
                    startX + i * (cellWidth + 5.0f), startY,
                    partialWidth, cellHeight,
                    0, 0,
                    250, 20,
                    1.5f,
                    1, 1, 1, 1);

                // 残りの部分を暗い色で描画
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
                // スキルタイムが残っていない場合は暗い色で描画
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

    if (Player::Instance().GetHealth() == 0)
    {
        TextFont->textout(dc, "Right Click To Go Back Title", 1280 * 0.3f, 720 * 0.5f, 20, 20, { 1,1,1,1 });
    }

}

void UI::RenderSpiderSense(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    Model::Node* PlayerHeadPos = Player::Instance().model->FindNode("mixamorig:HeadTop_End");
    DirectX::XMVECTOR playerHeadPos = DirectX::XMVectorSet(
        PlayerHeadPos->worldTransform._41,
        PlayerHeadPos->worldTransform._42,
        PlayerHeadPos->worldTransform._43,
        1.0f);

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

    spiderSense->Render(dc,
        screenPosition.x - 55.0f, screenPosition.y - 40.0f,
        120.0f, 75.0f,
        0, 0,
        static_cast<float>(spiderSense->GetTextureWidth()), static_cast<float>(spiderSense->GetTextureHeight()),
        0,
        1, 1, 1, alpha);

    spiderSense2->Render(dc,
        screenPosition.x - 55.0f, screenPosition.y - 40.0f,
        120.0f, 75.0f,
        0, 0,
        static_cast<float>(spiderSense2->GetTextureWidth()), static_cast<float>(spiderSense2->GetTextureHeight()),
        0,
        1, 1, 1, alphaColor);

}


void UI::RenderTutorialSprite(std::unique_ptr<Sprite>& sprite, ID3D11DeviceContext* dc, float screenWidth, float screenHeight, float alpha, bool isController, float offsetX, float offsetY )
{
    float x = screenWidth - 200.0f + offsetX;
    float y = screenHeight - 500.0f + offsetY;
    float width = 180.0f;
    float height = 180.0f;
    float srcX = isController ? sprite->GetTextureWidth() * 0.5f : 0.0f;
    float srcY = 0.0f;
    float srcWidth = sprite->GetTextureWidth() * 0.5f;
    float srcHeight = sprite->GetTextureHeight();

    sprite->Render(dc, x, y, width, height, srcX, srcY, srcWidth, srcHeight, 0, 1, 1, 1, alpha);
}

void UI::RenderTutorial(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();
    bool isController = SceneGame::Instance().GetIsUseController();

    //  最初のチュトリアル
    if (SceneGame::Instance().GetIsTutorialPaused())
    {
        switch (SceneGame::Instance().GetTutorialState())
        {
        case SceneGame::TutorialState::Move:
            RenderTutorialSprite(TutorialMove, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::Jump:
            RenderTutorialSprite(TutorialJump, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::Attack:
            RenderTutorialSprite(TutorialAttack, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::Shot:
            RenderTutorialSprite(TutorialShot, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::CameraLock:
            RenderTutorialSprite(TutorialCameraLock, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::LockAttack:
            RenderTutorialSprite(TutorialAttack, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::LockShot:
            RenderTutorialSprite(TutorialShot, dc, screenWidth, screenHeight, alpha, isController);
            break;
        case SceneGame::TutorialState::Grab:
            RenderTutorialSprite(TutorialGrab, dc, screenWidth, screenHeight, alpha, isController);
            break;
        }
    }

    //スイングのチュトリアル
    if (SceneGame::Instance().GetTutorialState() == SceneGame::TutorialState::Swing)
    {
        Font->Render(dc, screenWidth - 300.0f, screenHeight - 600.0f, 180.0f, 180.0f, 0, 0, static_cast<float>(Font->GetTextureWidth()), static_cast<float>(Font->GetTextureHeight()), 0, 1, 1, 1, 1);
        TextFont->textout(dc, "Hold", screenWidth - 365.0f, screenHeight - 480.0f, 30, 30, { 0,0,0,1 });

        ShiftKey->Render(dc, screenWidth - 400.0f, screenHeight - 500.0f, 180.0f, 180.0f, isController ? ShiftKey->GetTextureWidth() * 0.5f : 0, 0, ShiftKey->GetTextureWidth() * 0.5f, ShiftKey->GetTextureHeight(), 0, 1, 1, 1, 1);
        TextFont->textout(dc, "+", screenWidth - 250.0f, screenHeight - 430.0f, 40, 40, { 1,1,1,1 });

        RenderTutorialSprite(TutorialMove, dc, screenWidth, screenHeight, 1.0f, isController, 0.0f, 0.0f);
    }

    //クライミングのチュトリアル
    if (SceneGame::Instance().GetTutorialState() == SceneGame::TutorialState::Climb)
    {
        Font2->Render(dc, screenWidth - 300.0f, screenHeight - 600.0f, 180.0f, 180.0f, 0, 0, static_cast<float>(Font->GetTextureWidth()), static_cast<float>(Font->GetTextureHeight()), 0, 1, 1, 1, 1);
        RenderTutorialSprite(TutorialMove, dc, screenWidth, screenHeight, 1.0f, isController, -200.0f, 0.0f);
        TextFont->textout(dc, "+", screenWidth - 230.0f, screenHeight - 430.0f, 40, 40, { 1,1,1,1 });
        RenderTutorialSprite(TutorialJump, dc, screenWidth, screenHeight, 1.0f, isController, 0.0f, -20.0f);
    }

    if (Player::Instance().GetState() == Player::State::Swing && TutorialCounter == 0)
    {
        TutorialCounter++;
        SceneGame::Instance().SetTutorialState(SceneGame::TutorialState::Climb);
    }
    if (Player::Instance().GetState() == Player::State::Climb && TutorialCounter == 1)
    {
        TutorialCounter++;
        SceneGame::Instance().SetTutorialState(SceneGame::TutorialState::Finish);
    }

    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    //カメラロックする時のLockOnScope
    if (Player::Instance().GetlockonState() == Player::LockonState::Locked)
    {
        // 変換行列
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
