//
// Created by gorev on 13.08.2025.
//

#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <memory>
#include <string>
#include <Windows.h>

class WindowContainer;

class RenderWindow
{
public:
    RenderWindow() = default;

    ~RenderWindow();

    bool Initialize(WindowContainer& window, HINSTANCE hinstance, std::string window_title,
                    std::string window_class, int width, int height);

    bool Initialize(HWND hwnd, int width, int height);

    bool ProcessMessages();

    HWND& GetHWND();

    HINSTANCE& GetHINST();

    std::shared_ptr<RECT> GetWindowRect() const;

protected:
    int width, height;

    HWND hWindow = NULL;
    HINSTANCE hInstance = NULL;

    std::string windowTitle = "";
    std::wstring windowTitleW = L"";
    std::string windowClassName = "";
    std::wstring windowClassNameW = L"";

    void RegisterWindowClass();

    static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //RENDERWINDOW_H
