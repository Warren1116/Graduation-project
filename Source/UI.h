#pragma once
#include "Graphics\Graphics.h"
#include "Graphics\Model.h"
#include "Graphics\Sprite.h"


class UI
{
public:
    static UI& Instance()
    {
        static UI instance;
        return instance;
    }

    UI() {}
    ~UI() {}

    void Initialize();

    void Update(float elapsedtime);

    void DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void Clear();


private:
    std::unique_ptr<Sprite> HpBar;
    std::unique_ptr<Sprite> Hp;


};
