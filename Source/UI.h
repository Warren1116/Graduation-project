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
    std::unique_ptr<Sprite> LockOnScope;
    std::unique_ptr<Sprite> TutorialMove;
    std::unique_ptr<Sprite> TutorialJump;
    std::unique_ptr<Sprite> TutorialAttack;
    std::unique_ptr<Sprite> TutorialShot;
    std::unique_ptr<Sprite> TutorialCameraLock;
    std::unique_ptr<Sprite> TextFont;

    std::unique_ptr<Sprite> TutorialKey;


};
