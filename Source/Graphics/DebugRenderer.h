#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class DebugRenderer
{
public:
    DebugRenderer(ID3D11Device* device);
    ~DebugRenderer() {}

public:
    // ï`âÊé¿çs
    void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    // ãÖï`âÊ
    void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);

    // â~íåï`âÊ
    void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);

    // éläpíåï`âÊ
    void DrawSquare(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT4& color);

    // î†ï`âÊ
    void DrawBox(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT4& color);


private:
    // ãÖÉÅÉbÉVÉÖçÏê¨
    void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);

    // â~íåÉÅÉbÉVÉÖçÏê¨
    void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);

    // éläpíåÉÅÉbÉVÉÖçÏê¨
    void CreateSquareMesh(ID3D11Device* device);

    // î†ÉÅÉbÉVÉÖçÏê¨
    void CreateBoxMesh(ID3D11Device* device, float width, float height, float depth);


private:
    struct CbMesh
    {
        DirectX::XMFLOAT4X4	wvp;
        DirectX::XMFLOAT4	color;
    };

    struct Sphere
    {
        DirectX::XMFLOAT4	color;
        DirectX::XMFLOAT3	center;
        float				radius;
    };

    struct Cylinder
    {
        DirectX::XMFLOAT4	color;
        DirectX::XMFLOAT3	position;
        float				radius;
        float				height;
    };

    struct Square
    {
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 scale;
    };

    struct Arrow
    {
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 position;
        float			  radius;
        float			  height;
    };

    struct Box
    {
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 angle;
        DirectX::XMFLOAT3 size;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer>			sphereVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinderVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>			squareVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>			arrowVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>			boxVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>			constantBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

    std::vector<Sphere>		spheres;
    std::vector<Cylinder>	cylinders;
    std::vector<Square>		squares;
    std::vector<Arrow>		arrows;
    std::vector<Box>		boxs;

    UINT	sphereVertexCount = 0;
    UINT	cylinderVertexCount = 0;
    UINT	squareVertexCount = 0;
    UINT	arrowVertexCount = 0;
    UINT	boxVertexCount = 0;
};
