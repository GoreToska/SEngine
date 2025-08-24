//
// Created by gorev on 24.08.2025.
//

#include "Component/LightComponent.h"

LightComponent::LightComponent(const std::shared_ptr<Transform>& transform)
    : Super(), transform(transform)
{
}

void LightComponent::Update(const float& deltaTime)
{
    Super::Update(deltaTime);
}

float LightComponent::GetIntensity() const
{
    return intensity;
}

void LightComponent::SetIntensity(const float intensity)
{
    this->intensity = intensity;
}

Vector3D LightComponent::GetColor() const
{
    return color;
}

void LightComponent::SetColor(const Vector3D& color)
{
    this->color = color;
}

std::weak_ptr<Transform> LightComponent::GetTransform() const
{
    return transform;
}

LightComponent::LightType LightComponent::GetLightType() const
{
    return type;
}

Vector3D LightComponent::GetPosition() const
{
    return transform.lock()->GetPosition();
}
