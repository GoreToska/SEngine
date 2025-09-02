#include "Common.hlsli"

cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix inverseView;
    matrix inverseProjection;
};

struct VS_IN
{
    float3 position : POSITION;
};

struct PS_IN
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD0;
    float3 worldPos : POSITION;
};

PS_IN main(VS_IN input)
{
    PS_IN output;

    output.worldPos = input.position;
    float4 pos = float4(input.position, 0.0f);

    output.position = mul(pos, view);
    output.position = mul(output.position, projection);
    output.position.z = output.position.w; // make shure depth is 1.0
    output.texCoord = input.position;

    return output;
}