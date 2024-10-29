#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <vector>

class GeometricPrimitive
{
public:
	GeometricPrimitive(ID3D11Device* device);
	virtual ~GeometricPrimitive() = default;

	void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	void GeometricPrimitive::DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);


	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	struct constants
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 material_color;
	};

	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4	color;
	};

private:
	void GeometricPrimitive::CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);


	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;

	ComPtr<ID3D11VertexShader> vertex_shader;
	ComPtr<ID3D11PixelShader> pixel_shader;
	ComPtr<ID3D11InputLayout> input_layout;
	ComPtr<ID3D11Buffer> constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cylinderVertexBuffer;
	std::vector<Cylinder>	cylinders;
	UINT	cylinderVertexCount = 0;


protected:
	void create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
		uint32_t* indices, size_t index_count);

};

class Geometric_cube : public GeometricPrimitive
{
public:
	Geometric_cube(ID3D11Device* device);
};

class Geometric_cylinder : public GeometricPrimitive
{
public:
	Geometric_cylinder(ID3D11Device* device, int segment);
};

class Geometric_sphere : public GeometricPrimitive
{
public:
	Geometric_sphere(ID3D11Device* device, uint32_t slices, uint32_t stacks);
};