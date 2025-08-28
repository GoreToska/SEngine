//
// Created by gorev on 20.08.2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <DirectXColors.h>
#include <wrl/client.h>

#include "DDSTextureLoader.h"
#include "GraphicsDevice.h"
#include "ShaderManager.h"
#include "WICTextureLoader.h"
#include "../../cmake-build-debug/_deps/directxtk-src/Src/PlatformHelpers.h"
#include "Engine/EngineTypes.h"
#include "assimp/material.h"
#include "Utilities/Asserter.h"
#include "Utilities/StringUtilities.h"

class Texture
{
public:
    enum class TextureType
    {
        None = 0,
        Albedo = 1,
        Normal = 2,
        Metallic = 3,
        Roughness = 4,
        Occlusion = 5,
    };

    Texture()
    {
        this->type = TextureType::None;
        InitializeTextureWithColor({255, 255, 255, 255});
    }

    Texture(const Texture& other)
        : texture(other.texture),
          textureSRV(other.textureSRV),
          type(other.type),
          color(other.color),
          path(other.path)
    {
    }

    Texture(Texture&& other) noexcept
        : texture(std::move(other.texture)),
          textureSRV(std::move(other.textureSRV)),
          type(std::move(other.type)),
          color(std::move(other.color)),
          path(std::move(other.path))
    {
        other.textureSRV = nullptr;
        other.texture = nullptr;
    }

    Texture& operator=(const Texture& other)
    {
        if (this == &other)
            return *this;

        texture = other.texture;
        textureSRV = other.textureSRV;
        type = other.type;
        color = other.color;
        path = other.path;
        return *this;
    }

    Texture& operator=(Texture&& other) noexcept
    {
        if (this == &other)
            return *this;

        texture = std::move(other.texture);
        textureSRV = std::move(other.textureSRV);
        type = std::move(other.type);
        color = std::move(other.color);
        path = std::move(other.path);
        other.textureSRV = nullptr;
        other.texture = nullptr;
        return *this;
    }

    Texture(const Color& color, const TextureType type)
    {
        this->type = type;
        InitializeTextureWithColor(color);
    }

    Texture(const std::filesystem::path& filePath, const TextureType type)
    {
        path = filePath;
        this->type = type;
        HRESULT hr;
        if (filePath.extension() == ".dds")
        {
            hr = DirectX::CreateDDSTextureFromFileEx(
                SDevice,
                ToWideString(filePath.string()).c_str(),
                0,
                D3D11_USAGE_DEFAULT,
                D3D11_BIND_SHADER_RESOURCE,
                D3D11_CPU_ACCESS_READ,
                0,
                DirectX::DDS_LOADER_FORCE_SRGB,
                reinterpret_cast<ID3D11Resource**>(texture.GetAddressOf()),
                textureSRV.GetAddressOf()
            );
        }
        else
        {
            hr = DirectX::CreateWICTextureFromFileEx(
                SDevice,
                ToWideString(filePath.string()).c_str(),
                0,
                D3D11_USAGE_DEFAULT,
                D3D11_BIND_SHADER_RESOURCE,
                D3D11_CPU_ACCESS_READ,
                0,
                DirectX::WIC_LOADER_FORCE_SRGB,
                reinterpret_cast<ID3D11Resource**>(texture.GetAddressOf()),
                textureSRV.GetAddressOf());
        }

        if (FAILED(hr))
            InitializeTextureWithColor(ERROR_COLOR);
    }

    ~Texture()
    {
    }

    void InitializeTextureWithColor(const Color& color)
    {
        this->color = color;
        texture.Reset();
        textureSRV.Reset();

        CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1);
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        D3D11_SUBRESOURCE_DATA initialData{};
        initialData.pSysMem = color;
        initialData.SysMemPitch = sizeof(Color);

        ThrowIfFailed(
            SDevice->CreateTexture2D(
                &textureDesc,
                &initialData,
                &texture),
            "Failed to initialize texture from color data."
        );

        CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
        ThrowIfFailed(
            SDevice->CreateShaderResourceView(texture.Get(), &srvDesc, textureSRV.GetAddressOf()),
            "Failed to initialize texture resource."
        );
    }

    TextureType GetType() const
    {
        return type;
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureSRV() const
    {
        return textureSRV;
    }

    std::filesystem::path GetFilePath() const
    {
        return path;
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;
    TextureType type;
    Color color;
    std::filesystem::path path = "";
};

#endif //TEXTURE_H
