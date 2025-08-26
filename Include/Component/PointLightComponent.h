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

    void SetAttenuation(float constant, float linear, float exponent);

    float GetAttenuationConst() const;

    float GetAttenuationLinear() const;

    float GetAttenuationExponent() const;

protected:
    float attenuation_const = 1.0f;
    float attenuation_linear = 0.045f;
    float attenuation_exponent = 0.0075f;
};


#endif //POINTLIGHTCOMPONENT_H
