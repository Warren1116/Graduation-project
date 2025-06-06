#include <stdio.h>
#include <memory>
#include "Misc.h"
#include "Graphics/DebugRenderer.h"

DebugRenderer::DebugRenderer(ID3D11Device* device)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\DebugVS.cso", "rb");
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
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\DebugPS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
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
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbMesh);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
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
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 球メッシュ作成
	CreateSphereMesh(device, 1.0f, 16, 16);

	// 円柱メッシュ作成
	CreateCylinderMesh(device, 1.0f, 1.0f, 0.0f, 1.0f, 16, 1);

	// 四角柱メッシュ作成
	CreateSquareMesh(device);

	// 箱メッシュ作成
	CreateBoxMesh(device, 1.0f, 1.0f, 1.0f);

	CreateTriangleMesh(device);

	//
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(DirectX::XMFLOAT3) * 2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, lineVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

}

// 描画開始
void DebugRenderer::Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// シェーダー設定
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);
	context->IASetInputLayout(inputLayout.Get());

	// 定数バッファ設定
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	//context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// レンダーステート設定
	const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);
	context->RSSetState(rasterizerState.Get());

	// ビュープロジェクション行列作成
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;

	// プリミティブ設定
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 球描画
	context->IASetVertexBuffers(0, 1, sphereVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const Sphere& sphere : spheres)
	{
		// ワールドビュープロジェクション行列作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sphere.radius, sphere.radius, sphere.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(sphere.center.x, sphere.center.y, sphere.center.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CbMesh cbMesh;
		cbMesh.color = sphere.color;
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, WVP);

		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(sphereVertexCount, 0);
	}
	spheres.clear();

	// 円柱描画
	context->IASetVertexBuffers(0, 1, cylinderVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const Cylinder& cylinder : cylinders)
	{
		// ワールドビュープロジェクション行列作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cylinder.radius, cylinder.height, cylinder.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cylinder.position.x, cylinder.position.y, cylinder.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CbMesh cbMesh;
		cbMesh.color = cylinder.color;
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, WVP);

		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(cylinderVertexCount, 0);
	}
	cylinders.clear();

	// 四角柱描画
	context->IASetVertexBuffers(0, 1, squareVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const Square& square : squares)
	{
		// ワールドプロジェクション行列
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(square.scale.x, square.scale.y, square.scale.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(square.position.x, square.position.y, square.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CbMesh cbMesh;
		cbMesh.color = square.color;
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, WVP);

		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(squareVertexCount, 0);
	}
	squares.clear();

	// 矢印描画
	context->IASetVertexBuffers(0, 1, arrowVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const Arrow& arrow : arrows)
	{
		// ワールドプロジェクション行列
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(arrow.radius, arrow.radius, arrow.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(arrow.position.x, arrow.position.y, arrow.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CbMesh cbMesh;
		cbMesh.color = arrow.color;
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, WVP);

		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(squareVertexCount, 0);
	}
	arrows.clear();

	// 箱描画
	context->IASetVertexBuffers(0, 1, boxVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const Box& box : boxs)
	{
		// ワールドプロジェクション行列
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(box.size.x, box.size.y, box.size.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(box.position.x, box.position.y, box.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CbMesh cbMesh;
		cbMesh.color = box.color;
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, WVP);

		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(boxVertexCount, 0);
	}
	boxs.clear();

	//三角形描画
	context->IASetVertexBuffers(0, 1, triangleVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetInputLayout(inputLayout.Get());
	for (const Triangle& triangle : triangles)
	{

		UINT stride = sizeof(DirectX::XMFLOAT3);
		UINT offset = 0;

		context->IASetIndexBuffer(triangleIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->DrawIndexed(3, 0, 0);
	}
	triangles.clear();


    // 線描画
	context->IASetVertexBuffers(0, 1, lineVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const auto& line : lines)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = context->Map(lineVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		if (SUCCEEDED(hr))
		{
			DirectX::XMFLOAT3* vertexData = reinterpret_cast<DirectX::XMFLOAT3*>(mappedResource.pData);
			vertexData[0] = line.start;
			vertexData[1] = line.end;
			context->Unmap(lineVertexBuffer.Get(), 0);
		}

		CbMesh cbMesh;
		cbMesh.color = line.color;
		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, identity * VP);
		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);


		context->Draw(2, 0);
	}
	lines.clear();
}

// 球描画
void DebugRenderer::DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color)
{
	Sphere sphere;
	sphere.center = center;
	sphere.radius = radius;
	sphere.color = color;
	spheres.emplace_back(sphere);
}

// 円柱描画
void DebugRenderer::DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
	Cylinder cylinder;
	cylinder.position = position;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.color = color;
	cylinders.emplace_back(cylinder);
}

// 四角柱描画
void DebugRenderer::DrawSquare(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT4& color)
{
	Square square;
	square.position = position;
	square.scale = scale;
	square.color = color;
	squares.push_back(square);
}


void DebugRenderer::DrawBox(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT4& color)
{
	Box box;
	box.position = position;
	box.angle = angle;
	box.size = size;
	box.color = color;
	boxs.emplace_back(box);
}

void DebugRenderer::DrawTriangle(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2, const DirectX::XMFLOAT3& v3, const DirectX::XMFLOAT4& color)
{
	Triangle triangle;
	triangle.v1 = v1;
	triangle.v2 = v2;
	triangle.v3 = v3;
	triangle.color = color;

	triangles.push_back(triangle);
}

void DebugRenderer::DrawLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4& color)
{
	lines.push_back({ start, end, color });
}





// 球メッシュ作成
void DebugRenderer::CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks)
{
	sphereVertexCount = stacks * slices * 2 + slices * stacks * 2;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(sphereVertexCount);

	float phiStep = DirectX::XM_PI / stacks;
	float thetaStep = DirectX::XM_2PI / slices;

	DirectX::XMFLOAT3* p = vertices.get();
	
	for (int i = 0; i < stacks; ++i)
	{
		float phi = i * phiStep;
		float y = radius * cosf(phi);
		float r = radius * sinf(phi);

		for (int j = 0; j < slices; ++j)
		{
			float theta = j * thetaStep;
			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;

			theta += thetaStep;

			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;
		}
	}

	thetaStep = DirectX::XM_2PI / stacks;
	for (int i = 0; i < slices; ++i)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(i * thetaStep);
		for (int j = 0; j < stacks; ++j)
		{
			float theta = j * thetaStep;
			DirectX::XMVECTOR V1 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
			DirectX::XMStoreFloat3(p++, P1);

			int n = (j + 1) % stacks;
			theta += thetaStep;

			DirectX::XMVECTOR V2 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
			DirectX::XMStoreFloat3(p++, P2);
		}
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * sphereVertexCount);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, sphereVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// 円柱メッシュ作成
void DebugRenderer::CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks)
{
	cylinderVertexCount = 2 * slices * (stacks + 1) + 2 * slices;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(cylinderVertexCount);

	DirectX::XMFLOAT3* p = vertices.get();

	float stackHeight = height / stacks;
	float radiusStep = (radius2 - radius1) / stacks;

	// vertices of ring
	float dTheta = DirectX::XM_2PI / slices;

	for (int i = 0; i < slices; ++i)
	{
		int n = (i + 1) % slices;

		float c1 = cosf(i * dTheta);
		float s1 = sinf(i * dTheta);

		float c2 = cosf(n * dTheta);
		float s2 = sinf(n * dTheta);

		for (int j = 0; j <= stacks; ++j)
		{
			float y = start + j * stackHeight;
			float r = radius1 + j * radiusStep;

			p->x = r * c1;
			p->y = y;
			p->z = r * s1;
			p++;

			p->x = r * c2;
			p->y = y;
			p->z = r * s2;
			p++;
		}

		p->x = radius1 * c1;
		p->y = start;
		p->z = radius1 * s1;
		p++;

		p->x = radius2 * c1;
		p->y = start + height;
		p->z = radius2 * s1;
		p++;
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * cylinderVertexCount);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, cylinderVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// 四角柱メッシュ作成
void DebugRenderer::CreateSquareMesh(ID3D11Device* device)
{
	squareVertexCount = 24;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(squareVertexCount);

	DirectX::XMFLOAT3* p = vertices.get();

	uint32_t face = 0;

	face = 0;
	vertices[face * 4 + 0] = { -0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 1] = { +0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2] = { -0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 3] = { +0.5f, +0.5f, -0.5f };

	face += 1;
	vertices[face * 4 + 0] = { -0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 1] = { +0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 2] = { -0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3] = { +0.5f, -0.5f, -0.5f };

	face += 1;
	vertices[face * 4 + 0] = { -0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 1] = { +0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 2] = { -0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3] = { +0.5f, -0.5f, -0.5f };

	face += 1;
	vertices[face * 4 + 0] = { -0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 1] = { +0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2] = { -0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 3] = { +0.5f, -0.5f, +0.5f };

	face += 1;
	vertices[face * 4 + 0] = { +0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 1] = { +0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2] = { +0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3] = { +0.5f, -0.5f, +0.5f };

	face += 1;
	vertices[face * 4 + 0] = { -0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 1] = { -0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2] = { -0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3] = { -0.5f, -0.5f, +0.5f };

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc{};
		D3D11_SUBRESOURCE_DATA subresourceData{};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * squareVertexCount);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, squareVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// 箱メッシュ作成
void DebugRenderer::CreateBoxMesh(ID3D11Device* device, float width, float height, float depth)
{
	boxVertexCount = 36;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(boxVertexCount);

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	DirectX::XMFLOAT3 v[8] = {
		{ -halfWidth, -halfHeight, -halfDepth },	//左下後
		{ -halfWidth, +halfHeight, -halfDepth },	//左上後
		{ +halfWidth, +halfHeight, -halfDepth },	//右上後
		{ +halfWidth, -halfHeight, -halfDepth },	//右下後
		{ -halfWidth, -halfHeight, +halfDepth },	//左下前
		{ -halfWidth, +halfHeight, +halfDepth },	//左上前
		{ +halfWidth, +halfHeight, +halfDepth },	//右上前
		{ +halfWidth, -halfHeight, +halfDepth },	//右下前
	};

	int indices[36] = {
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7,
	};

	for (int i = 0; i < boxVertexCount; ++i)
	{
		vertices[i] = v[indices[i]];
	}

	D3D11_BUFFER_DESC desc = {};
	D3D11_SUBRESOURCE_DATA subresourceData = {};

	desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * boxVertexCount);
	desc.Usage = D3D11_USAGE_IMMUTABLE; 
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	subresourceData.pSysMem = vertices.get();

	HRESULT hr = device->CreateBuffer(&desc, &subresourceData, boxVertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr)); 
}

void DebugRenderer::CreateTriangleMesh(ID3D11Device* device)
{
	DirectX::XMFLOAT3 vertices[] = {
		{ 0.0f,  1.0f, 0.0f },  
		{ 1.0f, -1.0f, 0.0f },  
		{ -1.0f, -1.0f, 0.0f },
	};

	DWORD indices[] = { 0, 1, 2 };

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;

	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, triangleVertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;

	hr = device->CreateBuffer(&indexBufferDesc, &indexData, triangleIndexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}
