struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 norm : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 tex : TEXCOORD0;
};

cbuffer material : register(b0)
{
	float4 diffuseColor;
    float4 specularColor;
    float4 emissiveColor;
	bool normalMapEnabled;
    float shininess;
}

Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
Texture2D normalTexture : register(t2);
SamplerState samplerState : register(s0);

struct GBuffer
{
    float4 depth : SV_Target0;
    float4 normal : SV_Target1;
    float4 diffuse : SV_Target2;
    float4 specular : SV_Target3;
};

float3 MapNormal(
    const in float3 tan,
    const in float3 bitan,
    const in float3 normal,
    const in float2 tc,
    uniform Texture2D nmap,
    uniform SamplerState splr)
{
    const float3x3 tanToTarget = float3x3(tan, bitan, normal);
    float3 normalSample = nmap.Sample(splr, tc).xyz;
	//normalSample.y = -normalSample.y;
    const float3 tanNormal = normalSample * 2.0f - 1.0f;
    return normalize(mul(tanNormal, tanToTarget));
}

[earlydepthstencil]
GBuffer main(PS_IN input)
{
    GBuffer buf = (GBuffer) 0;

    // TODO: move this values to const buffer
    // ------------------
    float nearPlane = 0.1;
    float farPlane = 3000;
    // ------------------

    [branch]
    if(normalMapEnabled)
    {
        const float3 mappedNormal = MapNormal(normalize(input.tangent),
        normalize(input.bitangent), input.norm, input.tex, normalTexture, samplerState);

        // TODO: make normal map weight
        //buf.normal.xyz = lerp(input.norm, mappedNormal, weight);
        buf.normal.xyz = mappedNormal;
    }
    else
    {
        buf.normal.xyz = input.norm;
    }

    float depth = (input.viewPos.z - nearPlane) / (farPlane - nearPlane);
    float4 textureColor = diffuseTexture.Sample(samplerState, input.tex);

    buf.depth.xyz = float3(depth, depth, depth);
    buf.diffuse =  diffuseColor * textureColor;
    buf.specular = specularColor;

    return buf;
}