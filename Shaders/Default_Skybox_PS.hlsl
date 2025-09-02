#include "Common.hlsli"

struct PS_IN
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD0;
    float3 worldPos : POSITION;
};


TextureCube skyboxTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(PS_IN input): SV_TARGET
{
    float4 color = skyboxTexture.Sample(samplerState, input.worldPos);
    //return float4(0.53 + color.x,0.8 + color.y,0.98 + color.z,1);
    return skyboxTexture.Sample(samplerState, input.worldPos);
}