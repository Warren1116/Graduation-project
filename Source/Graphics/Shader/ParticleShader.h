#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Graphics/RenderContext.h"

class ParticleShader
{
public:
    ParticleShader(ID3D11Device* device);

    void Begin(const RenderContext& rc);
    void Draw(const RenderContext& rc);
    void End(const RenderContext& rc);

    void UpdateParticles(float elapsedTime);
private:
    struct Particle
    {
        DirectX::XMFLOAT3 position;
        float life;
    };

    std::vector<Particle> particles;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilOff;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
};
