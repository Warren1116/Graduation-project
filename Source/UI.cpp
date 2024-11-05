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
}

void UI::Update(float elapsedtime)
{

}

void UI::DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();

    //  最初のチュトリアル
    if (SceneGame::Instance().GetIsPaused())
    {
        switch (SceneGame::Instance().GetTutorialState())
        {
        case SceneGame::TutorialState::Move:
            TutorialMove->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialMove->GetTextureWidth()), static_cast<float>(TutorialMove->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;
        case SceneGame::TutorialState::Jump:
            TutorialJump->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialJump->GetTextureWidth()), static_cast<float>(TutorialJump->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;
        case SceneGame::TutorialState::Attack:
            TutorialAttack->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialAttack->GetTextureWidth()), static_cast<float>(TutorialAttack->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;
        case SceneGame::TutorialState::Shot:
            TutorialShot->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialShot->GetTextureWidth()), static_cast<float>(TutorialShot->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;       
        case SceneGame::TutorialState::CameraLock:
            TutorialCameraLock->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialCameraLock->GetTextureWidth()), static_cast<float>(TutorialCameraLock->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;
        case SceneGame::TutorialState::LockAttack:
            TutorialAttack->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialAttack->GetTextureWidth()), static_cast<float>(TutorialAttack->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;
        case SceneGame::TutorialState::LockShot:
            TutorialShot->Render(dc,
                screenWidth - 200.0f, screenHeight - 500.0f,
                180.0f, 180.0f,
                0, 0,
                static_cast<float>(TutorialShot->GetTextureWidth()), static_cast<float>(TutorialShot->GetTextureHeight()),
                0,
                1, 1, 1, 1);
            break;
        }
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
            screenPosition.x - 15.0f, screenPosition.y - 15.0f,
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
