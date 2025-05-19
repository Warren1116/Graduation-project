#include "Finalpass.hlsli"



// �V�[���e�N�X�`��
Texture2D sceneTexture : register(t0);

// �u���[���e�N�X�`��
Texture2D bloomTexture : register(t1);

// �V�[���T���v���X�e�[�g
SamplerState sceneSampler : register(s0);

//
Texture2D<float> DepthMap : register(t2);


float4 main(VS_OUT pin) : SV_TARGET
{  
    float2 scene_map_size;
    sceneTexture.GetDimensions(scene_map_size.x, scene_map_size.y);
    
    float4 color = sceneTexture.Sample(sceneSampler, pin.texcoord);
    float4 result_color = color;
    
    float2 blur_vector = (blur_center - pin.texcoord);
    blur_vector *= (blur_radius / scene_map_size.xy) / blur_sampling_count;
    for (int index = 1; index < blur_sampling_count; ++index)
    {
        result_color += sceneTexture.Sample(sceneSampler, pin.texcoord + blur_vector * index);
    }
    
      //�w��͈͓̔��͓K���ʂ�ς���
    float mask_radius = blur_mask_radius / min(scene_map_size.x, scene_map_size.y);
    float mask_value = saturate(length(pin.texcoord - blur_center) / mask_radius);
    color = lerp(color, result_color / blur_sampling_count, mask_value);

     //�u���[���e�N�X�`�������Z����
    color.rgb += bloomTexture.Sample(sceneSampler, pin.texcoord).rgb;

      //�[�x�l�����
    float depth = DepthMap.Sample(sceneSampler, pin.texcoord).r;
    
      //NDC���W�Ɛ[�x�l���烏�[���h���W�ɕϊ�����
    float4 ndc = float4(pin.texcoord.xy, depth, 1);
    ndc.x = ndc.x * 2 - 1;
    ndc.y = ndc.y * -2 + 1;
    float4 pos = mul(ndc, mul(inverseProjection, inverseView));
    pos /= pos.w;
      //�J��������s�N�Z�����W�ւ̋��������߂�
    depth = length(pos.xyz - viewPosition.xyz);
   
    return color;
    
}

