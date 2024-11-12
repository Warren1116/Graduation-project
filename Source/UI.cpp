#include "UI.h"
#include "Player.h"
#include "SceneGame.h"
#include "Graphics\Graphics.h"

void UI::Initialize()
{
    LockOnScope = std::make_unique<Sprite>("Data/Sprite/LockOnScope.png");
    TutorialMove = std::make_unique<Sprite>("Data/Sprite/Tutorial_Move.png");
    TutorialJump = std::make_unique<Sprite>("Data/Sprite/Tutorial_Jump.png");
    TutorialAttack = std::make_unique<Sprite>("Data/Sprite/Tutorial_Attack.png");
    TutorialShot = std::make_unique<Sprite>("Data/Sprite/Tutorial_Shot.png");
    TutorialCameraLock = std::make_unique<Sprite>("Data/Sprite/Tutorial_CameraLock.png");
    TextFont = std::make_unique<Sprite>("Data/Font/font4.png");
    Font = std::make_unique<Sprite>("Data/Sprite/Font.png");
    Font2 = std::make_unique<Sprite>("Data/Sprite/Font2.png");
    spiderSense = std::make_unique<Sprite>("Data/Sprite/SpiderSense.png");
    spiderSense2 = std::make_unique<Sprite>("Data/Sprite/SpiderSense2.png");


    ShiftKey = std::make_unique<Sprite>("Data/Sprite/ShiftKey.png");
    TutorialCounter = 0;
}

void UI::Update(float elapsedtime)
{
    if (SceneGame::Instance().GetIsPaused())
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
    if (Player::Instance().GetAttackSoon())
    {
        RenderSpiderSense(dc, view, projection);
    }

    RenderTutorial(dc, view, projection);

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
        100.0f, 70.0f,
        0, 0,
        static_cast<float>(spiderSense->GetTextureWidth()), static_cast<float>(spiderSense->GetTextureHeight()),
        0,
        1, 1, 1, alpha);

    spiderSense2->Render(dc,
        screenPosition.x - 55.0f, screenPosition.y - 40.0f,
        100.0f, 70.0f,
        0, 0,
        static_cast<float>(spiderSense2->GetTextureWidth()), static_cast<float>(spiderSense2->GetTextureHeight()),
        0,
        1, 1, 1, alphaColor);

}

void UI::RenderTutorial(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();

    //  最初のチュトリアル
    if (SceneGame::Instance().GetIsPaused())
    {
        switch (SceneGame::Instance().GetTutorialState())
        {
            //移動のチュトリアル
        case SceneGame::TutorialState::Move:
            TutorialMove->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialMove->GetTextureWidth()), static_cast<float>(TutorialMove->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
            //ジャンブのチュトリアル
        case SceneGame::TutorialState::Jump:
            TutorialJump->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialJump->GetTextureWidth()), static_cast<float>(TutorialJump->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
            //攻撃のチュトリアル
        case SceneGame::TutorialState::Attack:
            TutorialAttack->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialAttack->GetTextureWidth()), static_cast<float>(TutorialAttack->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
            //ショットのチュトリアル
        case SceneGame::TutorialState::Shot:
            TutorialShot->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialShot->GetTextureWidth()), static_cast<float>(TutorialShot->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
            //カメラロックのチュトリアル
        case SceneGame::TutorialState::CameraLock:
            TutorialCameraLock->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialCameraLock->GetTextureWidth()), static_cast<float>(TutorialCameraLock->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
            //ロックオンの攻撃チュトリアル
        case SceneGame::TutorialState::LockAttack:
            TutorialAttack->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialAttack->GetTextureWidth()), static_cast<float>(TutorialAttack->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
            //ロックオンのショットチュトリアル
        case SceneGame::TutorialState::LockShot:
            TutorialShot->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialShot->GetTextureWidth()), static_cast<float>(TutorialShot->GetTextureHeight()),
                0,
                1, 1, 1, alpha);
            break;
        }
    }
    //スイングのチュトリアル
    if (SceneGame::Instance().GetTutorialState() == SceneGame::TutorialState::Swing)
    {
        Font->Render(dc,
            screenWidth - 300.0f, screenHeight - 600.0f,
            180.0f, 180.0f,
            0, 0,
            static_cast<float>(Font->GetTextureWidth()), static_cast<float>(Font->GetTextureHeight()),
            0,
            1, 1, 1, 1);

        TextFont->textout(dc, "Hold", screenWidth - 365.0f, screenHeight - 480.0f, 30, 30, { 0,0,0,1 });

        ShiftKey->Render(dc,
            screenWidth - 400.0f, screenHeight - 500.0f,
            180.0f, 180.0f,
            0, 0,
            static_cast<float>(ShiftKey->GetTextureWidth()), static_cast<float>(ShiftKey->GetTextureHeight()),
            0,
            1, 1, 1, 1);

        TextFont->textout(dc, "+", screenWidth - 250.0f, screenHeight - 430.0f, 40, 40, { 1,1,1,1 });

        TutorialMove->Render(dc,
            screenWidth - 200.0f, screenHeight - 500.0f,
            180.0f, 180.0f,
            0, 110,
            static_cast<float>(TutorialMove->GetTextureWidth()), static_cast<float>(TutorialMove->GetTextureHeight()) - 110,
            0,
            1, 1, 1, 1);
    }
    //クライミングのチュトリアル
    if (SceneGame::Instance().GetTutorialState() == SceneGame::TutorialState::Climb)
    {
        Font2->Render(dc,
            screenWidth - 300.0f, screenHeight - 600.0f,
            180.0f, 180.0f,
            0, 0,
            static_cast<float>(Font->GetTextureWidth()), static_cast<float>(Font->GetTextureHeight()),
            0,
            1, 1, 1, 1);

        TutorialMove->Render(dc,
            screenWidth - 400.0f, screenHeight - 500.0f,
            180.0f, 180.0f,
            0, 110,
            static_cast<float>(TutorialMove->GetTextureWidth()), static_cast<float>(TutorialMove->GetTextureHeight()) - 110,
            0,
            1, 1, 1, 1);

        TextFont->textout(dc, "+", screenWidth - 230.0f, screenHeight - 430.0f, 40, 40, { 1,1,1,1 });

        TutorialJump->Render(dc,
            screenWidth - 200.0f, screenHeight - 480.0f,
            180.0f, 180.0f,
            0, 160,
            static_cast<float>(TutorialJump->GetTextureWidth()), static_cast<float>(TutorialJump->GetTextureHeight() - 160),
            0,
            1, 1, 1, 1);

    }

    if (Player::Instance().GetState() == Player::State::Swing && TutorialCounter ==0)
    {
        TutorialCounter++;
        SceneGame::Instance().SetTutorialState(SceneGame::TutorialState::Climb);
    }
    if (Player::Instance().GetState() == Player::State::Climb && TutorialCounter ==1)
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
