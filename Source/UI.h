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
    // �V���O���g���C���X�^���X�̎擾
    static UI& Instance()
    {
        static UI instance;
        return instance;
    }

    // �R���X�g���N�^�ƃf�X�g���N�^
    UI() {}
    ~UI() {}

    // ������
    void Initialize();
    // �X�V����
    void Update(float elapsedtime);
    // �`�揈��
    void DrawUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // SpiderSense�̕`�揈��
    void RenderSpiderSense(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // �`���[�g���A���̕`�揈��
    void RenderTutorial(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // �`���[�g���A���X�v���C�g�̕`�揈��
    void RenderTutorialSprite(std::unique_ptr<Sprite>& sprite, ID3D11DeviceContext* dc, float screenWidth, float screenHeight, float alpha, bool isController, float offsetX = 0.0f, float offsetY = 0.0f);
    // WaveCounter�̕`�揈��
    void RenderWaveCounter(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // �W�����̕`�揈��
    void RenderFocusingLine(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // ���b�N�I���X�R�[�v�̕`�揈��
    void RenderLockOnScope(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // �G�̃V���b�g���C���̕`�揈��
    void RenderEnemyShotLine();
    // HP�o�[�̕`�揈��
    void RenderHpBar(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
    // Screen��ɃX�v���C�g��`�悷��֐�
    void RenderWorldSpriteToScreen(
        ID3D11DeviceContext* dc,
        Sprite* sprite,
        const DirectX::XMFLOAT3& worldPosition,
        float offsetX, float offsetY,
        float width, float height,
        float alpha,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);
    // �N���A
    void Clear();


private:
    // �`���[�g���A���X�v���C�g
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

    // ����
    std::unique_ptr<Sprite> TextFont;
    std::unique_ptr<Sprite> Font;
    std::unique_ptr<Sprite> Font2;
    std::unique_ptr<Sprite> Hold;
    std::unique_ptr<Sprite> Number;

    // ���b�N�I��
    std::unique_ptr<Sprite> LockOnScope;

    // SpiderSense
    std::unique_ptr<Sprite> SpiderSense;
    std::unique_ptr<Sprite> SpiderSense2;

    // �W����
    std::unique_ptr<Sprite> FocusingLineB;
    std::unique_ptr<Sprite> FocusingLineW;
    bool switchFocusingLine = false;
    int frameCounter = 0;               // �t���[���J�E���^�[
    const int frameThreshold = 30;      // �X�v���C�g��؂�ւ���t���[������臒l

    // Hp�o�[
    std::unique_ptr<Sprite> HpBox;
    std::unique_ptr<Sprite> HpBar;

    // �X�L���o�[
    std::unique_ptr<Sprite> SkillBar;

    // WaveCounter
    std::unique_ptr<Sprite> WaveCounterBox;

    // �`��̂��߂̕ϐ�
    float alpha = 1.0f;
    float alphaColor = 0.3f;
    float alphaSpeed = 0.02f;
    bool increasingAlpha = false;
    bool fadeOut = true;

    // PostprocessingRenderer
    std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
    // ���W�A���u���[
    float radialblurRadius = 0.0f;

};
