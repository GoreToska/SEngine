//
// Created by gorev on 24.08.2025.
//

#include "Component/DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(const std::shared_ptr<Transform>& transform)
    : Super(transform)
{
    type = LightType::Directional;
}

void DirectionalLightComponent::Update(const float& deltaTime)
{
    Super::Update(deltaTime);

    direction = transform.lock()->GetForward();
}

Vector3D DirectionalLightComponent::GetDirection() const
{
    return direction;
}
