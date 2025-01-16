#pragma once

#include "Renderer/BaseRenderer.h"

// �V�F�[�_�[���\�[�X�r���[�̏��
struct ShaderResourceViewData
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	color;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	depth;
	UINT	width = 0;
	UINT	height = 0;
};

// �|�X�g�v���Z�X�p�̃����_���[
class PostprocessingRenderer : public BaseRenderer
{
public:
	PostprocessingRenderer(UINT width, UINT height);
	~PostprocessingRenderer();

	//	�`��
	void PostprocessingRenderer::Render(ID3D11DeviceContext* dc)override;

	// RenderTarget��ShaderResourceView���擾
	const SRVHandleList GetRenderTargetShaderResourceViews() const override;

	// DepthStencil��ShaderResourceView���擾
	const SRVHandle GetDepthStencilShaderResourceView() const override;

	//	�V�[�����̐ݒ�
	void	SetSceneData(ShaderResourceViewData srv);

	//	�f�o�b�O���̕\��
	void	DrawDebugGUI();

    void	radialblurActive(bool active);


private:
	// �V�[���`��f�[�^
	ShaderResourceViewData			sceneData;

	// �`��p�X�v���C�g
	std::unique_ptr<Sprite>			renderSprite;

	// �F���␳�f�[�^
	ColorGradingData				colorGradingData;

	//���W�A���u���[
	RadialBlurData					radialblurData;

	//	���P�x���o�e�N�X�`��
	std::unique_ptr<RenderTarget>	workRenderTarget[2];

};

