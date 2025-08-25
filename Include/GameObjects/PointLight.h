//
// Created by gorev on 25.08.2025.
//

#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "GameObject.h"
#include "Component/PointLightComponent.h"


class PointLight : public GameObject
{
    typedef GameObject Super;

public:
    PointLight(const Vector3D position = Vector3D::Zero,
               const Quaternion rotation = Quaternion::Identity,
               const Vector3D scale = Vector3D::One);

protected:
    std::shared_ptr<PointLightComponent> lightComponent;
    std::shared_ptr<MeshRender> debugMesh;
};


#endif //POINTLIGHT_H
