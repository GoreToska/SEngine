//
// Created by gorev on 14.08.2025.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "IComponent.h"
#include "Engine/EngineTypes.h"


class Transform : public IComponent
{
    typedef IComponent Super;

public:
    Transform(const Vector3D position = Vector3D::Zero,
              const Quaternion rotation = Quaternion::Identity,
              const Vector3D scale = Vector3D::One);

    virtual ~Transform() override;

    virtual void Update(const float& deltaTime) override;

    void SetPosition(const Vector3D position);

    void MovePosition(const Vector3D delta);

    void SetRotation(const Quaternion rotation);

    void SetRotation(const Vector3D rotation);

    void AddRotation(const Vector3D rotation);

    void AddRotation(const Vector3D axis, const float angle);

    void AddRotation(const Quaternion rotation);

    void SetScale(const Vector3D scale);

    Vector3D GetPosition() const;

    Quaternion GetRotation() const;

    Vector3D GetScale() const;

    Matrix GetWorldMatrix() const;

    Vector3D GetForward() const;

    Vector3D GetUp() const;

    Vector3D GetRight() const;

protected:
    Vector3D position;
    Quaternion rotation;
    Vector3D scale;
    Matrix objectMatrix;

    Matrix CalculateWorldMatrix() const;
};


#endif //TRANSFORM_H
