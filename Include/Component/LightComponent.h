//
// Created by gorev on 24.08.2025.
//

#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H
#include <memory>

#include "MeshRender.h"
#include "Engine/EngineTypes.h"


class LightComponent : public IComponent
{
    typedef IComponent Super;

public:
    enum LightType
    {
        None = 0,
        Directional = 1,
        Point = 2,
        Spot = 3,
    };

    LightComponent(const std::shared_ptr<Transform>& transform);

    void Update(const float& deltaTime) override;

    float GetIntensity() const;

    void SetIntensity(float intensity);

    Vector3D GetColor() const;

    void SetColor(const Vector3D& color);

    std::weak_ptr<Transform> GetTransform() const;

    LightType GetLightType() const;

    Vector3D GetPosition() const;

protected:
    std::weak_ptr<Transform> transform;

    Vector3D color = Vector3D(1, 1, 1);
    float intensity = 1.0f;
    LightType type = LightType::None;
};


#endif //LIGHTCOMPONENT_H
