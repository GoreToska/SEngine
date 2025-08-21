cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix inverseView;
    matrix inverseProjection;
}

Texture2D<float4> depthTex : register(t2);
Texture2D<float4> normalTex : register(t3);
Texture2D<float4> diffuseTex : register(t4);
Texture2D<float4> specularTex : register(t5);
Texture2D<float4> nonLinearDepthTex : register(t6);

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 globalPos : POSITION1;
};

Texture2DArray shadowMap : SHADOWMAP : register(t1);
SamplerComparisonState shadowSampler : SHADOWSAMPLER : register(s1);

float4 main(PS_IN input) : SV_Target
{
    float3 diffuse = diffuseTex.Load(int3(input.pos.xy, 0)).xyz;
	diffuse *= float3(1,1,1) * 4;
    return float4(diffuse, 1);
}