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
        DirectX::XMFLOAT4	screenSize;	// xy : サイズ, z : nearクリップ, w : farクリップ
        //	カメラ情報
        DirectX::XMFLOAT4	viewPosition;
        DirectX::XMFLOAT4X4	view;
        DirectX::XMFLOAT4X4	projection;
        DirectX::XMFLOAT4X4	inverseView;
        DirectX::XMFLOAT4X4	inverseProjection;


    };

    struct RadialBlurConstance
    {
        float blur_radius = 50.0f;
        int blur_sampling_count = 10;
        DirectX::XMFLOAT2 blur_center = { 0.5f,0.5f };
        float blur_mask_radius = 0;		//	centerからの指定の範囲はブラーを適応しないようにする
        DirectX::XMFLOAT3  blur_dummy;
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
    Microsoft::WRL::ComPtr<ID3D11SamplerState>          radialblurSamplerState;
};
