//
// Created by gorev on 13.08.2025.
//

#ifndef MOUSE_H
#define MOUSE_H
#include "Engine/EngineTypes.h"
#include "Utilities/Delegate.h"


class Mouse
{
public:
    Mouse(Mouse&) = delete;

    void operator=(Mouse&) = delete;

    static Mouse& Instance();

    void LeftPressed(const int x, const int y);

    void RightPressed(const int x, const int y);

    void MiddlePressed(const int x, const int y);

    void LeftReleased(const int x, const int y);

    void RightReleased(const int x, const int y);

    void MiddleReleased(const int x, const int y);

    void WheelUp(const int x, const int y);

    void WheelDown(const int x, const int y);

    void Move(int x, int y);

    void Delta(int x, int y);

    bool IsLeftDown() const;

    bool IsRightDown() const;

    bool IsMiddleDown() const;

    int GetX() const;

    int GetY() const;

    Vector2D GetPosition() const;

    MulticastDelegate<const int, const int> DeltaEvent;
    MulticastDelegate<const int, const int> MoveEvent;
    MulticastDelegate<const int, const int> LeftPressEvent;
    MulticastDelegate<const int, const int> LeftReleaseEvent;
    MulticastDelegate<const int, const int> RightPressEvent;
    MulticastDelegate<const int, const int> RightReleaseEvent;
    MulticastDelegate<const int, const int> MiddlePressEvent;
    MulticastDelegate<const int, const int> MiddleReleaseEvent;
    MulticastDelegate<const int, const int> WheelUpEvent;
    MulticastDelegate<const int, const int> WheelDownEvent;

private:
    Mouse() = default;

    bool leftIsDown = false;
    bool rightIsDown = false;
    bool middleIsDown = false;

    float x = 0;
    float y = 0;
};


#endif //MOUSE_H
