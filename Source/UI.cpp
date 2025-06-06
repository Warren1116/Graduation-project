#include "UI.h"
#include "Player.h"
#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\GamePad.h"
#include "Input\Input.h"
#include "EnemyManager.h"
#include "SceneTitle.h"
#include "MiniMap.h"

#define TUTORIAL

void UI::Initialize()
{
    // 画像読み込み
    LockOnScope = std::make_unique<Sprite>("Data/Sprite/LockOnScope.png");
    TutorialMove = std::make_unique<Sprite>("Data/Sprite/Tutorial_Move.png");
    TutorialJump = std::make_unique<Sprite>("Data/Sprite/Tutorial_Jump.png");
    TutorialAttack = std::make_unique<Sprite>("Data/Sprite/Tutorial_Attack.png");
    TutorialShot = std::make_unique<Sprite>("Data/Sprite/Tutorial_Shot.png");
    TutorialCameraLock = std::make_unique<Sprite>("Data/Sprite/Tutorial_CameraLock.png");
    TutorialGrab = std::make_unique<Sprite>("Data/Sprite/Tutorial_Grab.png");
    TutorialDodge = std::make_unique<Sprite>("Data/Sprite/Tutorial_Dodge.png");
    TutorialSwing = std::make_unique<Sprite>("Data/Sprite/Tutorial_Swing.png");
    TutorialHealing = std::make_unique<Sprite>("Data/Sprite/Tutorial_Healing.png");
    TutorialUltimate = std::make_unique<Sprite>("Data/Sprite/Tutorial_Ultimate.png");
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
    Hold = std::make_unique<Sprite>("Data/Sprite/Hold.png");
    HpBar = std::make_unique<Sprite>();
    SkillBar = std::make_unique<Sprite>();

    MiniMap::Instance().Initialize(Graphics::Instance().GetDevice(), 128);
}

void UI::Update(float elapsedtime)
{
    MiniMap::Instance().Update(elapsedtime);
    //  チュートリアルの進行
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

//  UIの描画
void UI::DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    MiniMap::Instance().Render(dc);

    //  攻撃を受ける時にSpiderSenseを描画する
    if (Player::Instance().GetAttackSoon())
    {
        RenderSpiderSense(dc, view, projection);
    }

    //  敵から銃撃を受ける時に狙いを
    if (Player::Instance().GetShotSoon())
    {
        //  敵の銃撃の線を描画
        RenderEnemyShotLine();
    }

    // 　Waveカウンターの描画
    RenderWaveCounter(dc, view, projection);

    //  HPバーとSkillバーの描画
    RenderHpBar(dc, view, projection);

    //　集中線の描画
    RenderFocusingLine(dc, view, projection);

    //  ロックオンスコープの描画
    RenderLockOnScope(dc, view, projection);


#ifdef TUTORIAL
    //  チュトリアルの描画
    RenderTutorial(dc, view, projection);
#endif // !TUTORIAL


}

//  SpiderSenseの描画
void UI::RenderSpiderSense(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


    // プレイヤーの頭の位置を取得
    Model::Node* PlayerHeadPos = Player::Instance().model->FindNode("mixamorig:HeadTop_End");
    DirectX::XMVECTOR playerHeadPos = DirectX::XMVectorSet(
        PlayerHeadPos->worldTransform._41,
        PlayerHeadPos->worldTransform._42,
        PlayerHeadPos->worldTransform._43,
        1.0f);

    DirectX::XMFLOAT3 headPos;
    DirectX::XMStoreFloat3(&headPos, playerHeadPos);

    RenderWorldSpriteToScreen(dc,
        SpiderSense.get(),
        headPos,
        -55.0f, -40.0f,
        120.0f, 75.0f,
        alpha,
        view, projection);

    RenderWorldSpriteToScreen(dc,
        SpiderSense2.get(),
        headPos,
        -55.0f, -40.0f,
        120.0f, 75.0f,
        alphaColor,
        view, projection);

}


//  チュートリアルの描画
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

//  WaveCounterの描画
void UI::RenderWaveCounter(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    float screenWidth = Graphics::Instance().GetScreenWidth();
    float screenHeight = Graphics::Instance().GetScreenHeight();

    // ウェーブ数を取得
    int wave = SceneGame::Instance().GetCurrentWave();
    // 最大ウェーブ数を取得
    int totalWaves = SceneGame::Instance().GetTotalWaves();
    // ウェーブ数の文字列
    std::string waveText = "Wave " + std::to_string(wave) + "/" + std::to_string(totalWaves);

    // 次のウェーブの文字を表示
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

//  集中線の描画
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

void UI::RenderLockOnScope(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //カメラロックする時のLockOnScope
    if (Player::Instance().GetlockonState() == Player::LockonState::Locked && Player::Instance().GetLockonEnemy())
    {
        DirectX::XMFLOAT3 enemyPos = Player::Instance().GetLockonEnemy()->GetPosition();
        enemyPos.y += 0.5f;

        RenderWorldSpriteToScreen(dc,
            LockOnScope.get(),
            enemyPos,
            -15.0f, -60.0f,
            30.0f, 30.0f,
            1.0f,
            view, projection);
    }
}

void UI::RenderEnemyShotLine()
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

//  HPのUI表現
void UI::RenderHpBar(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    const float hpBoxX = 30.0f;
    const float hpBoxY = 20.0f;
    const float hpBoxWidth = 100.0f;
    const float hpBoxHeight = 50.0f;
    const float hpBarX = 130.0f;
    const float hpBarY = 35.0f;
    const float hpBarWidth = 500.0f;
    const float hpBarHeight = 25.0f;
    const float skillBarX = 130.0f;
    const float skillBarY = 75.0f;
    const float cellWidth = 40.0f;
    const float cellHeight = 25.0f;
    const float cellSpacing = 5.0f;
    const int maxSkillTime = 5; // 最大スキルタイムを5と仮定

    // HPボックスの描画
    HpBox->Render(dc, hpBoxX, hpBoxY, hpBoxWidth, hpBoxHeight, 0, 0, static_cast<float>(HpBox->GetTextureWidth()), static_cast<float>(HpBox->GetTextureHeight()), 1.5f, 1, 1, 1, 0.8f);

    // HP数値の描画
    Number->RenderNumber(dc, Player::Instance().GetHealth(), hpBoxX + 20.0f, hpBoxY + 10.0f, 25.0f, 25.0f, 1, 1, 1, 1);

    // HPバーの描画
    float gaugeWidth = Player::Instance().GetMaxHealth() * Player::Instance().GetHealth();
    HpBar->Render(dc, hpBarX, hpBarY, hpBarWidth, hpBarHeight, 0, 0, 250, 20, 1.5f, 1, 1, 1, 0.5f);
    HpBar->Render(dc, hpBarX, hpBarY, gaugeWidth, hpBarHeight, 0, 0, 250, 20, 1.5f, 1, 1, 1, 1);

    // スキルバーの描画
    float skillTime = Player::Instance().GetSkillTime();
    for (int i = 0; i < maxSkillTime; ++i)
    {
        float x = skillBarX + i * (cellWidth + cellSpacing);
        if (i < static_cast<int>(skillTime))
        {
            // スキルタイムが残っている場合は明るい色で描画
            SkillBar->Render(dc, x, skillBarY, cellWidth, cellHeight, 0, 0, 250, 20, 1.5f, 1, 1, 1, 1);
        }
        else if (i == static_cast<int>(skillTime))
        {
            // スキルタイムが部分的に残っている場合は部分的に描画
            float partialWidth = (skillTime - static_cast<int>(skillTime)) * cellWidth;
            SkillBar->Render(dc, x, skillBarY, partialWidth, cellHeight, 0, 0, 250, 20, 1.5f, 1, 1, 1, 1);
            SkillBar->Render(dc, x + partialWidth, skillBarY, cellWidth - partialWidth, cellHeight, 0, 0, 250, 20, 1.5f, 0.5f, 0.5f, 0.5f, 0.5f);
        }
        else
        {
            // スキルタイムが残っていない場合は暗い色で描画
            SkillBar->Render(dc, x, skillBarY, cellWidth, cellHeight, 0, 0, 250, 20, 1.5f, 0.5f, 0.5f, 0.5f, 0.5f);
        }
    }
}

//　チュートリアルのクリア
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

        Hold->Render(dc, screenWidth * 0.38f, screenHeight * 0.65f, 120, 120, 0, 0, static_cast<float>(Hold->GetTextureWidth()), static_cast<float>(Hold->GetTextureHeight()), 0, 1, 1, 1, 1);
        TextFont->textout(dc, "+", screenWidth * 0.48f, screenHeight * 0.75f, 40, 40, { 1,1,1,1 });

        RenderTutorialSprite(TutorialMove, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, 0, -300);
        break;
    case SceneGame::TutorialState::Climb:
        Font2->Render(dc, screenWidth * 0.43f, screenHeight * 0.8f, 220.0f, 220.0f, 0, 0, static_cast<float>(Font2->GetTextureWidth()), static_cast<float>(Font2->GetTextureHeight()), 0, 1, 1, 1, 1);
        RenderTutorialSprite(TutorialMove, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, -250, -300);
        TextFont->textout(dc, "+", screenWidth * 0.48f, screenHeight * 0.75f, 40, 40, { 1,1,1,1 });
        RenderTutorialSprite(TutorialJump, dc, screenWidth * 0.5f, screenHeight, 1.0f, isController, 0, -300);
        break;
    case SceneGame::TutorialState::Healing:
        RenderTutorialSprite(TutorialHealing, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
        break;
    case SceneGame::TutorialState::Ultimate:
        RenderTutorialSprite(TutorialUltimate, dc, screenWidth * 0.48f, screenHeight, alpha, isController, -100, -300);
        break;
    }


    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

}


void UI::RenderWorldSpriteToScreen(ID3D11DeviceContext* dc, Sprite* sprite, const DirectX::XMFLOAT3& worldPosition, float offsetX, float offsetY, float width, float height, float alpha, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // ビューポート取得
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // 行列変換
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // 3D座標をベクトルに
    DirectX::XMVECTOR worldPos = DirectX::XMLoadFloat3(&worldPosition);

    // スクリーン座標へ変換
    DirectX::XMVECTOR screenPosVec = DirectX::XMVector3Project(
        worldPos,
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        viewport.MinDepth, viewport.MaxDepth,
        Projection, View, World);

    DirectX::XMFLOAT3 screenPos;
    DirectX::XMStoreFloat3(&screenPos, screenPosVec);

    // スプライト描画
    sprite->Render(dc,
        screenPos.x + offsetX,
        screenPos.y + offsetY,
        width, height,
        0, 0,
        static_cast<float>(sprite->GetTextureWidth()),
        static_cast<float>(sprite->GetTextureHeight()),
        0,
        1, 1, 1, alpha);
}

void UI::Clear()
{
}
