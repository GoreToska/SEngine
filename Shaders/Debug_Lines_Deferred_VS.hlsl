struct VSInput
{
    float3 pos : POSITION;
};

struct PSInput
{
    float4 posH : SV_POSITION;
};

cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix worldView;
    matrix viewProjection;
    matrix worldViewProjection;
    matrix inverseWorld;
    matrix inverseView;
    matrix inverseProjection;
};

PSInput main(VSInput input)
{
    PSInput output;
    output.posH = mul(float4(input.pos, 1.0f), worldViewProjection);
    return output;
}