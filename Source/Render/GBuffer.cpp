//
// Created by gorev on 17.08.2025.
//

#include "Render/GBuffer.h"

#include "Engine/Engine.h"
#include "Render/GraphicsDevice.h"
#include "Utilities/Asserter.h"

GBuffer::GBuffer()
{
    D3D11_TEXTURE2D_DESC depthTexDesc = {};
    depthTexDesc.Width = SEngine.GetGraphics().GetClientWidth();
    depthTexDesc.Height = SEngine.GetGraphics().GetClientHeight();
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    depthTexDesc.CPUAccessFlags = 0;
    depthTexDesc.MiscFlags = 0;
    depthTexDesc.SampleDesc = {1, 0};

    ThrowIfFailed(SDevice->CreateTexture2D(&depthTexDesc, nullptr, &depth), "Failed to create depth.");

    D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc = {};
    depthSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    depthSrvDesc.Texture2D.MipLevels = 1;
    depthSrvDesc.Texture2D.MostDetailedMip = 0;

    D3D11_RENDER_TARGET_VIEW_DESC depthRtvDesc = {};
    depthRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    depthRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    depthRtvDesc.Texture2D.MipSlice = 0;

    ThrowIfFailed(SDevice->CreateShaderResourceView(depth.Get(), &depthSrvDesc, &depthSRV),
                  "Failed to create depth srv.");
    ThrowIfFailed(SDevice->CreateRenderTargetView(depth.Get(), &depthRtvDesc, &depthRTV), "Failed.");


    D3D11_TEXTURE2D_DESC normalTexDesc = {};
    normalTexDesc.Width = SEngine.GetGraphics().GetClientWidth();
    normalTexDesc.Height = SEngine.GetGraphics().GetClientHeight();
    normalTexDesc.MipLevels = 1;
    normalTexDesc.ArraySize = 1;
    normalTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    normalTexDesc.Usage = D3D11_USAGE_DEFAULT;
    normalTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    normalTexDesc.CPUAccessFlags = 0;
    normalTexDesc.MiscFlags = 0;
    normalTexDesc.SampleDesc = {1, 0};

    ThrowIfFailed(SDevice->CreateTexture2D(&normalTexDesc, nullptr, &normal), "Failed.");


    D3D11_SHADER_RESOURCE_VIEW_DESC normalSrvDesc = {};
    normalSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    normalSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    normalSrvDesc.Texture2D.MipLevels = 1;
    normalSrvDesc.Texture2D.MostDetailedMip = 0;
    D3D11_RENDER_TARGET_VIEW_DESC normalRtvDesc = {};
    normalRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    normalRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    normalRtvDesc.Texture2D.MipSlice = 0;

    ThrowIfFailed(SDevice->CreateShaderResourceView(normal.Get(), &normalSrvDesc, &normalSRV), "Failed");
    ThrowIfFailed(SDevice->CreateRenderTargetView(normal.Get(), &normalRtvDesc, &normalRTV), "Failed");


    D3D11_TEXTURE2D_DESC diffuseTexDesc = {};
    diffuseTexDesc.Width = SEngine.GetGraphics().GetClientWidth();
    diffuseTexDesc.Height = SEngine.GetGraphics().GetClientHeight();
    diffuseTexDesc.MipLevels = 1;
    diffuseTexDesc.ArraySize = 1;
    diffuseTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    diffuseTexDesc.Usage = D3D11_USAGE_DEFAULT;
    diffuseTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    diffuseTexDesc.CPUAccessFlags = 0;
    diffuseTexDesc.MiscFlags = 0;
    diffuseTexDesc.SampleDesc = {1, 0};

    ThrowIfFailed(SDevice->CreateTexture2D(&diffuseTexDesc, nullptr, &diffuse), "Failed.");

    D3D11_SHADER_RESOURCE_VIEW_DESC diffuseSrvDesc = {};
    diffuseSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    diffuseSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    diffuseSrvDesc.Texture2D.MipLevels = 1;
    diffuseSrvDesc.Texture2D.MostDetailedMip = 0;
    D3D11_RENDER_TARGET_VIEW_DESC diffuseRtvDesc = {};
    diffuseRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    diffuseRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    diffuseRtvDesc.Texture2D.MipSlice = 0;

    ThrowIfFailed(SDevice->CreateShaderResourceView(diffuse.Get(), &diffuseSrvDesc, &diffuseSRV), "Failed.");
    ThrowIfFailed(SDevice->CreateRenderTargetView(diffuse.Get(), &diffuseRtvDesc, &diffuseRTV), "Failed");

    D3D11_TEXTURE2D_DESC specTexDesc = {};
    specTexDesc.Width = SEngine.GetGraphics().GetClientWidth();
    specTexDesc.Height = SEngine.GetGraphics().GetClientHeight();
    specTexDesc.MipLevels = 1;
    specTexDesc.ArraySize = 1;
    specTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    specTexDesc.Usage = D3D11_USAGE_DEFAULT;
    specTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    specTexDesc.CPUAccessFlags = 0;
    specTexDesc.MiscFlags = 0;
    specTexDesc.SampleDesc = {1, 0};

    ThrowIfFailed(SDevice->CreateTexture2D(&specTexDesc, nullptr, &specular), "Failed.");

    D3D11_SHADER_RESOURCE_VIEW_DESC specSrvDesc = {};
    specSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    specSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    specSrvDesc.Texture2D.MipLevels = 1;
    specSrvDesc.Texture2D.MostDetailedMip = 0;
    D3D11_RENDER_TARGET_VIEW_DESC specRtvDesc = {};
    specRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    specRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    specRtvDesc.Texture2D.MipSlice = 0;

    ThrowIfFailed(SDevice->CreateShaderResourceView(specular.Get(), &specSrvDesc, &specularSRV), "Failed.");
    ThrowIfFailed(SDevice->CreateRenderTargetView(specular.Get(), &specRtvDesc, &specularRTV), "Failed.");
}

void GBuffer::SetRenderTargets() const
{
    ID3D11RenderTargetView* rtvs[] = {
        depthRTV.Get(),
        normalRTV.Get(),
        diffuseRTV.Get(),
        specularRTV.Get()
    };

    SDeviceContext->OMSetRenderTargets(4, rtvs,
                                       SEngine.GetGraphics().GetRenderSubsystem().GetDepthStencilView().Get());
}

void GBuffer::PSBindResourceViews(int startIndex) const
{
    ID3D11ShaderResourceView* srvs[] = {
        depthSRV.Get(),
        normalSRV.Get(),
        diffuseSRV.Get(),
        specularSRV.Get(),
        SEngine.GetGraphics().GetRenderSubsystem().GetDepthStencilSRV().Get(),
    };

    SDeviceContext->PSSetShaderResources(startIndex, ARRAYSIZE(srvs), srvs);
}

void GBuffer::PSClearResourceViews(int startIndex) const
{
    ID3D11ShaderResourceView* rsvs[] = {nullptr, nullptr, nullptr, nullptr, nullptr};

    SDeviceContext->PSSetShaderResources(startIndex, ARRAYSIZE(rsvs), rsvs);
}

void GBuffer::ClearRenderTargets() const
{
    constexpr float clearColorWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr float clearColorBlack[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    SDeviceContext->ClearRenderTargetView(depthRTV.Get(), clearColorWhite);
    SDeviceContext->ClearRenderTargetView(normalRTV.Get(), clearColorBlack);
    SDeviceContext->ClearRenderTargetView(diffuseRTV.Get(), clearColorBlack);
    SDeviceContext->ClearRenderTargetView(specularRTV.Get(), clearColorBlack);
}
