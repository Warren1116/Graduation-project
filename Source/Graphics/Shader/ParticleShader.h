#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Graphics/Shader.h"
struct Particle
{
    DirectX::XMFLOAT3 position;
    float life;
};


class ParticleShader
{
public:
    ParticleShader(ID3D11Device* device);
    void Begin(const RenderContext& rc);
    void Draw(const RenderContext& rc);
    void End(const RenderContext& rc);
    void UpdateParticles(float elapsedTime);
    void EmitRandomParticles(int count, const DirectX::XMFLOAT3& viewPosition);


private:
    struct CbScene
    {
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 viewProjection;
    };

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;

    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

    std::vector<Particle> particles;
};
