#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class FinalpassShader : public SpriteShader
{
public:
    FinalpassShader(ID3D11Device* device);
    ~FinalpassShader() override {}

    void Begin(const RenderContext& rc) override;
    void Draw(const RenderContext& rc, const Sprite* sprite) override;
    void End(const RenderContext& rc) override;

private:
    struct CBFinalpassConstance
    {
        DirectX::XMFLOAT4	screenSize;	// xy : �T�C�Y, z : near�N���b�v, w : far�N���b�v
        //	�J�������
        DirectX::XMFLOAT4	viewPosition;
        DirectX::XMFLOAT4X4	view;
        DirectX::XMFLOAT4X4	projection;
        DirectX::XMFLOAT4X4	inverseView;
        DirectX::XMFLOAT4X4	inverseProjection;
        //	�F���␳
        float				hueShift;	// �F������
        float				saturation;	// �ʓx����
        float				brightness;	// ���x����
        float				colorGradiationDummy;

    };
    struct Fog
    {
        DirectX::XMFLOAT3 fogColor;
        //float fogIntensity;
        float fogDensity;
        float fogHeightFalloff;
        float fogCutoffDistance;
        float groundLevel;
        //float mieScatteringCoef;
        //float timeScale;
        //float noiseScale;
        float dummy;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer>				finalpassConstantBuffer[2];

    Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

    Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>			shadowMapSamplerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>			depthSamplerState;

};
