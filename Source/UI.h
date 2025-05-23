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

    void RenderWaveCounter(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void RenderFocusingLine(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void RenderHpBar(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void RenderWorldSpriteToScreen(
        ID3D11DeviceContext* dc,
        Sprite* sprite,
        const DirectX::XMFLOAT3& worldPosition,
        float offsetX, float offsetY,
        float width, float height,
        float alpha,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);
    void Clear();


private:
    std::unique_ptr<Sprite> LockOnScope;
    std::unique_ptr<Sprite> TutorialMove;
    std::unique_ptr<Sprite> TutorialJump;
    std::unique_ptr<Sprite> TutorialAttack;
    std::unique_ptr<Sprite> TutorialShot;
    std::unique_ptr<Sprite> TutorialCameraLock;
    std::unique_ptr<Sprite> TutorialGrab;
    std::unique_ptr<Sprite> TutorialDodge;
    std::unique_ptr<Sprite> TutorialSwing;
    std::unique_ptr<Sprite> TutorialHealing;
    std::unique_ptr<Sprite> TutorialUltimate;
    std::unique_ptr<Sprite> TextFont;

    std::unique_ptr<Sprite> Font;
    std::unique_ptr<Sprite> Font2;
    std::unique_ptr<Sprite> Hold;

    std::unique_ptr<Sprite> SpiderSense;
    std::unique_ptr<Sprite> SpiderSense2;

    std::unique_ptr<Sprite> FocusingLineB;
    std::unique_ptr<Sprite> FocusingLineW;
    bool switchFocusingLine = false;
    int frameCounter = 0; // フレームカウンター
    const int frameThreshold = 30; // スプライトを切り替えるフレーム数の閾値


    std::unique_ptr<Sprite> HpBox;
    std::unique_ptr<Sprite> Number;

    std::unique_ptr<Sprite> HpBar;
    std::unique_ptr<Sprite> SkillBar;
    std::unique_ptr<Sprite> WaveCounterBox;

    float alpha = 1.0f;
    float alphaColor = 0.3f;
    float alphaSpeed = 0.02f;
    bool increasingAlpha = false;
    bool fadeOut = true;


    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
    float radialblurRadius = 0.0f;

};
