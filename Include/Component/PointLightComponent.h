//
// Created by gorev on 25.08.2025.
//

#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H
#include "LightComponent.h"


class PointLightComponent : public LightComponent
{
    typedef LightComponent Super;

public:
    PointLightComponent(const std::shared_ptr<Transform>& transform);

protected:
};


#endif //POINTLIGHTCOMPONENT_H
