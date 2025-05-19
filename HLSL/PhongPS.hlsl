#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); //�@���}�b�v
Texture2D shadowMap : register(t2); //�V���h�E�}�b�v

//Texture2D shadowMap[ShadowmapCount] : register(t2); //�V���h�E�}�b�v(�z��ɂ��Ă���̂�ShadowmapCount��
//���W�X�^�[�𗘗p���Ă���ƍl���Ă��������BShadowmapCount = 4�@�̏ꍇ��t2,t3,t4,t5�܂Ŏg���Ă��܂�)

SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
    
    // �@���}�b�v����xyz�������擾����( -1 �` +1 )�̊ԂɃX�P�[�����O
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

    // �ϊ��p��3X3�s���p�ӂ���
	// ���ӓ_�Ƃ��āA���_�V�F�[�_�[���ŎZ�o�����P�ʃx�N�g���ނ̓��X�^���C�Y�̍ۂɐ��l�����_�Ԃŕ�Ԃ���܂��B
	// ���̂��߁A�s�N�Z���V�F�[�_�[�̃^�C�~���O�ł͒P�ʃx�N�g���ł͂Ȃ����̂������Ă���\��������̂ŁA
	// ���K����Y��Ȃ��悤�ɂ��Ă��������B
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

    // �@���}�b�v�Ŏ擾�����@�����ɕϊ��s����|�����킹�܂��傤�B���K���͖Y�ꂸ��
    //normal�}�b�v�����Ă���� 0 ~ 1 �����Ȃ��A�g�������Ȃ� -1 ~ 1 �ɕϊ�
    float3 N = normalize(mul(normal * 2.0f - 1.0f, CM));
    
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);
    
    //�}�e���A���萔
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(0, 0, 0);
    float shiness = 128;
    
    
	// �����̌v�Z
    float3 ambient = ka.rgb * ambientLightColor.rgb/* * diffuseColor.rgb*/;	

    float3 diffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    float3 specular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

    // ���s�����̉e�Ȃ̂ŁA���s�����ɑ΂��ĉe��K��
    float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
    
    diffuse *= shadow;
    specular *= shadow;
            
    return float4((diffuseColor.rgb * (ambient + diffuse) + specular), diffuseColor.a);
    
}