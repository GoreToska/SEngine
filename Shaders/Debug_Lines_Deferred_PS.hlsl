struct PSInput
{
    float4 posH : SV_POSITION;
};

struct GBuffer
{
    float4 depth : SV_Target0;
    float4 normal : SV_Target1;
    float4 diffuse : SV_Target2;
    float4 metal : SV_Target3;
    float4 roughness : SV_Target4;
};

Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

GBuffer main(PSInput input) : SV_TARGET
{
    GBuffer buf = (GBuffer) 0;
    float4 textureColor = diffuseTexture.Sample(samplerState, float2(0,0));
    buf.diffuse = textureColor;
    return buf;
}