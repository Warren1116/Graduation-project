#pragma once
#include "Graphics\Graphics.h"
#include "Graphics\Model.h"
#include "Graphics\Sprite.h"
#include <fstream>
#include <string>
#include <iostream>
#include "Renderer\PostprocessingRenderer.h"

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

    void RenderSpiderSense(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void RenderTutorial(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection); 

    void RenderTutorialSprite(std::unique_ptr<Sprite>& sprite, ID3D11DeviceContext* dc, float screenWidth, float screenHeight, float alpha, bool isController, float offsetX = 0.0f, float offsetY = 0.0f);


    void Clear();

    int GetTutorialCounter() { return TutorialCounter; }

private:
    std::unique_ptr<Sprite> LockOnScope;
    std::unique_ptr<Sprite> TutorialMove;
    std::unique_ptr<Sprite> TutorialJump;
    std::unique_ptr<Sprite> TutorialAttack;
    std::unique_ptr<Sprite> TutorialShot;
    std::unique_ptr<Sprite> TutorialCameraLock;
    std::unique_ptr<Sprite> TutorialGrab;
    std::unique_ptr<Sprite> TextFont;

    std::unique_ptr<Sprite> ShiftKey;
    std::unique_ptr<Sprite> Font;
    std::unique_ptr<Sprite> Font2;
    std::unique_ptr<Sprite> spiderSense;
    std::unique_ptr<Sprite> spiderSense2;

    std::unique_ptr<Sprite> HpBox;
    std::unique_ptr<Sprite> Number;
    std::unique_ptr<Sprite> HpBar;
    std::unique_ptr<Sprite> SkillBar;

    float alpha = 1.0f;
    float alphaColor = 0.3f;
    float alphaSpeed = 0.02f;
    bool increasingAlpha = false;
    bool fadeOut = true;
    int TutorialCounter;

    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
    float radialblurRadius = 0.0f;

};
