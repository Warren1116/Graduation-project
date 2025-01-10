#pragma once

#include "Renderer/ModelRenderer.h"

// �V���h�E�}�b�v�����_���[
class ShadowmapCasterRenderer : public ModelRenderer
{
	static constexpr float Near = 1.0f;
	static constexpr float Far = 10000.0f;
public:
	ShadowmapCasterRenderer(UINT shadowmapSize);
	~ShadowmapCasterRenderer();

	// �V���h�E�}�b�v����RenderContext�ɐς�
	ShadowMapData GetShadowMapData();

	static ShadowmapCasterRenderer& Instance()
	{
		static ShadowmapCasterRenderer* instance;
		return *instance;
	}

public:
	//	�`����s
	void Render(ID3D11DeviceContext* dc) override;

	//	�f�o�b�OGUI�`��
	void DrawDebugGUI() override;

	//	RenderTarget��ShaderResourceView���擾
	const SRVHandleList GetRenderTargetShaderResourceViews() const override;

	//	DepthStencil��ShaderResourceView���擾
	const SRVHandle GetDepthStencilShaderResourceView() const override;

private:
	//	�V���h�E�}�b�v�Ŏg�p����e�s����Z�o
	void CalcShadowmapMatrix(DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection);


private:
	//std::unique_ptr<DepthStencil>	depthStencil;						// �V���h�E�}�b�v�p�[�x�o�b�t�@
	float							shadowRect = 500.0f;				// �`��͈�
	//DirectX::XMFLOAT3				shadowColor = { 0.2f, 0.2f, 0.2f };	// �e�̐F
	//float							shadowBias = 0.0001f;				// �[�x��r�p�̃I�t�Z�b�g�l

	std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
	DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
	float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };


};

