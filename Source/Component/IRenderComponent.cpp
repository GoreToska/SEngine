//
// Created by gorev on 14.08.2025.
//

#include "Component/IRenderComponent.h"

#include "Engine/Engine.h"
#include "Render/GraphicsDevice.h"
#include "Utilities/Asserter.h"


IRenderComponent::IRenderComponent(const std::weak_ptr<Transform>& transform, const std::string& vs,
                                   const std::string& ps,
                                   const std::string& gs, D3D_PRIMITIVE_TOPOLOGY topology) : transform(transform),
    v_shader(vs),
    p_shader(ps), g_shader(gs),
    topology(topology)
{
}

IRenderComponent::~IRenderComponent()
{
}

void IRenderComponent::SetShaders()
{
    SDeviceContext->IASetInputLayout(
        SEngine.GetGraphics().GetShaderManager()
        .GetShader<VertexShader>(v_shader)->GetInputLayout().Get());
    SDeviceContext->VSSetShader(
        SEngine.GetGraphics().GetShaderManager()
        .GetShader<VertexShader>(v_shader)->GetShader().Get(), NULL, 0);
    SDeviceContext->PSSetShader(
        SEngine.GetGraphics().GetShaderManager()
        .GetShader<PixelShader>(p_shader)->GetShader().Get(), NULL, 0);
}

void IRenderComponent::Update(const float& deltaTime)
{
    Super::Update(deltaTime);
}

void IRenderComponent::Render()
{
    SDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

const std::weak_ptr<Transform>& IRenderComponent::GetTransform() const
{
    return transform;
}
