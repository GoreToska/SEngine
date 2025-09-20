//
// Created by gorev on 29.08.2025.
//

#include "Component/SkyBoxComponent.h"

#include "WICTextureLoader.h"
#include "Engine/Engine.h"
#include "Render/GraphicsDevice.h"
#include "Utilities/Asserter.h"
#include "Utilities/Logger.h"
#include "Utilities/PrimitiveVolumes.h"

SkyBoxComponent::SkyBoxComponent(const std::filesystem::path& path,
                                 const std::vector<std::wstring>& textures,
                                 const std::string& vs, const std::string& ps)
    : Super(), path(path), textureNames(textures), ps(ps), vs(vs)
{
    InitializeSkybox();

    ThrowIfFailed(objectMatrixBuffer.Initialize(), "Failed to initialize object matrix buffer");
}

void SkyBoxComponent::Render(std::weak_ptr<CameraComponent> camera)
{
    SDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    auto cameraComponent = camera.lock();
    if (!cameraComponent)
        return;

    objectMatrixBuffer.GetData()->view = cameraComponent->GetViewMatrix().Transpose();
    objectMatrixBuffer.GetData()->projection = cameraComponent->GetProjectionMatrix().Transpose();
    objectMatrixBuffer.ApplyChanges();

    SDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
    SDeviceContext->PSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());

    ID3D11ShaderResourceView* nullResource = nullptr;
    for (size_t i = 0; i < 7; i++)
    {
        SDeviceContext->PSSetShaderResources(i, 1, &nullResource);
    }

    SDeviceContext->PSSetShaderResources(0, 1, textureSRV.GetAddressOf());
    UINT stride = sizeof(vertices[0]);
    UINT offset = 0;
    SDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    SDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    SDeviceContext->DrawIndexed(indices.size(), 0, 0);
}

void SkyBoxComponent::SetShaders() const
{
    SDeviceContext->IASetInputLayout(
        SEngine.GetGraphics().GetShaderManager()
        .GetShader<VertexShader>(vs)->GetInputLayout().Get());
    SDeviceContext->VSSetShader(
        SEngine.GetGraphics().GetShaderManager()
        .GetShader<VertexShader>(vs)->GetShader().Get(), NULL, 0);
    SDeviceContext->PSSetShader(
        SEngine.GetGraphics().GetShaderManager()
        .GetShader<PixelShader>(ps)->GetShader().Get(), NULL, 0);
}

void SkyBoxComponent::InitializeSkybox()
{
    if (textureNames.size() != 6)
    {
        SERROR("Skybox require 6 textures");
        return;
    }

    // this is needed for width, height and format
    auto firstFace = LoadSkyboxFace(path / textureNames[0]);

    D3D11_TEXTURE2D_DESC firstFaceDesc;
    firstFace->GetDesc(&firstFaceDesc);

    D3D11_TEXTURE2D_DESC cubeDesc = {};
    cubeDesc.Width = firstFaceDesc.Width;
    cubeDesc.Height = firstFaceDesc.Height;
    cubeDesc.MipLevels = firstFaceDesc.MipLevels;
    cubeDesc.ArraySize = 6;
    cubeDesc.Format = firstFaceDesc.Format;
    cubeDesc.SampleDesc.Count = 1;
    cubeDesc.SampleDesc.Quality = 0;
    cubeDesc.Usage = D3D11_USAGE_DEFAULT;
    cubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    cubeDesc.CPUAccessFlags = 0;

    ThrowIfFailed(
        SDevice->CreateTexture2D(
            &cubeDesc, nullptr, texture.GetAddressOf()),
        "Failed to create texture2D");

    for (UINT face = 0; face < 6; ++face)
    {
        auto faceTexture = LoadSkyboxFace(path / textureNames[face]);

        D3D11_TEXTURE2D_DESC faceDesc;
        faceTexture->GetDesc(&faceDesc);

        if (faceDesc.Width != cubeDesc.Width ||
            faceDesc.Height != cubeDesc.Height ||
            faceDesc.Format != cubeDesc.Format)
        {
            SERROR("Skybox faces must have same dimensions and format");
        }

        for (UINT mip = 0; mip < cubeDesc.MipLevels; ++mip)
        {
            SDeviceContext->CopySubresourceRegion(
                texture.Get(),
                D3D11CalcSubresource(mip, face, cubeDesc.MipLevels),
                0, 0, 0,
                faceTexture.Get(),
                mip,
                nullptr
            );
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = cubeDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = cubeDesc.MipLevels;
    srvDesc.TextureCube.MostDetailedMip = 0;

    ThrowIfFailed(SDevice->CreateShaderResourceView(texture.Get(), &srvDesc, textureSRV.GetAddressOf()),
                  "Failed to create texture SRV");

    volume::GetBoxVertices(vertices);
    volume::GetBoxIndices(indices);

    ThrowIfFailed(vertexBuffer.Initialize(vertices.data(), vertices.size()), "Failed to initialize vertex buffer");
    ThrowIfFailed(indexBuffer.Initialize(indices.data(), indices.size()), "Failed to initialize index buffer");
}


Microsoft::WRL::ComPtr<ID3D11Texture2D> SkyBoxComponent::LoadSkyboxFace(const std::filesystem::path& path)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

    DirectX::CreateWICTextureFromFileEx(
        SDevice,
        path.c_str(),
        0,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE,
        0,
        0,
        DirectX::WIC_LOADER_FORCE_SRGB,
        reinterpret_cast<ID3D11Resource**>(texture.GetAddressOf()),
        srv.GetAddressOf()
    );

    return texture;
}
