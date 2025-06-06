#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class PhongShader : public ModelShader
{
public:
	PhongShader(ID3D11Device* device);
	~PhongShader() override {}

	void Begin(const RenderContext& rc) override;
	void SetBuffers(const RenderContext& rc, const std::vector<Model::Node>& nodes, const ModelResource::Mesh& mesh) override;
	void DrawSubset(const RenderContext& rc, const ModelResource::Subset& subset) override;

	void Draw(const RenderContext& rc, const Model* model) override;
	void End(const RenderContext& rc) override;

private:
	static const int MaxBones = 128;

	struct CbScene
	{
		DirectX::XMFLOAT4 viewPosition;
		DirectX::XMFLOAT4X4 viewProjection;
		DirectX::XMFLOAT4 ambientLightColor;
		DirectionalLightData directionalLightData; //平行光源情報
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
		DirectX::XMFLOAT4	ka;
		DirectX::XMFLOAT4	kd;
		DirectX::XMFLOAT4	ks;
		float				shiness;
		DirectX::XMFLOAT3	dummy;
	};

	struct CbShadowMap
	{
		DirectX::XMFLOAT4X4 lightViewProjection;  //ライトビュープロジェクション行列
		DirectX::XMFLOAT3 shadowColor;            //影の色
		float shadowBias;                         //深度比較用のオフセット値
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			meshConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			shadowMapConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		shadowMapSamplerState;
};
