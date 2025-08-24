//
// Created by gorev on 24.08.2025.
//

#include "GameObjects/DirectionalLight.h"

DirectionalLight::DirectionalLight(const Vector3D position, const Quaternion rotation, const Vector3D scale)
    : Super(position, rotation, scale)
{
    lightComponent = AddComponent<DirectionalLightComponent>(transform);
}
