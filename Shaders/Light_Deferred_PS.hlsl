#ifndef LIGHT_TYPE
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define POINT_LIGHT 3
#endif

cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix inverseView;
    matrix inverseProjection;
}

cbuffer lightBuffer : register(b1)
{
    float3 lightColor;
    float intensity;
    float3 lightDirection;
    float3 lightPosition;
    int sourceType;
    float lightWidth;
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
        // TODO: get this values from const buffer
    // ---------------------------------
    const float client_width = 1024.0f;
    const float client_height = 768.0f;
    // ---------------------------------
    

    float3 lightDir = 0;
    float3 spec = 0;
    float shininess = 1.0f;
    float shineDistance = 100.0f;

    float depth = depthTex.Load(int3(input.pos.xy, 0)).x;
    float3 diffuse = diffuseTex.Load(int3(input.pos.xy, 0)).xyz;
    float4 specular = specularTex.Load(int3(input.pos.xy, 0));
    float3 normal = normalTex.Load(int3(input.pos.xy, 0)).xyz;
	//normal.z = normal.z;
	//normal = normalize(normal * 2.0 - 1.0); //

    float nonlinearDepth = nonLinearDepthTex.Load(int3(input.pos.xy, 0)).x;

    float ndcX = input.pos.x / client_width * 2.0f - 1.0f;
    float ndcY = -(input.pos.y / client_height * 2.0f - 1.0f);
    float4 viewSpaceVertPos = mul(float4(ndcX, ndcY, nonlinearDepth, 1.0f), inverseProjection);
    viewSpaceVertPos /= viewSpaceVertPos.w;
    float4 globalSpaceVertPos = mul(viewSpaceVertPos, inverseView);
    globalSpaceVertPos /= globalSpaceVertPos.w;
    float3 globalVertPos = globalSpaceVertPos.xyz;

    [branch]
	if (sourceType == DIRECTIONAL_LIGHT)
	{
    	lightDir = -normalize(lightDirection);
    	//shadow = CalculateShadow(depth, globalVertPos);
	}
	else if (sourceType == POINT_LIGHT || sourceType == SPOT_LIGHT)
	{
    	lightDir = normalize(lightPosition.xyz - globalVertPos);
		//TODO: read shininess from specular map
    	shininess = saturate(1 - length(lightPosition.xyz - globalVertPos) / shineDistance);
	}

	float ndotL = dot(normal, lightDir);
	if(ndotL <= 0.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

    //float3 shineRadius = normalize(2 * dot(normal, lightDir) * normal - lightDir);
    float3 shineRadius = normalize(2 * dot(normal, lightDir) * normal - lightDir);

 	//if (sourceType == POINT_LIGHT || sourceType == SPOT_LIGHT)
    // 	spec = shininess * lightColor * saturate(specular.xyz * intensity * pow(dot(cameraViewAngle, shineRadius), 200));

    float3 ambient = float3(1,1,1) * 1.05f;
    float3 diffuseColor = shininess * lightColor * diffuse * intensity * saturate(dot(lightDir, normal)) * ambient;

    return float4(spec + diffuseColor, 1.0);
}