//
// Created by gorev on 13.08.2025.
//

#include "Input/Keyboard.h"

#include <iostream>

Keyboard::Keyboard()
{
    autoRepeatKeys = false;
    autoRepeatChars = false;

    for (int i = 0; i < 256; ++i)
    {
        keyStates[i] = false;
    }
}

Keyboard& Keyboard::Instance()
{
    static Keyboard instance;

    return instance;
}

bool Keyboard::IsKeyPressed(const unsigned char keyCode) const
{
    return keyStates[keyCode];
}

void Keyboard::KeyPressed(const unsigned char keyCode)
{
    keyStates[keyCode] = true;
    KeyPressedEvent.Broadcast(keyCode);
}

void Keyboard::KeyReleased(const unsigned char keyCode)
{
    keyStates[keyCode] = false;
    KeyReleasedEvent.Broadcast(keyCode);
}

void Keyboard::CharPressed(const unsigned char character)
{
    CharPressedEvent.Broadcast(character);
}

void Keyboard::EnableAutoRepeatKeys()
{
    autoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
    autoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatChars()
{
    autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatChars()
{
    autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat() const
{
    return autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat() const
{
    return autoRepeatChars;
}
