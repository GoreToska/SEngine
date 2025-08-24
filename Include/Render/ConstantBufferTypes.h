//
// Created by gorev on 14.08.2025.
//

#ifndef CONSTANTBUFFERTYPES_H
#define CONSTANTBUFFERTYPES_H
#include "Engine/EngineTypes.h"

struct VS_ObjectBuffer
{
    Matrix world;
    Matrix view;
    Matrix projection;
    Matrix inverseView;
    Matrix inverseProjection;
};

struct PS_LightBuffer
{
    Vector3D lightColor;
    float intensity;
    Vector4D lightDirection;
    Vector3D lightPosition;
    int sourceType;
    float coneWidth;
};

struct PS_MaterialBuffer
{
    Vector4D diffuseColor;
    Vector4D specularColor;
    Vector4D emissiveColor;
    BOOL normalMapEnabled;
    float shininess;
};

struct PS_CascadeShadowsBuffer
{
    Matrix ViewProjectionMatrix[4];
    Vector4D Distances;
    Vector4D CameraPosition;
};

struct CS_Sort
{
    UINT iLevel;
    UINT iLevelMask;
    UINT iWidth;
    UINT iHeight;
};
#endif //CONSTANTBUFFERTYPES_H
