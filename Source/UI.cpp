#include "UI.h"
#include "EnemyManager.h"
#include "Player.h"

void UI::Initialize()
{
    Player_Health = std::make_unique<Sprite>("Data/Sprite/Numbers.png");

}

void UI::Update(float elapsedTime)
{
}

void UI::RenderUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    //タイマーの描画
    {
        Player_Health->Timer(dc, Player::Instance().GetHealth(), 600, 100, 25, 25, 1, 1, 1, 1);
    }

    {
        //変換行列
        DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
        DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
        DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

        ////全ての敵の頭上にHPゲージを表示
        //EnemyManager& enemyManager = EnemyManager::Instance();
        //int enemyCount = enemyManager.GetEnemyCount();

        //for (int i = 0; i < enemyCount; ++i)
        //{
        //    Enemy* enemy = enemyManager.GetEnemy(i);

        //    float gaugeWidth = enemy->GetMaxHealth() * enemy->GetHealth() * 3.75f;

            //Enemy_HpBar->Render(dc, 37, 13, 1208, 46, 0, 0, 300, 55, 0, 1, 1, 1, 1);
        //    Enemy_Hp->Render(dc, 80, 21, gaugeWidth, 30, 0.0f, 0.0f, Enemy_Hp->GetTextureWidth(), Enemy_Hp->GetTextureHeight(), 0, 1.0f, 0.0f, 0.0f, 1.0f);

        //}
        //float gaugeWidth = Player::Instance().GetMaxHealth() * Player::Instance().GetHealth() * 1.27f;

        //Player_HpBar->Render(dc, 37, 650, 304, 45, 0, 0, 300, 55, 0, 1, 1, 1, 1);
        //Player_Hp->Render(dc, 48, 658, gaugeWidth, 30, 0, 0, Player_Hp->GetTextureWidth(), Player_Hp->GetTextureHeight(), 0, 0.0f, 1.0f, 1.0f, 1.0f);
    }

}

void UI::Clear()
{
}
