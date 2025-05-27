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
    // シングルトンインスタンスの取得
    static UI& Instance()
    {
        static UI instance;
        return instance;
    }

    // コンストラクタとデストラクタ
    UI() {}
    ~UI() {}

    // 初期化
    void Initialize();
    // 更新処理
    void Update(float elapsedtime);
    // 描画処理
    void DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // SpiderSenseの描画処理
    void RenderSpiderSense(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // チュートリアルの描画処理
    void RenderTutorial(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // チュートリアルスプライトの描画処理
    void RenderTutorialSprite(std::unique_ptr<Sprite>& sprite, ID3D11DeviceContext* dc, float screenWidth, float screenHeight, float alpha, bool isController, float offsetX = 0.0f, float offsetY = 0.0f);
    // WaveCounterの描画処理
    void RenderWaveCounter(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // 集中線の描画処理
    void RenderFocusingLine(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // ロックオンスコープの描画処理
    void RenderLockOnScope(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // 敵のショットラインの描画処理
    void RenderEnemyShotLine();
    // HPバーの描画処理
    void RenderHpBar(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // Screen上にスプライトを描画する関数
    void RenderWorldSpriteToScreen(
        ID3D11DeviceContext* dc,
        Sprite* sprite,
        const DirectX::XMFLOAT3& worldPosition,
        float offsetX, float offsetY,
        float width, float height,
        float alpha,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);
    // クリア
    void Clear();


private:
    // チュートリアルスプライト
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

    // 文字
    std::unique_ptr<Sprite> TextFont;
    std::unique_ptr<Sprite> Font;
    std::unique_ptr<Sprite> Font2;
    std::unique_ptr<Sprite> Hold;
    std::unique_ptr<Sprite> Number;

    // ロックオン
    std::unique_ptr<Sprite> LockOnScope;

    // SpiderSense
    std::unique_ptr<Sprite> SpiderSense;
    std::unique_ptr<Sprite> SpiderSense2;

    // 集中線
    std::unique_ptr<Sprite> FocusingLineB;
    std::unique_ptr<Sprite> FocusingLineW;
    bool switchFocusingLine = false;
    int frameCounter = 0;               // フレームカウンター
    const int frameThreshold = 30;      // スプライトを切り替えるフレーム数の閾値

    // Hpバー
    std::unique_ptr<Sprite> HpBox;
    std::unique_ptr<Sprite> HpBar;

    // スキルバー
    std::unique_ptr<Sprite> SkillBar;

    // WaveCounter
    std::unique_ptr<Sprite> WaveCounterBox;

    // 描画のための変数
    float alpha = 1.0f;
    float alphaColor = 0.3f;
    float alphaSpeed = 0.02f;
    bool increasingAlpha = false;
    bool fadeOut = true;

    // PostprocessingRenderer
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
    // ラジアルブラー
    float radialblurRadius = 0.0f;

};
