//
// Created by gorev on 25.08.2025.
//

#include "Component/PointLightComponent.h"

#include "Component/LightComponent.h"

PointLightComponent::PointLightComponent(const std::shared_ptr<Transform>& transform)
    : Super(transform)
{
    type = LightType::Point;
}

void PointLightComponent::SetAttenuation(const float constant, const float linear, const float exponent)
{
    attenuation_const = constant;
    attenuation_linear = linear;
    attenuation_exponent = exponent;
}

float PointLightComponent::GetAttenuationConst() const
{
    return attenuation_const;
}

float PointLightComponent::GetAttenuationLinear() const
{
    return attenuation_linear;
}

float PointLightComponent::GetAttenuationExponent() const
{
    return attenuation_exponent;
}
