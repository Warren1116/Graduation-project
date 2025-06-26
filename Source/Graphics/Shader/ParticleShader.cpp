#include "ParticleShader.h"
#include "Misc.h"
#include <d3dcompiler.h>
#include <memory>

ParticleShader::ParticleShader(ID3D11Device* device)
{
    {
        FILE* fp = nullptr;
        fopen_s(&fp, "Shader\\ParticleVS.cso", "rb");
        _ASSERT_EXPR_A(fp, "CSO File not found");

        fseek(fp, 0, SEEK_END);
        long csoSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
        fread(csoData.get(), csoSize, 1, fp);
        fclose(fp);

        //頂点シェーダー生成
        HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr,
            vertexShader.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        //入力レイアウト
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "LIFE",     0, DXGI_FORMAT_R32_FLOAT,       0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        };
        hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(),
            csoSize, inputLayout.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }


    {
        FILE* fp = nullptr;
        fopen_s(&fp, "Shader\\ParticlePS.cso", "rb");
        _ASSERT_EXPR_A(fp, "CSO File not found");

        fseek(fp, 0, SEEK_END);
        long csoSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
        fread(csoData.get(), csoSize, 1, fp);
        fclose(fp);

        //ピクセルシェーダー生成
        HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr,
            pixelShader.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    device->CreateBuffer(&desc, nullptr, constantBuffer.GetAddressOf());

    desc.ByteWidth = sizeof(Particle) * 1000;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&desc, nullptr, vertexBuffer.GetAddressOf());

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blendDesc, blendState.GetAddressOf());

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = false;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    device->CreateDepthStencilState(&dsDesc, depthStencilState.GetAddressOf());

    D3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.DepthClipEnable = true;
    device->CreateRasterizerState(&rastDesc, rasterizerState.GetAddressOf());

}

void ParticleShader::UpdateParticles(float elapsedTime)
{
    particles.resize(100);
    for (int i = 0; i < (int)particles.size(); ++i)
    {
        float x = ((rand() % 100) - 50) / 10.0f;
        float y = ((rand() % 100)) / 10.0f;
        float z = ((rand() % 100) - 50) / 10.0f;
        particles[i].position = { x, y, z };
        particles[i].life = 1.0f;
    }
}

void ParticleShader::Begin(const RenderContext& rc)
{
    rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    rc.deviceContext->IASetInputLayout(inputLayout.Get());
    rc.deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);
    rc.deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    rc.deviceContext->RSSetState(rasterizerState.Get());
}

void ParticleShader::Draw(const RenderContext& rc)
{
    rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &rc.projection, 0, 0);
    rc.deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    D3D11_MAPPED_SUBRESOURCE mapped;
    rc.deviceContext->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, particles.data(), sizeof(Particle) * particles.size());
    rc.deviceContext->Unmap(vertexBuffer.Get(), 0);

    UINT stride = sizeof(Particle);
    UINT offset = 0;
    rc.deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    rc.deviceContext->Draw((UINT)particles.size(), 0);
}

void ParticleShader::End(const RenderContext& rc)
{
    rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
    rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
    rc.deviceContext->IASetInputLayout(nullptr);
}