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
};


// ラジアルブラー用の定数バッファ
cbuffer RADIAL_BLUR_CONSTANT_BUFFER : register(b1)
{
    float blur_radius;
    int blur_sampling_count;
    float2 blur_center;
    float blur_mask_radius;
    float3 blur_dummy;
}