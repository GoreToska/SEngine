//
// Created by gorev on 14.08.2025.
//

#include "Component/Transform.h"

Transform::Transform(const Vector3D position, const Quaternion rotation, const Vector3D scale)
    : position(position), rotation(rotation), scale(scale)
{
    objectMatrix = CalculateWorldMatrix();
}

Transform::~Transform()
{
}

void Transform::Update(const float& deltaTime)
{
    Super::Update(deltaTime);

    objectMatrix = CalculateWorldMatrix();
}

void Transform::SetPosition(const Vector3D position)
{
    this->position = position;
}

void Transform::MovePosition(const Vector3D delta)
{
    this->position += delta;
}

void Transform::SetRotation(const Quaternion rotation)
{
    this->rotation = rotation;
}

void Transform::SetRotation(const Vector3D rotation)
{
    this->rotation = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
}

void Transform::AddRotation(const Vector3D rotation)
{
    //Quaternion yawRotation = Quaternion::CreateFromYawPitchRoll(rotation.y, 0.0f, 0.0f);
    //Quaternion pitchRotation = Quaternion::CreateFromYawPitchRoll(0.0f, rotation.x, 0.0f);
    //Quaternion rollRotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, rotation.z);
    //this->rotation *= yawRotation * pitchRotation * rollRotation;
    this->rotation *= Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
    this->rotation.Normalize();
}

void Transform::AddRotation(Vector3D axis, const float angle)
{
    if (axis == Vector3D::Zero)
        return;

    axis.Normalize();
    this->rotation *= Quaternion::CreateFromAxisAngle(axis, angle);
}

void Transform::AddRotation(const Quaternion rotation)
{
    this->rotation *= rotation;
}

void Transform::SetScale(const Vector3D scale)
{
    this->scale = scale;
}

Vector3D Transform::GetPosition() const
{
    return position;
}

Quaternion Transform::GetRotation() const
{
    return rotation;
}

Vector3D Transform::GetScale() const
{
    return scale;
}

Matrix Transform::GetWorldMatrix() const
{
    return objectMatrix;
}

Vector3D Transform::GetForward() const
{
    return objectMatrix.Forward();
}

Vector3D Transform::GetUp() const
{
    return objectMatrix.Up();
}

Vector3D Transform::GetRight() const
{
    return objectMatrix.Right();
}

Matrix Transform::CalculateWorldMatrix() const
{
    return Matrix::CreateScale(scale)
           * Matrix::CreateFromQuaternion(rotation)
           * Matrix::CreateTranslation(position);
}
