#pragma once

#include "Renderer/ModelRenderer.h"
#include "Graphics/geometric_primitive.h"

// �V�[�������_���[
class SceneRenderer : public ModelRenderer
{
public:
	SceneRenderer(UINT width, UINT height);
	~SceneRenderer() {}

	// �V���h�E�}�b�v����RenderContext�ɐς�
	void SetShadowmapData(ShadowMapData shadowMapData) { this->shadowMapData = shadowMapData; }

	static SceneRenderer& Instance()
	{
		static SceneRenderer* instance;
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
	std::unique_ptr<RenderTarget>	renderTarget;		//	�I�t�X�N���[�������_�����O�p�`��o�b�t�@
	std::unique_ptr<DepthStencil>	depthStencil;		//	�I�t�X�N���[�������_�����O�p�[�x�o�b�t�@
	std::unique_ptr<Sprite>			skyboxRenderer;		//	��`��p�X�v���C�g
	ShadowMapData					shadowMapData;		//	�V���h�E�}�b�v���
	bool							drawDebugPrimitive;	//	�f�o�b�O�v���~�e�B�u�`��
	int								timer;				//���Ԍo��

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;
private:
	//	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	std::unique_ptr<DepthStencil> shadowmapDepthStencil[ShadowmapCount];
	//	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT4X4 lightViewProjection[ShadowmapCount];
	//	�[�x��r�p�̃I�t�Z�b�g�l
	float shadowBias[ShadowmapCount] = { 0.001f,0.002f,0.004f,0.01f };
	//	�e�̐F
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };


};
