//
// Created by gorev on 24.08.2025.
//

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <memory>

#include "Component/DirectionalLightComponent.h"


class DirectionalLight : public GameObject
{
    typedef GameObject Super;
public:
    DirectionalLight(const Vector3D position = Vector3D::Zero,
                     const Quaternion rotation = Quaternion::Identity,
                     const Vector3D scale = Vector3D::One);

protected:
    std::shared_ptr<DirectionalLightComponent> lightComponent;
};


#endif //DIRECTIONALLIGHT_H
