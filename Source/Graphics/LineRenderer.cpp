#include <stdio.h>
#include <memory>
#include "Misc.h"
#include "Graphics/LineRenderer.h"

LineRenderer::LineRenderer(ID3D11Device* device)
{
	// ���_�V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\LineVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ���_�V�F�[�_�[����
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �s�N�Z���V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\LinePS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// �s�N�Z���V�F�[�_�[����
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
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

	// �u�����h�X�e�[�g
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

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
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

// �`��J�n
void LineRenderer::Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// �V�F�[�_�[�ݒ�
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);
	context->IASetInputLayout(inputLayout.Get());

	// �萔�o�b�t�@�ݒ�
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	//context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// �����_�[�X�e�[�g�ݒ�
	const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);
	context->RSSetState(rasterizerState.Get());

	// �r���[�v���W�F�N�V�����s��쐬
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;

	// �v���~�e�B�u�ݒ�
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	// ���`��
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

void LineRenderer::DrawLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4& color)
{
	lines.push_back({ start, end, color });
}

