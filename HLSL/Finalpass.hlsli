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
    float4 screenSize; // xy : サイズ, z : nearクリップ, w : farクリップ
	//	カメラ情報
    float4 viewPosition;
    row_major float4x4 view;
    row_major float4x4 projection;
    row_major float4x4 inverseView;
    row_major float4x4 inverseProjection;
	//	色調補正
    float hueShift; // 色相調整
    float saturation; // 彩度調整
    float brightness; // 明度調整
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

