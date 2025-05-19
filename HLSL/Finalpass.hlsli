#include "FilterFunctions.hlsli"
#include "Light.hlsli"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
    float depth : DEPTH;
};

cbuffer CBFinalpass : register(b0)
{
    float4 screenSize; // xy : �T�C�Y, z : near�N���b�v, w : far�N���b�v
	//	�J�������
    float4 viewPosition;
    row_major float4x4 view;
    row_major float4x4 projection;
    row_major float4x4 inverseView;
    row_major float4x4 inverseProjection;	
};


// ���W�A���u���[�p�̒萔�o�b�t�@
cbuffer RADIAL_BLUR_CONSTANT_BUFFER : register(b1)
{
    float blur_radius;
    int blur_sampling_count;
    float2 blur_center;
    float blur_mask_radius;
    float3 blur_dummy;
}