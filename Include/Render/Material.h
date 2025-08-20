//
// Created by gorev on 15.08.2025.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <d3d11.h>
#include <DirectXColors.h>
#include <wrl/client.h>

#include "Texture.h"
#include "Engine/EngineTypes.h"

struct Material
{
    Material() = default;
    Material(const Material& other)
    {
        diffuseTexture = other.diffuseTexture;
        specularTexture = other.specularTexture;
        normalTexture = other.normalTexture;
        diffuseColor = other.diffuseColor;
        specularColor = other.specularColor;
        emissiveColor = other.emissiveColor;
        shininess = other.shininess;
    }

    Vector4D diffuseColor = DirectX::Colors::White;
    Vector4D specularColor = DirectX::Colors::White;
    Vector4D emissiveColor = DirectX::Colors::Transparent;
    float shininess = 1.0f;
    Texture diffuseTexture;
    Texture normalTexture;
    Texture specularTexture;
};

#endif //MATERIAL_H
