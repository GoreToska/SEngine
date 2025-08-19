//
// Created by gorev on 18.08.2025.
//

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#include "Transform.h"


class CameraComponent : public Transform
{
    typedef Transform Super;

    enum ProjectionType
    {
        Perspective,
        Orthographic
    };

public:
    CameraComponent(const Vector3D position = Vector3D::Zero,
                    const Quaternion rotation = Quaternion::Identity,
                    const Vector3D scale = Vector3D::One,
                    ProjectionType type = ProjectionType::Perspective);

    void Update(const float& deltaTime) override;

    void SetProjectionType(ProjectionType type);

    void SetPerspectiveProjection();

    void SetOrthographicProjection();

    void SetPerspectiveProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    void SetOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ);

    Matrix GetViewMatrix() const;

    Matrix GetProjectionMatrix() const;

    float GetNearZ() const;

    float GetFarZ() const;

    float GetFOV() const;

    float GetAspectRatio() const;

    ProjectionType GetProjectionType() const;

    void GetFrustumCorners(std::vector<Vector4D>& corners) const;

protected:
    float fov;
    float aspectRatio;
    float orthoWidth;
    float orthoHeight;
    float nearZ = 0.1f;
    float farZ = 3000.0f;
    ProjectionType projectionType;
    Matrix projectionMatrix;
    Matrix viewMatrix;
};


#endif //CAMERACOMPONENT_H
