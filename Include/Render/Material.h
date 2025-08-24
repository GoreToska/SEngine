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
        : diffuseColor(other.diffuseColor),
          specularColor(other.specularColor),
          emissiveColor(other.emissiveColor),
          normalMapEnabled(other.normalMapEnabled),
          shininess(other.shininess),
          diffuseTexture(other.diffuseTexture),
          normalTexture(other.normalTexture),
          specularTexture(other.specularTexture)
    {
    }

    Material(Material&& other) noexcept
        : diffuseColor(std::move(other.diffuseColor)),
          specularColor(std::move(other.specularColor)),
          emissiveColor(std::move(other.emissiveColor)),
          normalMapEnabled(other.normalMapEnabled),
          shininess(std::move(other.shininess)),
          diffuseTexture(std::move(other.diffuseTexture)),
          normalTexture(std::move(other.normalTexture)),
          specularTexture(std::move(other.specularTexture))
    {
    }

    Material& operator=(const Material& other)
    {
        if (this == &other)
            return *this;

        diffuseColor = other.diffuseColor;
        specularColor = other.specularColor;
        emissiveColor = other.emissiveColor;
        shininess = other.shininess;
        diffuseTexture = other.diffuseTexture;
        normalTexture = other.normalTexture;
        specularTexture = other.specularTexture;
        normalMapEnabled = other.normalMapEnabled;
        return *this;
    }

    Material& operator=(Material&& other) noexcept
    {
        if (this == &other)
            return *this;

        diffuseColor = std::move(other.diffuseColor);
        specularColor = std::move(other.specularColor);
        emissiveColor = std::move(other.emissiveColor);
        shininess = std::move(other.shininess);
        diffuseTexture = std::move(other.diffuseTexture);
        normalTexture = std::move(other.normalTexture);
        specularTexture = std::move(other.specularTexture);
        normalMapEnabled = std::move(other.normalMapEnabled);
        return *this;
    }

    Vector4D diffuseColor = DirectX::Colors::White;
    Vector4D specularColor = DirectX::Colors::White;
    Vector4D emissiveColor = DirectX::Colors::Transparent;
    BOOL normalMapEnabled = FALSE;
    float shininess = 1.0f;

    Texture diffuseTexture;
    Texture normalTexture;
    Texture specularTexture;
};

#endif //MATERIAL_H
