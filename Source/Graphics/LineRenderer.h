#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class LineRenderer
{
public:
    LineRenderer(ID3D11Device* device);
    ~LineRenderer() {}

public:
    // �`����s
    void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void DrawLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4& color);


private:
    struct CbMesh
    {
        DirectX::XMFLOAT4X4	wvp;
        DirectX::XMFLOAT4	color;
    };

    struct Line
    {
        DirectX::XMFLOAT3 start;
        DirectX::XMFLOAT3 end;
        DirectX::XMFLOAT4 color;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer>			constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>			lineVertexBuffer;


    Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

    std::vector<Line>       lines;

};
