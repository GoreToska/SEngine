struct VS_IN
{
    float3 pos : POSITION0;
    float3 norm : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 norm : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 tex : TEXCOORD0;
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

PS_IN main(VS_IN input)
{
	PS_IN output = (PS_IN) 0;

 	output.pos = mul(float4(input.pos, 1), world);
 	output.pos = mul(output.pos, view);
 	output.pos = mul(output.pos, projection);

    output.viewPos = mul(float4(input.pos, 1), world);
    output.viewPos = mul(output.viewPos, view);

    output.tex = input.tex;

    output.norm = float4(mul(input.norm.xyz, (float3x3)world), 0);
    output.tangent = mul(input.tangent, (float3x3)world);
    output.bitangent = mul(input.bitangent, (float3x3)world);

    output.norm = normalize(output.norm);
    output.tangent = normalize(output.tangent);
    output.bitangent = normalize(output.bitangent);

    return output;
}