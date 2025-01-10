#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//	���s�������
struct DirectionalLightData
{
    DirectX::XMFLOAT4	direction;	// ����
    DirectX::XMFLOAT4	color;		// �F
};

// �_�������
struct PointLightData
{
    DirectX::XMFLOAT4	position;	// ���W
    DirectX::XMFLOAT4	color;		// �F
    float				range;		// �͈�
    float				power;
    DirectX::XMFLOAT2	dummy;
};

// �_�����̍ő吔
static	constexpr	int	PointLightMax = 8;

// �X�|�b�g���C�g���
struct SpotLightData
{
    DirectX::XMFLOAT4	position;	// ���W
    DirectX::XMFLOAT4	direction;	// ����
    DirectX::XMFLOAT4	color;		// �F
    float				range;		// �͈�
    float				power;
    float				innerCorn; 	// �C���i�[�p�x�͈�
    float				outerCorn; 	// �A�E�^�[�p�x�͈�
};

// �X�|�b�g���C�g�̍ő吔
static	constexpr	int	SpotLightMax = 8;

// �F���␳���
struct ColorGradingData
{
    float	hueShift = 0;	// �F������
    float	saturation = 1;	// �ʓx����
    float	brightness = 1;	// ���x����
    float	dummy;
};


// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxKernelSize = 16;

struct FogData
{

    DirectX::XMFLOAT3 fogColor = { 1.0f,1.0f,1.0f };
    //float fogIntensity = 0.02f;
    float fogDensity = 0.02f;
    float fogHeightFalloff = 0.05f;
    float fogCutoffDistance = 500.0f;
    float groundLevel = 0.0f;
    //float mieScatteringCoef;
    //float timeScale = 0.35f;
    //float noiseScale = 0.2f;

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
    //ID3D11ShaderResourceView* shadowMap;		//�V���h�E�}�b�v�e�N�X�`��
    //DirectX::XMFLOAT4X4 lightViewProjection;	//���C�g�r���[�v���W�F�N�V�����s��
    //DirectX::XMFLOAT3 shadowColor;				//�e�̐F
    //float shadowBias;							//�[�x��r�p�̃I�t�Z�b�g�l


    ID3D11ShaderResourceView* shadowMap[ShadowmapCount];				//	�V���h�E�}�b�v�e�N�X�`��
    DirectX::XMFLOAT4X4			lightViewProjection[ShadowmapCount];	//	���C�g�r���[�v���W�F�N�V�����s��
    DirectX::XMFLOAT3			shadowColor;			//	�e�̐F
    float						shadowBias[ShadowmapCount];				//	�[�x��r�p�̃I�t�Z�b�g�l
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
    SpotLightData				spotLightData[SpotLightMax];	// �X�|�b�g���C�g���
    int							spotLightCount = 0;				// �X�|�b�g���C�g��

    //	�F���␳���
    ColorGradingData			colorGradingData;

    //	�ŏI�p�X���
    FinalpassData				finalpassData;

    //	�V���h�E�}�b�v���
    ShadowMapData				shadowMapData;

    //	�X�J�C�{�b�N�X���
    SkyboxData					skyboxData;


};
