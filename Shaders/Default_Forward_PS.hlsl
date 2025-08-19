

struct PS_IN
{
    float4 pos : POSITION;
    float4 normal : NORMAL0;
    float4 tex : TEXCOORD0;
};

float3 main( PS_IN input ) : SV_Target
{
    float3 pos = input.pos;
    float3 normal = input.normal;
    float2 tex = input.tex;
    return float3(1,0,0);
}