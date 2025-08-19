//
// Created by gorev on 15.08.2025.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <d3d11.h>
#include <wrl/client.h>
#include "Engine/EngineTypes.h"

struct Material
{
    Vector4D diffuseColor;
    Vector4D specularColor;
    Vector4D emissiveColor;
    float shininess;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularTexture;
};

#endif //MATERIAL_H
