float4 main(float4 pos : SV_POSITION, float life : LIFE) : SV_TARGET
{
    return float4(1, 1, 1, life);
}
