#pragma once
#include "Graphics\Sprite.h"
#include "Input/Input.h"
#include <memory>
using namespace DirectX;

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

    void Update(float elapsedTime);

    void RenderUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void Clear();

private:
    std::unique_ptr<Sprite> Player_Health;    //

    std::unique_ptr<Sprite> Enemy_Hp;         //
    std::unique_ptr<Sprite> Player_Hp;        //

    std::unique_ptr<Sprite> Enemy_HpBar;      //
    std::unique_ptr<Sprite> Player_HpBar;     //


    float CountDownTime = 100.0f * 60;        //‰¼180•b
    bool isClear = false;



};