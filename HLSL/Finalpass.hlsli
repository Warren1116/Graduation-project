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
	//	�F���␳
    float hueShift; // �F������
    float saturation; // �ʓx����
    float brightness; // ���x����
    float colorGradiationDummy;
	
};

cbuffer CBFog : register(b1)
{
    float3 fogColor;
    //float fogIntensity;
	
    float fogDensity;
    float fogHeightFalloff;
    float fogCutoffDistance;
    float groundLevel;

    //float mieScatteringCoef;
    //float timeScale;
    //float noiseScale;
    float dummy;
}

