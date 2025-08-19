//
// Created by gorev on 30.06.2025.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>
#include <vector>
#include <Windows.h>

#include "Timer.h"
#include "Component/GameObject.h"
#include "Render/Graphics.h"
#include "Render/WindowContainer.h"

class RenderSubsystem;

#define SEngine Engine::Instance()

class Engine : public WindowContainer
{
    Engine();

public:
    Engine(Engine const&) = delete;

    void operator =(Engine const&) = delete;

    ~Engine();

    static Engine& Instance();

    void Initialize(HINSTANCE hinstance, std::string title, std::string window_class, int width, int height);

    void Initialize(HWND hwnd, int width, int height);

    void Run();

    void Update();

    void RenderFrame() const;

    bool ProcessMessages() const;

    bool IsRunning() const;

    const Timer& GetTimer();

    float GetDeltaTime() const;

    Graphics& GetGraphics() const;

    std::vector<std::weak_ptr<GameObject>>& GetGameObjects();

    void SpawnGameObject(std::shared_ptr<GameObject> object);

private:
    std::unique_ptr<RenderSubsystem> renderSubsystem;
    std::unique_ptr<Graphics> graphics;
    Timer timer;
    float deltaTime = 0.0f;
    bool isRunning = true;

    std::vector<std::weak_ptr<GameObject>> gameObjects;
};


#endif //ENGINE_H
