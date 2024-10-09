#include "UI.h"
#include "Player.h"

void UI::Initialize()
{
    HpBar = std::make_unique<Sprite>();
    Hp = std::make_unique<Sprite>();
}

void UI::Update(float elapsedtime)
{
}

void UI::DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    float gaugeWidth = Player::Instance().GetMaxHealth() * Player::Instance().GetHealth() * 1.5f;

    if (Player::Instance().GetHealth() > 0)
    {
        HpBar->Render(dc, 37, 650, gaugeWidth, 45, 0, 0, Hp->GetTextureWidth(), Hp->GetTextureHeight(), 0, 1, 0, 0, 1);
    }
}


void UI::Clear()
{
}
