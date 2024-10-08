#ifndef	__CONSTANTS_HLSLI__
#define	__CONSTANTS_HLSLI__
// ‰~Žü—¦
static const float PI = 3.141592654f;

#endif // #endif __CONSTANTS_HLSLI__


cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;
    float4 light_direction;
    float4 camera_position;
	// SHADOW
    row_major float4x4 light_view_projection;
	// FOG
    row_major float4x4 inverse_view_projection;
    float time;
};

cbuffer FOG_CONSTANT_BUFFER : register(b2)
{
    float4 _fog_color;
	
    float _fog_density;
    float _fog_height_falloff;
    float _ground_level;
    float _fog_cutoff_distance;

    float _mie_scattering_coef;
	
    bool _enable_dither;
    bool _enable_blur;

    float _time_scale;
    float _noise_scale;
};
