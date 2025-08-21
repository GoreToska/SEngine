//
// Created by gorev on 13.08.2025.
//

#include "Render/WindowContainer.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Render/Graphics.h"
#include "Render/RenderWindow.h"
#include "Utilities/Logger.h"

WindowContainer::WindowContainer()
{
    renderWindow = std::make_unique<RenderWindow>();
}

WindowContainer::~WindowContainer()
{
    renderWindow.release();
}

void WindowContainer::InitializeWindowContainer(HINSTANCE hinstance, std::string title, std::string window_class,
                                                int width,
                                                int height)
{
    if (!renderWindow->Initialize(*this, hinstance, title, window_class, width, height))
    {
        SERROR("Failed to initialize render window!");
        exit(-1);
    }

    static bool raw_input_initialized = false;

    if (raw_input_initialized == false)
    {
        RAWINPUTDEVICE Rid[2];

        Rid[0].usUsagePage = 0x01;
        Rid[0].usUsage = 0x02;
        Rid[0].dwFlags = 0; // adds HID mouse and also ignores legacy mouse messages
        Rid[0].hwndTarget = renderWindow->GetHWND();

        Rid[1].usUsagePage = 0x01;
        Rid[1].usUsage = 0x06;
        Rid[1].dwFlags = 0; // adds HID keyboard and also ignores legacy keyboard messages
        Rid[1].hwndTarget = renderWindow->GetHWND();

        if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
        {
            SERROR(GetLastError(), "Failed to register raw input devices.");
            exit(-1);
        }

        raw_input_initialized = true;
    }
}

LRESULT WindowContainer::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
#pragma region KeyboardMessages
        // TODO: sys keys down case

        case WM_KEYDOWN:
        {
            unsigned char keycode = static_cast<unsigned char>(wParam);

            if (Keyboard::Instance().IsKeysAutoRepeat())
            {
                Keyboard::Instance().KeyPressed(keycode);
            }
            else
            {
                const bool wasPressed = lParam & 0x40000000;

                if (!wasPressed)
                {
                    Keyboard::Instance().KeyPressed(keycode);
                }
            }

            return 0;
        }

        case WM_KEYUP:
        {
            unsigned char keycode = static_cast<unsigned char>(wParam);
            Keyboard::Instance().KeyReleased(keycode);

            return 0;
        }

        case WM_CHAR:
        {
            unsigned char ch = static_cast<unsigned char>(wParam);

            if (Keyboard::Instance().IsCharsAutoRepeat())
            {
                Keyboard::Instance().CharPressed(ch);
            }
            else
            {
                const bool wasPressed = lParam & 0x40000000;

                if (!wasPressed)
                {
                    Keyboard::Instance().CharPressed(ch);
                }
            }

            return 0;
        }
#pragma endregion

#pragma region MouseMessages
        case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().Move(x, y);

            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().LeftPressed(x, y);

            return 0;
        }
        case WM_LBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().LeftReleased(x, y);

            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().RightPressed(x, y);

            return 0;
        }
        case WM_RBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().RightReleased(x, y);

            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().MiddlePressed(x, y);

            return 0;
        }
        case WM_MBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Mouse::Instance().MiddleReleased(x, y);

            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
                Mouse::Instance().WheelUp(x, y);
            else
                Mouse::Instance().WheelDown(x, y);

            return 0;
        }
        case WM_INPUT:
        {
            UINT dataSize{};
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
            //Need to populate data size first

            if (dataSize > 0)
            {
                std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);

                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize,
                                    sizeof(RAWINPUTHEADER)) == dataSize)
                {
                    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());

                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        Mouse::Instance().Delta(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                    }
                }
            }

            return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
        }
#pragma endregion

        default:
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

RenderWindow& WindowContainer::GetRenderWindow() const
{
    return *renderWindow.get();
}
