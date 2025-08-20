//
// Created by gorev on 19.08.2025.
//

#ifndef FIRSTPERSONPLAYER_H
#define FIRSTPERSONPLAYER_H
#include "GameObject.h"


class CameraComponent;

class FirstPersonPlayer : public GameObject
{
    typedef GameObject Super;

public:
    FirstPersonPlayer(const Vector3D position = Vector3D::Zero,
                      const Quaternion rotation = Quaternion::Identity,
                      const Vector3D scale = Vector3D::One);

    void Update(const float deltaTime) override;

private:
    std::shared_ptr<CameraComponent> camera;

    // TODO: move this to input component
    void OnLook(const int x, const int y);

    // TODO: move this to input component
    void OnMoveButtonPress(const unsigned char c);

    // TODO: move this to input component
    void OnMoveButtonRelease(const unsigned char c);

    void OnWheelUp(const int x, const int y);

    void OnWheelDown(const int x, const int y);

    // TODO: move this to movement component
    Vector3D velocity = Vector3D::Zero;
    Vector3D lookDirection = Vector3D::Zero;
    float moveSpeed = 0.01f;
    float moveSpeedDelta = 0.001f;
    float mouseSpeed = 0.001f;

    // TODO: move this to input component
    unsigned char forwardKey = 'W';
    unsigned char backwardKey = 'S';
    unsigned char leftKey = 'A';
    unsigned char rightKey = 'D';
    unsigned char upKey = 'E';
    unsigned char downKey = 'Q';
};


#endif //FIRSTPERSONPLAYER_H
