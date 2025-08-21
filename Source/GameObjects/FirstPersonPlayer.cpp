//
// Created by gorev on 19.08.2025.
//

#include "GameObjects/FirstPersonPlayer.h"

#include <iostream>

#include "Engine/Engine.h"
#include "GameObjects/SCamera.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Render/RenderWindow.h"
#include "Utilities/SMath.h"

FirstPersonPlayer::FirstPersonPlayer(
    const Vector3D position,
    const Quaternion rotation,
    const Vector3D scale)
    : Super(position, rotation, scale)
{
    camera = AddComponent<CameraComponent>(position, rotation, scale);
    Keyboard::Instance().KeyPressedEvent.AddRaw(this, &FirstPersonPlayer::OnMoveButtonPress);
    Keyboard::Instance().KeyReleasedEvent.AddRaw(this, &FirstPersonPlayer::OnMoveButtonRelease);
    Mouse::Instance().DeltaEvent.AddRaw(this, &FirstPersonPlayer::OnLook);
    Mouse::Instance().WheelUpEvent.AddRaw(this, &FirstPersonPlayer::OnWheelUp);
    Mouse::Instance().WheelDownEvent.AddRaw(this, &FirstPersonPlayer::OnWheelDown);
    Mouse::Instance().RightPressEvent.AddRaw(this, &FirstPersonPlayer::OnRightDown);
    Mouse::Instance().RightReleaseEvent.AddRaw(this, &FirstPersonPlayer::OnRightUp);
}

void FirstPersonPlayer::Update(const float deltaTime)
{
    if (!IsEnabled())
        return;

    Super::Update(deltaTime);

    if (!Mouse::Instance().IsRightDown())
        return;

    Vector3D forward = camera->GetForward() * velocity.z;
    Vector3D right = camera->GetRight() * velocity.x;
    Vector3D up = Vector3D(0, velocity.y, 0);

    camera->MovePosition((right + forward + up) * deltaTime * moveSpeed);
    camera->AddRotation(Vector3D::Up, deltaTime * mouseSpeed * lookDirection.y);
    camera->AddRotation(camera->GetRight(), deltaTime * mouseSpeed * lookDirection.x);
    transform->MovePosition((right + forward + up) * deltaTime * moveSpeed);
    transform->AddRotation(Vector3D::Up, deltaTime * mouseSpeed * lookDirection.y);

    lookDirection = Vector3D(0, 0, 0);
}

void FirstPersonPlayer::OnLook(const int x, const int y)
{
    lookDirection = Vector3D(-y, -x, 0);
}

void FirstPersonPlayer::OnRightDown(const int x, const int y)
{
    SetCapture(SEngine.GetRenderWindow().GetHWND());
    ShowCursor(false);
    lastMousePos = Mouse::Instance().GetPosition();
}

void FirstPersonPlayer::OnRightUp(const int x, const int y)
{
    SetCapture(nullptr);
    ShowCursor(true);
    SetCursorPos(lastMousePos.x + SEngine.GetRenderWindow().GetWindowRect()->left,
                 lastMousePos.y + SEngine.GetRenderWindow().GetWindowRect()->top);
}

void FirstPersonPlayer::OnMoveButtonPress(const unsigned char c)
{
    if (c == forwardKey)
    {
        velocity += Vector3D(0, 0, 1);
        return;
    }

    if (c == backwardKey)
    {
        velocity += Vector3D(0, 0, -1);
        return;
    }

    if (c == rightKey)
    {
        velocity += Vector3D(1, 0, 0);
        return;
    }

    if (c == leftKey)
    {
        velocity += Vector3D(-1, 0, 0);
        return;
    }

    if (c == upKey)
    {
        velocity += Vector3D(0, 1, 0);
        return;
    }

    if (c == downKey)
    {
        velocity += Vector3D(0, -1, 0);
        return;
    }
}

void FirstPersonPlayer::OnMoveButtonRelease(const unsigned char c)
{
    if (c == forwardKey)
    {
        velocity += Vector3D(0, 0, -1);
        return;
    }

    if (c == backwardKey)
    {
        velocity += Vector3D(0, 0, 1);
        return;
    }

    if (c == rightKey)
    {
        velocity += Vector3D(-1, 0, 0);
        return;
    }

    if (c == leftKey)
    {
        velocity += Vector3D(1, 0, 0);
        return;
    }

    if (c == upKey)
    {
        velocity += Vector3D(0, -1, 0);
        return;
    }

    if (c == downKey)
    {
        velocity += Vector3D(0, 1, 0);
        return;
    }
}

void FirstPersonPlayer::OnWheelUp(const int x, const int y)
{
    moveSpeed += moveSpeedDelta;
    moveSpeed = std::clamp(moveSpeed, moveSpeedDelta, 1.0f);
}

void FirstPersonPlayer::OnWheelDown(const int x, const int y)
{
    moveSpeed -= moveSpeedDelta;
    moveSpeed = std::clamp(moveSpeed, moveSpeedDelta, 1.0f);
}
