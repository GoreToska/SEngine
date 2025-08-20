//
// Created by gorev on 18.08.2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "Component/CameraComponent.h"
#include "GameObject.h"


class SCamera : public GameObject
{
    typedef GameObject Super;

public:
    SCamera(const Vector3D position = Vector3D::Zero,
            const Quaternion rotation = Quaternion::Identity,
            const Vector3D scale = Vector3D::One);

    std::weak_ptr<CameraComponent> GetCamera();

protected:
    std::shared_ptr<CameraComponent> camera;
};


#endif //CAMERA_H
