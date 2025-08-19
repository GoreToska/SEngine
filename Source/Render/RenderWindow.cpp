//
// Created by gorev on 13.08.2025.
//

#include "Render/RenderWindow.h"

#include "Render/WindowContainer.h"
#include "Utilities/Logger.h"
#include "Utilities/StringUtilities.h"

RenderWindow::~RenderWindow()
{
}

bool RenderWindow::Initialize(WindowContainer& window, HINSTANCE hinstance, std::string window_title,
                              std::string window_class, int width, int height)
{
    this->hInstance = hinstance;
    this->width = width;
    this->height = height;
    this->windowTitle = window_title;
    this->windowTitleW = ToWideString(window_title);
    this->windowClassName = window_class;
    this->windowClassNameW = ToWideString(window_class);

    RegisterWindowClass();

    int centerX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int centerY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    RECT wr;
    wr.left = centerX;
    wr.top = centerY;
    wr.right = wr.left + width;
    wr.bottom = wr.top + height;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    hWindow = CreateWindowEx(0,
                             windowClassName.c_str(),
                             windowTitle.c_str(),
                             WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                             wr.left, // window X position
                             wr.top, // window Y position
                             wr.right - wr.left, // width
                             wr.bottom - wr.top, // height
                             NULL,
                             NULL,
                             hInstance,
                             &window);

    if (hWindow == NULL)
    {
        SERROR(GetLastError(), "CreateWindowEX failed for window " + window_title);
        return false;
    }

    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);

    return true;
}

bool RenderWindow::Initialize(HWND hwnd, int width, int height)
{
    SERROR("Initialize is not implemented");
    return true;
}

bool RenderWindow::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, hWindow, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_NULL)
    {
        if (!IsWindow(hWindow))
        {
            hWindow = NULL;
            UnregisterClassW(windowClassNameW.c_str(), hInstance);

            return false;
        }
    }

    return true;
}

void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc{};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // flags for redraw on size changed
    wc.lpfnWndProc = HandleMsgSetup; // ptr to window proc func for handling messages from this window
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = windowClassName.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);
}

LRESULT RenderWindow::HandleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_NCCREATE:
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            auto pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

            if (pWindow == nullptr)
            {
                SERROR("Critical error: pointer to window container is null during WM_NCCREATE.");
                exit(-1);
            }

            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

            return pWindow->WndProc(hwnd, uMsg, wParam, lParam);
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT RenderWindow::HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        default:
            const auto pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return pWindow->WndProc(hwnd, uMsg, wParam, lParam);
    }
}

HWND& RenderWindow::GetHWND()
{
    return hWindow;
}

HINSTANCE& RenderWindow::GetHINST()
{
    return hInstance;
}
