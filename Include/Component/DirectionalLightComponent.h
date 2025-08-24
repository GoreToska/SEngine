//
// Created by gorev on 24.08.2025.
//

#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H
#include "LightComponent.h"


class DirectionalLightComponent : public LightComponent
{
    typedef LightComponent Super;

public:
    DirectionalLightComponent(const std::shared_ptr<Transform>& transform);

    void Update(const float& deltaTime) override;

    Vector3D GetDirection() const;

protected:
    Vector3D direction;
};


#endif //DIRECTIONALLIGHTCOMPONENT_H
