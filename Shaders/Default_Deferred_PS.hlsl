struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 norm : NORMAL;
    float4 tex : TEXCOORD0;
    float4 viewPos : POSITION0;
};

Texture2D objTexture : register(t0);
SamplerState samplerState : register(s0);

struct GBuffer
{
    float4 depth : SV_Target0;
    float4 normal : SV_Target1;
    float4 diffuse : SV_Target2;
    float4 specular : SV_Target3;
};

[earlydepthstencil]
GBuffer main(PS_IN input)
{
    GBuffer buf = (GBuffer) 0;

    // TODO: move this values to const buffer
    // ------------------
    float nearPlane = 0.1;
    float farPlane = 1000;
    // ------------------

    float depth = (input.viewPos.z - nearPlane) / (farPlane - nearPlane);
    float4 textureColor = objTexture.Sample(samplerState, input.tex.xy);

    buf.depth.xyz = float3(depth, depth, depth);
    buf.normal.xyz = input.norm;
    buf.diffuse = float4(1,0,0,1);
    buf.specular.xyz = float3(1, 1, 1);

    return buf;
}