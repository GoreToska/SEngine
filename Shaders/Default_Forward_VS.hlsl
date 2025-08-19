
struct VS_IN
{
    float4 pos : POSITION;
    float4 normal : NORMAL0;
    float4 tex : TEXCOORD0;
};

struct PS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL0;
    float2 tex : TEXCOORD0;
};

PS_IN main( VS_IN input )
{
    PS_IN output = (PS_IN)0;

    output.pos = input.pos;
    output.normal = input.normal;
    output.tex = input.tex;

    return output;
}
