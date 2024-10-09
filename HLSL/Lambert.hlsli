struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer CbScene : register(b0)
{
    //ÉJÉÅÉâèÓïÒ
    float4 viewPosition;
	row_major float4x4	viewProjection;
	float4				lightDirection;
    
    row_major float4x4 projection;
    row_major float4x4 inverseView;
    row_major float4x4 inverseProjection;


};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};


cbuffer CBFog : register(b3)
{
    float3 fogColor;
	
    float fogDensity;
    float fogHeightFalloff;
    float fogCutoffDistance;
    float groundLevel;

    float dummy;
}