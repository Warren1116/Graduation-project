#include "Misc.h"
#include "Graphics/Shader/FinalpassShader.h"

FinalpassShader::FinalpassShader(ID3D11Device* device)
{
    // 頂点シェーダー
    {
        // ファイルを開く
        FILE* fp = nullptr;
        fopen_s(&fp, "Shader\\FinalpassVS.cso", "rb");
        _ASSERT_EXPR_A(fp, "CSO File not found");

        // ファイルのサイズを求める
        fseek(fp, 0, SEEK_END);
        long csoSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // メモリ上に頂点シェーダーデータを格納する領域を用意する
        std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
        fread(csoData.get(), csoSize, 1, fp);
        fclose(fp);

        // 頂点シェーダー生成
        HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertexShader.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        // 入力レイアウト
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    // ピクセルシェーダー
    {
        // ファイルを開く
        FILE* fp = nullptr;
        fopen_s(&fp, "Shader\\FinalpassPS.cso", "rb");
        _ASSERT_EXPR_A(fp, "CSO File not found");

        // ファイルのサイズを求める
        fseek(fp, 0, SEEK_END);
        long csoSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // メモリ上にピクセルシェーダーデータを格納する領域を用意する
        std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
        fread(csoData.get(), csoSize, 1, fp);
        fclose(fp);

        // ピクセルシェーダー生成
        HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixelShader.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    // 定数バッファ
    {
        // シーン用バッファ
        D3D11_BUFFER_DESC desc{};
        ::memset(&desc, 0, sizeof(desc));

        desc.ByteWidth = sizeof(CBFinalpassConstance);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = device->CreateBuffer(&desc, 0, finalpassConstantBuffer[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


        //// フォグ用バッファ
        //desc.ByteWidth = sizeof(Fog);
        //desc.Usage = D3D11_USAGE_DEFAULT;
        //desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        //desc.CPUAccessFlags = 0;
        //desc.MiscFlags = 0;
        //desc.StructureByteStride = 0;

        //hr = device->CreateBuffer(&desc, 0, finalpassConstantBuffer[1].GetAddressOf());
        //_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


        desc.ByteWidth = sizeof(RadialBlurConstance);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = device->CreateBuffer(&desc, 0, finalpassConstantBuffer[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


    }

    // ブレンドステート
    {
        D3D11_BLEND_DESC desc;
        ::memset(&desc, 0, sizeof(desc));
        desc.AlphaToCoverageEnable = false;
        desc.IndependentBlendEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        HRESULT hr = device->CreateBlendState(&desc, blendState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    // 深度ステンシルステート
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ::memset(&desc, 0, sizeof(desc));
        desc.DepthEnable = true;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        HRESULT hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }


    // ラスタライザーステート
    {
        D3D11_RASTERIZER_DESC desc;
        ::memset(&desc, 0, sizeof(desc));
        desc.FrontCounterClockwise = false;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0;
        desc.SlopeScaledDepthBias = 0;
        desc.DepthClipEnable = true;
        desc.ScissorEnable = false;
        desc.MultisampleEnable = true;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_BACK;
        desc.AntialiasedLineEnable = false;

        HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    // サンプラステート
    {
        D3D11_SAMPLER_DESC desc;
        ::memset(&desc, 0, sizeof(desc));
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = 1;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.MinLOD = -FLT_MAX;
        desc.MaxLOD = FLT_MAX;
        desc.BorderColor[0] = 1.0f;
        desc.BorderColor[1] = 1.0f;
        desc.BorderColor[2] = 1.0f;
        desc.BorderColor[3] = 1.0f;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

        HRESULT hr = device->CreateSamplerState(&desc, samplerState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        //	シャドウマップ用
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.BorderColor[0] = FLT_MAX;
        desc.BorderColor[1] = FLT_MAX;
        desc.BorderColor[2] = FLT_MAX;
        desc.BorderColor[3] = FLT_MAX;

        hr = device->CreateSamplerState(&desc, shadowMapSamplerState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        //  ラジアルブラー用
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.MipLODBias = 0;
        desc.MaxAnisotropy = 16;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.BorderColor[0] = FLT_MAX;
        desc.BorderColor[1] = FLT_MAX;
        desc.BorderColor[2] = FLT_MAX;
        desc.BorderColor[3] = FLT_MAX;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        hr = device->CreateSamplerState(&desc, radialblurSamplerState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    }
}

// 描画開始
void FinalpassShader::Begin(const RenderContext& rc)
{
    rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    rc.deviceContext->IASetInputLayout(inputLayout.Get());

    rc.deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
    rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    ID3D11Buffer* constantBuffers[] =
    {
        finalpassConstantBuffer[0].Get(),
        finalpassConstantBuffer[1].Get(),
    };
    rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
    rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);


    const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    rc.deviceContext->OMSetBlendState(blendState.Get(), blend_factor, 0xFFFFFFFF);
    rc.deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    rc.deviceContext->RSSetState(rasterizerState.Get());
    ID3D11SamplerState* samplerStates[] =
    {
        samplerState.Get(),
        shadowMapSamplerState.Get(),
        //depthSamplerState.Get()
        radialblurSamplerState.Get()
    };
    rc.deviceContext->PSSetSamplers(0, ARRAYSIZE(samplerStates), samplerStates);


}

// 描画
void FinalpassShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
    CBFinalpassConstance cbFinalpassConstances;

    cbFinalpassConstances.viewPosition = rc.viewPosition;
    cbFinalpassConstances.screenSize = rc.screenSize;
    cbFinalpassConstances.view = rc.view;
    cbFinalpassConstances.projection = rc.projection;
    DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
    DirectX::XMStoreFloat4x4(&cbFinalpassConstances.inverseView, DirectX::XMMatrixInverse(nullptr, V));
    DirectX::XMStoreFloat4x4(&cbFinalpassConstances.inverseProjection, DirectX::XMMatrixInverse(nullptr, P));

    //cbFinalpassConstances.hueShift = rc.colorGradingData.hueShift;
    //cbFinalpassConstances.saturation = rc.colorGradingData.saturation;
    //cbFinalpassConstances.brightness = rc.colorGradingData.brightness;

    rc.deviceContext->UpdateSubresource(finalpassConstantBuffer[0].Get(), 0, 0, &cbFinalpassConstances, 0, 0);


    RadialBlurConstance radialBlur;
    radialBlur.blur_radius = rc.radialblurData.blur_radius;
    radialBlur.blur_sampling_count = rc.radialblurData.blur_sampling_count;
    radialBlur.blur_center = rc.radialblurData.blur_center;
    radialBlur.blur_mask_radius = rc.radialblurData.blur_mask_radius;

    rc.deviceContext->UpdateSubresource(finalpassConstantBuffer[1].Get(), 0, 0, &radialBlur, 0, 0);


    UINT stride = sizeof(Sprite::Vertex);
    UINT offset = 0;

    rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
    ID3D11ShaderResourceView* srvs[] =
    {
        sprite->GetShaderResourceView().Get(),
        rc.finalpassData.bloomTexture,
        rc.finalpassData.depthMap,
    };
    rc.deviceContext->PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);
    rc.deviceContext->Draw(4, 0);
}

// 描画終了
void FinalpassShader::End(const RenderContext& rc)
{
    rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
    rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
    rc.deviceContext->IASetInputLayout(nullptr);

    ID3D11ShaderResourceView* srvs[] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    rc.deviceContext->PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);
}
