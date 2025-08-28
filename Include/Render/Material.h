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
        : albedoColor(other.albedoColor),
          specularColor(other.specularColor),
          emissiveColor(other.emissiveColor),
          normalMapEnabled(other.normalMapEnabled),
          metallic(other.metallic),
          roughness(other.roughness),
          albedoTexture(other.albedoTexture),
          normalTexture(other.normalTexture),
          metallicTexture(other.metallicTexture),
          roughnessTexture(other.roughnessTexture),
          aoTexture(other.aoTexture)
    {
    }

    Material(Material&& other) noexcept
        : albedoColor(std::move(other.albedoColor)),
          specularColor(std::move(other.specularColor)),
          emissiveColor(std::move(other.emissiveColor)),
          normalMapEnabled(other.normalMapEnabled),
          metallic(std::move(other.metallic)),
          roughness(std::move(other.roughness)),
          albedoTexture(std::move(other.albedoTexture)),
          normalTexture(std::move(other.normalTexture)),
          metallicTexture(std::move(other.metallicTexture)),
          roughnessTexture(std::move(other.roughnessTexture)),
          aoTexture(std::move(other.aoTexture))
    {
    }

    Material& operator=(const Material& other)
    {
        if (this == &other)
            return *this;

        albedoColor = other.albedoColor;
        specularColor = other.specularColor;
        emissiveColor = other.emissiveColor;
        metallic = other.metallic;
        roughness = other.roughness;
        albedoTexture = other.albedoTexture;
        normalTexture = other.normalTexture;
        metallicTexture = other.metallicTexture;
        normalMapEnabled = other.normalMapEnabled;
        roughnessTexture = other.roughnessTexture;
        aoTexture = other.aoTexture;
        return *this;
    }

    Material& operator=(Material&& other) noexcept
    {
        if (this == &other)
            return *this;

        albedoColor = std::move(other.albedoColor);
        specularColor = std::move(other.specularColor);
        emissiveColor = std::move(other.emissiveColor);
        metallic = std::move(other.metallic);
        roughness = std::move(other.roughness);
        albedoTexture = std::move(other.albedoTexture);
        normalTexture = std::move(other.normalTexture);
        metallicTexture = std::move(other.metallicTexture);
        normalMapEnabled = std::move(other.normalMapEnabled);
        roughnessTexture = std::move(other.roughnessTexture);
        aoTexture = std::move(other.aoTexture);
        return *this;
    }

    Vector4D albedoColor = DirectX::Colors::White;
    Vector4D specularColor = DirectX::Colors::White;
    Vector4D emissiveColor = DirectX::Colors::Transparent;
    BOOL normalMapEnabled = FALSE;
    float metallic = 0.0f;
    float roughness = 0.0f;

    Texture albedoTexture;
    Texture normalTexture;
    Texture metallicTexture;
    Texture roughnessTexture;
    Texture aoTexture;
};

#endif //MATERIAL_H
