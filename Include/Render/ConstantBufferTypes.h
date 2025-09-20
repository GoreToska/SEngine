//
// Created by gorev on 14.08.2025.
//

#ifndef CONSTANTBUFFERTYPES_H
#define CONSTANTBUFFERTYPES_H
#include "Engine/EngineTypes.h"

struct PerObjectBuffer
{
    Matrix world;
    Matrix view;
    Matrix projection;
    Matrix worldView;
    Matrix viewProjection;
    Matrix worldViewProjection;
    Matrix inverseWorld;
    Matrix inverseView;
    Matrix inverseProjection;
};

struct LightBuffer
{
    Vector3D lightColor;
    float intensity;
    Vector4D lightDirection;
    Vector3D lightPosition;
    int sourceType;
    float coneWidth;
    float constAttenuation;
    float linearAttenuation;
    float exponentAttenuation;
};

struct MaterialBuffer
{
    Vector4D diffuseColor;
    Vector4D specularColor;
    Vector4D emissiveColor;
    BOOL normalMapEnabled;
    float metallic;
};

struct CascadeShadowsBuffer
{
    Matrix ViewProjectionMatrix[4];
    Vector4D Distances;
    Vector4D CameraPosition;
};

struct ShadowBuffer
{
    Vector3D CameraPosition;
};

struct SortBuffer
{
    UINT iLevel;
    UINT iLevelMask;
    UINT iWidth;
    UINT iHeight;
};
#endif //CONSTANTBUFFERTYPES_H
