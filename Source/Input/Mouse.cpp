//
// Created by gorev on 13.08.2025.
//

#include "Input/Mouse.h"

#include <iostream>

Mouse& Mouse::Instance()
{
    static Mouse instance;

    return instance;
}

void Mouse::LeftPressed(const int x, const int y)
{
    leftIsDown = true;
    LeftPressEvent.Broadcast(x, y);
}

void Mouse::RightPressed(const int x, const int y)
{
    rightIsDown = true;
    RightPressEvent.Broadcast(x, y);
}

void Mouse::MiddlePressed(const int x, const int y)
{
    middleIsDown = true;
    MiddlePressEvent.Broadcast(x, y);
}

void Mouse::LeftReleased(const int x, const int y)
{
    leftIsDown = false;
    LeftReleaseEvent.Broadcast(x, y);
}

void Mouse::RightReleased(const int x, const int y)
{
    rightIsDown = false;
    RightReleaseEvent.Broadcast(x, y);
}

void Mouse::MiddleReleased(const int x, const int y)
{
    middleIsDown = false;
    MiddleReleaseEvent.Broadcast(x, y);
}

void Mouse::WheelUp(const int x, const int y)
{
    WheelUpEvent.Broadcast(x, y);
}

void Mouse::WheelDown(const int x, const int y)
{
    WheelDownEvent.Broadcast(x, y);
}

void Mouse::Move(const int x, const int y)
{
    this->x = x;
    this->y = y;
    MoveEvent.Broadcast(x, y);
}

void Mouse::Delta(const int x, const int y)
{
    this->x += x;
    this->y += y;
    DeltaEvent.Broadcast(x, y);
}

bool Mouse::IsLeftDown() const
{
    return leftIsDown;
}

bool Mouse::IsRightDown() const
{
    return rightIsDown;
}

bool Mouse::IsMiddleDown() const
{
    return middleIsDown;
}

int Mouse::GetX() const
{
    return x;
}

int Mouse::GetY() const
{
    return y;
}

Vector2D Mouse::GetPosition() const
{
    return Vector2D{x, y};
}
