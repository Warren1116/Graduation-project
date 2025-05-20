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

};
