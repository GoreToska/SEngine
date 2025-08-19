//
// Created by gorev on 18.08.2025.
//

#include "GameObjects/SCamera.h"

#include "Component/CameraComponent.h"

SCamera::SCamera(const Vector3D position, const Quaternion rotation, const Vector3D scale)
{
    transform = std::make_shared<Transform>(position, rotation, scale);
    camera = AddComponent<CameraComponent>();
}

std::weak_ptr<CameraComponent> SCamera::GetCamera()
{
    return camera;
}
