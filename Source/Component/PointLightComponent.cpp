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
