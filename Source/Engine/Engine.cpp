//
// Created by gorev on 30.06.2025.
//

#include "Engine/Engine.h"

#include <iostream>

#include "Input/Keyboard.h"
#include "Render/RenderWindow.h"
#include "Utilities/Logger.h"

Engine::Engine()
{
    graphics = std::make_unique<Graphics>();
    timer = Timer();
}

Engine::~Engine()
{
    renderWindow.reset();
}

Engine& Engine::Instance()
{
    static Engine instance;

    return instance;
}

void Engine::Initialize(HINSTANCE hinstance, std::string title, std::string window_class, int width, int height)
{
    InitializeWindowContainer(hinstance, title, window_class, width, height);

    if (!graphics->Initialize(renderWindow->GetHWND(), width, height))
    {
        SERROR("Failed to initialize graphics engine!");
        exit(-1);
    }
}

void Engine::Initialize(HWND hwnd, int width, int height)
{
    if (!renderWindow->Initialize(hwnd, width, height))
    {
        SERROR("Failed to initialize render window!");
        exit(-1);
    }

    if (!graphics->Initialize(renderWindow->GetHWND(), width, height))
    {
        SERROR("Failed to initialize graphics engine!");
        exit(-1);
    }
}

void Engine::Run()
{
    isRunning = ProcessMessages();
    Update();
    RenderFrame();
}

void Engine::Update()
{
    deltaTime = timer.GetMilliseconds();
    timer.Restart();

    for (auto it = gameObjects.begin(); it != gameObjects.end();)
    {
        if (const auto object = it->lock())
        {
            object->Update(deltaTime);
            ++it;
        }
        else
        {
            it = gameObjects.erase(it);
        }
    }
}

void Engine::RenderFrame() const
{
    graphics->RenderFrame();
}

bool Engine::ProcessMessages() const
{
    return renderWindow->ProcessMessages();
}

bool Engine::IsRunning() const
{
    return isRunning;
}

const Timer& Engine::GetTimer()
{
    return timer;
}

float Engine::GetDeltaTime() const
{
    return timer.GetMilliseconds();
}

Graphics& Engine::GetGraphics() const
{
    return *graphics;
}

std::vector<std::weak_ptr<GameObject>>& Engine::GetGameObjects()
{
    return gameObjects;
}

void Engine::SpawnGameObject(std::shared_ptr<GameObject> object)
{
    if (const auto comp = object->GetComponent<IRenderComponent>())
        graphics->AddObjectToRenderPool(comp);

    if (const auto comp = object->GetComponent<LightComponent>())
        graphics->AddLightToLightPool(comp);

    if (const auto comp = object->GetComponent<CameraComponent>())
        graphics->AddCamera(comp);

    gameObjects.emplace_back(object);
}
