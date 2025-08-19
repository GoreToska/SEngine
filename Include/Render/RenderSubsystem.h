//
// Created by gorev on 30.06.2025.
//

#ifndef RENDERSUBSYSTEM_H
#define RENDERSUBSYSTEM_H

#include <Windows.h>
#include <wrl/client.h>
#include<d3d11.h>
#include <memory>

#include "ConstantBufferTypes.h"
#include "ConstBuffer.h"
#include "SimpleMath.h"
#include "Component/IRenderComponent.h"

class CameraComponent;

class RenderSubsystem
{
public:
    virtual void Initialize(HWND hwnd, int width, int height);

    virtual void Render(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
    std::weak_ptr<CameraComponent> cameraComponent) = 0;

    void RenderShadowMap(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
                         std::weak_ptr<CameraComponent> cameraComponent);

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView();

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetDepthStencilSRV();

protected:
    void RenderObjects(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
                       const std::weak_ptr<CameraComponent>& cameraComponent);

    void InitializeDirectX(HWND hwnd);

    void CreateDeviceAndSwapChain(HWND hwnd);

    void CreateRenderTargetView();

    void CreateRasterizerAndBlendState();

    void CreateSamplerState();

    void CreateDepthStencilState();

    void CreateDepthStencilBuffer();

    void CreateViewport();

    void ClearState();

    void ClearDepthAndTargetViews();

    int clientWidth;
    int clientHeight;

    D3D11_VIEWPORT viewport;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthStencilSRV;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> shadowSamplerState;

    ConstBuffer<VS_ObjectBuffer> objectMatrixBuffer;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> NoWriteNoCheckDSS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> NoWriteGreaterDSS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> NoWriteLessDSS;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastStateCullFront;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastStateCullBack;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

    Microsoft::WRL::ComPtr<ID3D11BlendState> additiveBlendState;
};


#endif //RENDERSUBSYSTEM_H
