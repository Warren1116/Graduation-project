#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//	���s�������
struct DirectionalLightData
{
    DirectX::XMFLOAT4	direction;	// ����
    DirectX::XMFLOAT4	color;		// �F
};

// ���W�A���u���[���
struct RadialBlurData
{
    float blur_radius = 0.0f;
    int blur_sampling_count = 10;
    DirectX::XMFLOAT2 blur_center = { 0.5f,0.5f };
    float blur_mask_radius = 500;		//	center����̎w��͈̔͂̓u���[��K�����Ȃ��悤�ɂ���
    DirectX::XMFLOAT3  blur_dummy;
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassData
{
    //	�u���[���e�N�X�`��
    ID3D11ShaderResourceView* bloomTexture;
    //	�J���[�e�N�X�`��
    ID3D11ShaderResourceView* colorMap;
    //	�[�x�e�N�X�`��
    ID3D11ShaderResourceView* depthMap;

};

//�V���h�E�}�b�v�̏��
static const int ShadowmapCount = 4;

//�V���h�E�}�b�v�p���
struct ShadowMapData
{
    ID3D11ShaderResourceView* shadowMap;		//�V���h�E�}�b�v�e�N�X�`��
    DirectX::XMFLOAT4X4 lightViewProjection;	//���C�g�r���[�v���W�F�N�V�����s��
    DirectX::XMFLOAT3 shadowColor;				//�e�̐F
    float shadowBias;							//�[�x��r�p�̃I�t�Z�b�g�l
};

struct SkyboxData
{
    ID3D11ShaderResourceView* skybox;					//	��e�N�X�`��
};


// �����_�[�R���e�L�X�g
struct RenderContext
{
    ID3D11DeviceContext* deviceContext;

    float						timer;		//	�^�C�}�[

    DirectX::XMFLOAT4			screenSize;	//	xy : �X�N���[���̃T�C�Y, z : near�N���b�v�����Aw : far�N���b�v����

    //	�J�������
    DirectX::XMFLOAT4			viewPosition;
    DirectX::XMFLOAT4X4			view;
    DirectX::XMFLOAT4X4			projection;

    //	���C�g���
    DirectX::XMFLOAT4			ambientLightColor;				// �������
    DirectionalLightData		directionalLightData;			// ���s�������

    //	�ŏI�p�X���
    FinalpassData				finalpassData;

    //	�V���h�E�}�b�v���
    ShadowMapData				shadowMapData;

    //	�X�J�C�{�b�N�X���
    SkyboxData					skyboxData;

    //���W�A���u���[���
    RadialBlurData            radialblurData;

};
