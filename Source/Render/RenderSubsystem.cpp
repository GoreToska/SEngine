//
// Created by gorev on 30.06.2025.
//

#include "Render/RenderSubsystem.h"

#include "Component/CameraComponent.h"
#include "Render/GraphicsDevice.h"
#include "Utilities/AdapterReader.h"
#include "Utilities/Asserter.h"

void RenderSubsystem::Initialize(HWND hwnd, int width, int height)
{
    clientWidth = width;
    clientHeight = height;
    InitializeDirectX(hwnd);
    ThrowIfFailed(objectMatrixBuffer.Initialize(), "Failed to initialize buffer.");
    ThrowIfFailed(lightBuffer.Initialize(), "Failed to initialize light buffer.");
}

// TODO: render shadow map should take light component, not camera component
void RenderSubsystem::RenderShadowMap(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
                                      std::weak_ptr<CameraComponent> cameraComponent)
{
    ClearState();
    //TODO: render each object with shadow map vs and gs
    //SERROR("Not implemented");
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> RenderSubsystem::GetDepthStencilView()
{
    return depthStencilView;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RenderSubsystem::GetDepthStencilSRV()
{
    return depthStencilSRV;
}

void RenderSubsystem::RenderObjects(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
                                    const std::weak_ptr<CameraComponent>& cameraComponent)
{
    auto camera = cameraComponent.lock();
    if (!camera)
    {
        SERROR("No active camera.");
        return;
    }

    objectMatrixBuffer.GetData()->view = camera->GetViewMatrix().Transpose();
    objectMatrixBuffer.GetData()->projection = camera->GetProjectionMatrix().Transpose();
    objectMatrixBuffer.GetData()->inverseView = camera->GetViewMatrix().Invert().Transpose();
    objectMatrixBuffer.GetData()->inverseProjection = camera->GetProjectionMatrix().Invert().Transpose();

    for (auto it = objectsToRender.begin(); it != objectsToRender.end();)
    {
        SDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());

        if (auto component = it->lock())
        {
            // TODO: update const buffers
            if (auto transform = component->GetTransform().lock())
            {
                objectMatrixBuffer.GetData()->world = transform->GetWorldMatrix().Transpose();
                objectMatrixBuffer.ApplyChanges();
                component->SetShaders();
                component->Render();
            }

            ++it;
        }
        else
        {
            it = objectsToRender.erase(it);
        }
    }
}

void RenderSubsystem::InitializeDirectX(HWND hwnd)
{
    CreateDeviceAndSwapChain(hwnd);
    CreateRenderTargetView();
    CreateDepthStencilBuffer();
    CreateDepthStencilState();
    CreateViewport();
    CreateRasterizerAndBlendState();
    CreateSamplerState();
}

void RenderSubsystem::CreateDeviceAndSwapChain(HWND hwnd)
{
    const auto& adapters = AdapterReader::GetAdapters();

    if (adapters.empty())
    {
        SERROR("No DXGI Adapters found.");
        exit(-1);
    }

    // TODO: decide which device is better to use
    // just using first for now

    DXGI_SWAP_CHAIN_DESC swapchain_desc{};
    swapchain_desc.BufferDesc.Width = clientWidth;
    swapchain_desc.BufferDesc.Height = clientHeight;
    swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.SampleDesc.Quality = 0;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.BufferCount = 2;
    swapchain_desc.OutputWindow = hwnd;
    swapchain_desc.Windowed = TRUE;
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1};

    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
                      &adapters[0].GetAdapter(),
                      D3D_DRIVER_TYPE_UNKNOWN, // if we do not specify adapter we can use D3D_DRIVER_TYPE_HARDWARE
                      NULL,
                      D3D11_CREATE_DEVICE_DEBUG,
                      featureLevels,
                      ARRAYSIZE(featureLevels),
                      D3D11_SDK_VERSION,
                      &swapchain_desc,
                      swapchain.GetAddressOf(),
                      SDeviceAddress,
                      NULL, // supported feature level
                      SDeviceContextAddress), "failed to create swap chain");
}

void RenderSubsystem::CreateRenderTargetView()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    ThrowIfFailed(
        swapchain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(backBuffer.GetAddressOf())),
        "Failed to get back buffer.");

    ThrowIfFailed(
        SDevice->CreateRenderTargetView(
            backBuffer.Get(),
            NULL,
            renderTargetView.GetAddressOf()),
        "Failed to create render target.");
}

void RenderSubsystem::CreateRasterizerAndBlendState()
{
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    ThrowIfFailed(
        SDevice->CreateRasterizerState(
            &rasterizerDesc,
            rasterizerState.GetAddressOf()),
        "Failed to create rasterizer state.");

    rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    ThrowIfFailed(
        SDevice->CreateRasterizerState(
            &rasterizerDesc,
            rastStateCullBack.GetAddressOf()),
        "Failed to create rasterizer state.");

    rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_FRONT;
    ThrowIfFailed(
        SDevice->CreateRasterizerState(
            &rasterizerDesc,
            rastStateCullFront.GetAddressOf()),
        "Failed to create rasterizer state.");

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ThrowIfFailed(SDevice->CreateBlendState(&blendDesc, &additiveBlendState), "Failed to create blend state.");
}

void RenderSubsystem::CreateSamplerState()
{
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ThrowIfFailed(
        SDevice->CreateSamplerState(
            &samplerDesc,
            samplerState.GetAddressOf()),
        "Failed to create sampler state.");


    D3D11_SAMPLER_DESC shadowSamplerDesc = {};
    shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
    shadowSamplerDesc.MinLOD = 0;
    shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    shadowSamplerDesc.MipLODBias = 0;
    shadowSamplerDesc.BorderColor[0] = 1.0f;
    shadowSamplerDesc.BorderColor[1] = 1.0f;
    shadowSamplerDesc.BorderColor[2] = 1.0f;
    shadowSamplerDesc.BorderColor[3] = 1.0f;

    ThrowIfFailed(
        SDevice->CreateSamplerState(
            &shadowSamplerDesc,
            shadowSamplerState.GetAddressOf()),
        "Failed to create sampler state.");
}

void RenderSubsystem::CreateDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    ThrowIfFailed(
        SDevice->CreateDepthStencilState(
            &depthStencilDesc,
            depthStencilState.GetAddressOf()),
        "Failed to create depth stencil state.");


    depthStencilDesc = {};
    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.StencilEnable = false;
    ThrowIfFailed(
        SDevice->CreateDepthStencilState(
            &depthStencilDesc,
            NoWriteNoCheckDSS.GetAddressOf()),
        "Failed to create depth stencil state.");

    depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = false;
    ThrowIfFailed(
        SDevice->CreateDepthStencilState(
            &depthStencilDesc,
            NoWriteLessDSS.GetAddressOf()),
        "Failed to create depth stencil state.");

    depthStencilDesc = {};
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
    depthStencilDesc.StencilEnable = false;
    ThrowIfFailed(
        SDevice->CreateDepthStencilState(
            &depthStencilDesc,
            NoWriteGreaterDSS.GetAddressOf()),
        "Failed to create depth stencil state.");
}

void RenderSubsystem::CreateDepthStencilBuffer()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = clientWidth;
    depthStencilDesc.Height = clientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    depthStencilDesc.SampleDesc = {1, 0};

    ThrowIfFailed(
        SDevice->CreateTexture2D(
            &depthStencilDesc,
            NULL,
            depthStencilTexture.GetAddressOf()),
        "Failed to create depth stencil buffer.");


    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    ThrowIfFailed(
        SDevice->CreateDepthStencilView(
            depthStencilTexture.Get(),
            &dsvDesc,
            depthStencilView.GetAddressOf()),
        "Failed to create depth stencil view.");

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    ThrowIfFailed(
        SDevice->CreateShaderResourceView(
            depthStencilTexture.Get(),
            &srvDesc,
            depthStencilSRV.GetAddressOf()),
        "Failed to create shader resource view.");
}

void RenderSubsystem::CreateViewport()
{
    viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = clientWidth;
    viewport.Height = clientHeight;
    viewport.MinDepth = 0.0;
    viewport.MaxDepth = 1.0;
}

void RenderSubsystem::ClearState()
{
    SDeviceContext->ClearState();
    SDeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
}

void RenderSubsystem::ClearDepthAndTargetViews()
{
    float color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    SDeviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
    SDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
