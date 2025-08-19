//
// Created by gorev on 14.08.2025.
//

#include "Component/IRenderComponent.h"

#include "Engine/Engine.h"
#include "Render/GraphicsDevice.h"


IRenderComponent::IRenderComponent(const std::shared_ptr<GameObject>& gameObject, const std::string& vs, const std::string& ps,
                 const std::string& gs, D3D_PRIMITIVE_TOPOLOGY topology) : gameObject(gameObject), v_shader(vs),
                                                                           p_shader(ps), g_shader(gs),
                                                                           topology(topology)
{
    transform = gameObject->GetComponent<Transform>();
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

const std::weak_ptr<GameObject>& IRenderComponent::GetGameObject() const
{
    return gameObject;
}
