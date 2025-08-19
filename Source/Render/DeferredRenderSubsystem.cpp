//
// Created by gorev on 13.08.2025.
//

#include "Render/DeferredRenderSubsystem.h"

#include "Engine/Engine.h"
#include "Render/ModelBuffer.h"
#include "Render/ShaderManager.h"

void DeferredRenderSubsystem::Initialize(HWND hwnd, int width, int height)
{
    Super::Initialize(hwnd, width, height);
    gBuffer = std::make_unique<GBuffer>();
}

void DeferredRenderSubsystem::Render(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
                                     const std::weak_ptr<CameraComponent> cameraComponent)
{
    gBuffer->ClearRenderTargets();
    SDeviceContext->RSSetViewports(1, &viewport);
    SDeviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
    SDeviceContext->RSSetState(rasterizerState.Get());
    SDeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    ClearDepthAndTargetViews();
    gBuffer->SetRenderTargets();

    RenderObjects(objectsToRender, cameraComponent);

    // todo: render light
    DrawLight();

    // todo: render particles

    swapchain->Present(1, NULL);
}

void DeferredRenderSubsystem::DrawLight()
{
    SDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
    SDeviceContext->RSSetState(rasterizerState.Get());
    SDeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    SDeviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    SDeviceContext->PSSetSamplers(1, 1, shadowSamplerState.GetAddressOf());
    gBuffer->PSBindResourceViews(2);

    DrawFullScreenQuad();
}

void DeferredRenderSubsystem::DrawFullScreenQuad()
{
    SDeviceContext->IASetInputLayout(
        SEngine.GetGraphics().GetShaderManager().GetShader<VertexShader>("Light_Deferred_VS")->GetInputLayout().Get());
    SDeviceContext->VSSetShader(
        SEngine.GetGraphics().GetShaderManager().GetShader<VertexShader>("Light_Deferred_VS")->GetShader().Get(),
        nullptr, 0);
    SDeviceContext->PSSetShader(
        SEngine.GetGraphics().GetShaderManager().GetShader<PixelShader>("Light_Deferred_PS")->GetShader().Get(),
        nullptr, 0);
    SDeviceContext->OMSetBlendState(additiveBlendState.Get(), nullptr, 0xFFFFFFFF);
    SDeviceContext->RSSetViewports(1, &viewport);

    std::vector<Vector4D> verts = {Vector4D()};
    std::vector<DWORD> idcs = {0};
    std::vector<UINT> mockOffsets = {0};
    VertexBuffer<Vector4D> vertexBuffer = {};
    IndexBuffer indexBuffer = {};
    vertexBuffer.Initialize(verts.data(), verts.size());
    indexBuffer.Initialize(idcs.data(), idcs.size());
    std::vector<UINT> mockStrides = {0};

    SDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), mockStrides.data(), mockOffsets.data());
    SDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    SDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    SDeviceContext->Draw(4, 0);
}
