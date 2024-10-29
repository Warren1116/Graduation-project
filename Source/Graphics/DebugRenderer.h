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
	// •`‰æÀs
	void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// ‹…•`‰æ
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);

	// ‰~’Œ•`‰æ
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);

	// lŠp’Œ•`‰æ
	void DrawSquare(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT4& color);

	// –îˆó•`‰æ
	void DrawArrow(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);



private:
	// ‹…ƒƒbƒVƒ…ì¬
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);

	// ‰~’ŒƒƒbƒVƒ…ì¬
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);

	// lŠp’ŒƒƒbƒVƒ…ì¬
	void CreateSquareMesh(ID3D11Device* device);

	// –îˆóƒƒbƒVƒ…ì¬
	void CreateArrowMesh(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);


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

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sphereVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinderVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			squareVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			arrowVertexBuffer;
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

	UINT	sphereVertexCount = 0;
	UINT	cylinderVertexCount = 0;
	UINT  squareVertexCount = 0;
	UINT arrowVertexCount = 0;
};
