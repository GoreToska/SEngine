#ifndef LIGHT_TYPE
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define POINT_LIGHT 3
#endif

#include "Common.hlsli"

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
}

cbuffer lightBuffer : register(b1)
{
    float3 lightColor;
    float intensity;
    float3 lightDirection;
    float3 lightPosition;
    int sourceType;
    float lightWidth;
    float constAttenuation;
    float linearAttenuation;
    float exponentAttenuation;
}

cbuffer shadowBuffer : register(b2)
{
	float3 cameraPosition;
}

Texture2D<float4> depthTex : register(t0);
Texture2D<float4> normalTex : register(t1);
Texture2D<float4> albedoTex : register(t2);
Texture2D<float4> metalTex : register(t3);
Texture2D<float4> roughnessTex : register(t4);
Texture2D<float4> nonLinearDepthTex : register(t5);

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 tex : TEXCOORD0;
};

Texture2DArray shadowMap : SHADOWMAP : register(t1);
SamplerComparisonState shadowSampler : SHADOWSAMPLER : register(s1);

float4 main(PS_IN input) : SV_Target
{
        // TODO: get this values from const buffer
    // ---------------------------------
    const float client_width = 1280.0f;
    const float client_height = 720.0f;
    // ---------------------------------


    float3 lightDir = 0;
    float depth = depthTex.Load(int3(input.pos.xy, 0)).x;
    float3 albedo = albedoTex.Load(int3(input.pos.xy, 0)).xyz;
    float metallic = metalTex.Load(int3(input.pos.xy, 0)).x;
	float roughness = roughnessTex.Load(int3(input.pos.xy, 0)).x;
    float3 normal = normalize(normalTex.Load(int3(input.pos.xy, 0)).xyz);
    float nonlinearDepth = nonLinearDepthTex.Load(int3(input.pos.xy, 0)).x;


    float ndcX = input.pos.x / client_width * 2.0f - 1.0f;
    float ndcY = -(input.pos.y / client_height * 2.0f - 1.0f);
    float4 viewSpaceVertPos = mul(float4(ndcX, ndcY, nonlinearDepth, 1.0f), inverseProjection);
    viewSpaceVertPos /= viewSpaceVertPos.w;
    float4 globalSpaceVertPos = mul(viewSpaceVertPos, inverseView);
    globalSpaceVertPos /= globalSpaceVertPos.w;
    float3 globalVertPos = globalSpaceVertPos.xyz;

	float3 V = normalize(cameraPosition - globalVertPos);
	float3 L = 0;

	[branch]
	if(sourceType == DIRECTIONAL_LIGHT)
		L = -normalize(lightDirection);
	else if(sourceType == SPOT_LIGHT || sourceType == POINT_LIGHT)
    {
		L = normalize(lightPosition - globalVertPos);
    }

    //return float4(normal, 1);
    float NdotL = max(dot(normal, L), 0.0);

	float3 H = normalize(V + L);
	float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
	float NDF = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, V, L, roughness);
	float3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	float3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.001;
	float3 specular = numerator / denominator;

	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	float3 diffuse = kD * albedo / PI;

	float3 ambient = float3(0.03, 0.03, 0.03);

	float attenuation = 1.0;
    if (sourceType == POINT_LIGHT || sourceType == SPOT_LIGHT)
    {
        float distance = length(lightPosition - globalVertPos);
        attenuation = 1.0 / (constAttenuation + linearAttenuation * distance + exponentAttenuation * distance * distance);
    }

    float3 radiance = lightColor * intensity * attenuation;

	//diffuse += ambient;

	float3 color = (diffuse + specular) * radiance * NdotL;


	return max(float4(MapToSRGB(color), 1.0), 0);
}