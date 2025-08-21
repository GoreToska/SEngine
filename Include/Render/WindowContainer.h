//
// Created by gorev on 13.08.2025.
//

#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include <memory>
#include <string>
#include <Windows.h>

class Graphics;
class RenderWindow;

class WindowContainer
{
public:
    WindowContainer();

    ~WindowContainer();

    void InitializeWindowContainer(HINSTANCE hinstance, std::string title, std::string window_class, int width, int height);

    static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    RenderWindow& GetRenderWindow() const;

protected:
    std::unique_ptr<RenderWindow> renderWindow;
};


#endif //WINDOWCONTAINER_H
