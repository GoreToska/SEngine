//
// Created by gorev on 13.08.2025.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Utilities/Delegate.h"


class Keyboard
{
public:
    Keyboard(Keyboard&) = delete;

    void operator=(Keyboard&) = delete;

    static Keyboard& Instance();

    bool IsKeyPressed(const unsigned char keyCode) const;

    void KeyPressed(const unsigned char keyCode);

    void KeyReleased(const unsigned char keyCode);

    void CharPressed(const unsigned char character);

    void EnableAutoRepeatKeys();

    void DisableAutoRepeatKeys();

    void EnableAutoRepeatChars();

    void DisableAutoRepeatChars();

    bool IsKeysAutoRepeat() const;

    bool IsCharsAutoRepeat() const;

    MulticastDelegate<const unsigned char> KeyPressedEvent;
    MulticastDelegate<const unsigned char> KeyReleasedEvent;
    MulticastDelegate<const unsigned char> CharPressedEvent;

private:
    Keyboard();

    bool autoRepeatKeys;
    bool autoRepeatChars;
    bool keyStates[256];
};


#endif //KEYBOARD_H
