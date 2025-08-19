//
// Created by gorev on 18.08.2025.
//

#include "../../Include/Component/CameraComponent.h"

#include <Windows.Media.Audio.h>

#include "Engine/Engine.h"
#include "Utilities/SMath.h"

CameraComponent::CameraComponent(const Vector3D position, const Quaternion rotation, const Vector3D scale,
                                 const ProjectionType type)
    : Super(position, rotation, scale), projectionType(type)
{
    fov = smath::ToRadians(90);
    aspectRatio = static_cast<float>(SEngine.GetGraphics().GetClientWidth()) /
        static_cast<float>(SEngine.GetGraphics().GetClientHeight());
    orthoWidth = static_cast<float>(SEngine.GetGraphics().GetClientWidth()) / 100.0f;
    orthoHeight = static_cast<float>(SEngine.GetGraphics().GetClientHeight()) / 100.0f;

    SetProjectionType(type);
}

void CameraComponent::Update(const float& deltaTime)
{
    Super::Update(deltaTime);

    viewMatrix = Matrix::CreateLookAt(position, position + GetForward(), Vector3D::Up);
}

void CameraComponent::SetProjectionType(const ProjectionType type)
{
    if (type == ProjectionType::Perspective)
        SetPerspectiveProjection();
    else
        SetOrthographicProjection();
}

void CameraComponent::SetPerspectiveProjection()
{
    projectionMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearZ, farZ);
}

void CameraComponent::SetOrthographicProjection()
{
    projectionMatrix = Matrix::CreateOrthographic(orthoWidth, orthoHeight, nearZ, farZ);
}

void CameraComponent::SetPerspectiveProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    fov = smath::ToRadians(fovDegrees);
    aspectRatio = aspectRatio;
    this->nearZ = nearZ;
    this->farZ = farZ;
    SetPerspectiveProjection();
}

void CameraComponent::SetOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
    orthoWidth = viewWidth;
    orthoHeight = viewHeight;
    this->nearZ = nearZ;
    this->farZ = farZ;
    SetOrthographicProjection();
}

Matrix CameraComponent::GetViewMatrix() const
{
    return viewMatrix;
}

Matrix CameraComponent::GetProjectionMatrix() const
{
    return projectionMatrix;
}

float CameraComponent::GetNearZ() const
{
    return nearZ;
}

float CameraComponent::GetFarZ() const
{
    return farZ;
}

float CameraComponent::GetFOV() const
{
    return fov;
}

float CameraComponent::GetAspectRatio() const
{
    return aspectRatio;
}

CameraComponent::ProjectionType CameraComponent::GetProjectionType() const
{
    return projectionType;
}

void CameraComponent::GetFrustumCorners(std::vector<Vector4D>& corners) const
{
    Matrix viewProjectionMatrix = projectionMatrix * viewMatrix;
    viewProjectionMatrix.Invert();
    corners.clear();
    corners.reserve(8);

    for (int x = 0; x < 2; ++x)
    {
        for (int y = 0; y < 2; ++y)
        {
            for (int z = 0; z < 2; ++z)
            {
                const Vector4D point = Vector4D::Transform(Vector4D(
                                                               2.0f * x - 1.0f,
                                                               2.0f * y - 1.0f,
                                                               z,
                                                               1.0f
                                                           ), viewProjectionMatrix);
                corners.push_back(point / point.w);
            }
        }
    }
}
