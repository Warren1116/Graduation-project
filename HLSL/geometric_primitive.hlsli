#include "scene_constant_buffer.hlsli"

#ifndef __GEOMETRIC_PRIMITIVE_HLSLI__
#define __GEOMETRIC_PRIMITIVE_HLSLI__

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 material_color;
};

#endif // __GEOMETRIC_PRIMITIVE_HLSLI__
