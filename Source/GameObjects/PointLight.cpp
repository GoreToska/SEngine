//
// Created by gorev on 25.08.2025.
//

#include "GameObjects/PointLight.h"


PointLight::PointLight(const Vector3D position, const Quaternion rotation, const Vector3D scale)
    : Super(position, rotation, scale)
{
    std::filesystem::path path = std::filesystem::current_path().parent_path() / "Data" / "Light" / "PointLight.gltf";
    lightComponent = AddComponent<PointLightComponent>(transform);
    debugMesh = AddComponent<MeshRender>(transform, path);
}
