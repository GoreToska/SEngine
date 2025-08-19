struct VS_IN
{
    float4 pos : POSITION0;
    float4 norm : NORMAL;
    float4 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 norm : NORMAL;
    float4 tex : TEXCOORD0;
    float4 viewPos : POSITION0;
};

cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix inverseView;
    matrix inverseProjection;
};

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    input.pos.w = 1.0f;

    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.viewPos = mul(input.pos, world);
    output.viewPos = mul(output.viewPos, view);

    output.tex.xy = input.tex.xy;

    output.norm = normalize(mul(float4(input.norm.xyz, 0), world));

    return output;
}