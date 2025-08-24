//
// Created by gorev on 13.08.2025.
//

#include "Render/Graphics.h"

#include "Render/DeferredRenderSubsystem.h"
#include "Utilities/Logger.h"

bool Graphics::Initialize(HWND hwnd, const int width, const int height, const RenderType renderType)
{
    SetClientSize(width, height);
    this->renderType = renderType;

    if (renderType == FORWARD)
    {
        SERROR("Not implemented yet!");
        exit(-1);
    }
    if (renderType == DEFERRED)
    {
        render_subsystem = std::make_unique<DeferredRenderSubsystem>();
    }

    render_subsystem->Initialize(hwnd, width, height);
    shader_manager = std::make_unique<ShaderManager>("../");
    renderObjects.reserve(30);

    return true;
}

RenderSubsystem& Graphics::GetRenderSubsystem() const
{
    return *render_subsystem;
}

Graphics::RenderType Graphics::GetRenderType() const
{
    return renderType;
}

std::weak_ptr<CameraComponent> Graphics::GetCurrentCamera() const
{
    return currentCamera;
}

void Graphics::RenderFrame()
{
    render_subsystem->RenderShadowMap(renderObjects, currentCamera);

    render_subsystem->Render(renderObjects, currentCamera, lightObjects);
}

void Graphics::RenderParticles()
{
    SERROR("Not implemented yet!");
}

void Graphics::SetClientSize(const int width, const int height)
{
    clientWidth = width;
    clientHeight = height;
}

ShaderManager& Graphics::GetShaderManager() const
{
    return *shader_manager;
}

void Graphics::AddObjectToRenderPool(std::shared_ptr<IRenderComponent> object)
{
    renderObjects.emplace_back(object);
}

void Graphics::AddLightToLightPool(std::shared_ptr<LightComponent> object)
{
    lightObjects.emplace_back(object);
}

void Graphics::AddCamera(const std::weak_ptr<CameraComponent>& camera)
{
    if (cameraComponents.contains(camera))
        return;

    cameraComponents.insert(camera);

    if (cameraComponents.size() == 1)
        currentCamera = camera;
}

void Graphics::SetCurrentCamera(const std::weak_ptr<CameraComponent>& camera)
{
    if (cameraComponents.empty())
        return;

    for (const auto& component: cameraComponents)
    {
        if (auto locked = component.lock())
        {
            if (locked.get() == camera.lock().get())
            {
                currentCamera = component;
                return;
            }
        }
    }
}

void Graphics::SetCurrentCamera(const int index)
{
    currentCamera = *std::next(cameraComponents.begin(), index);
}

int Graphics::GetClientWidth() const
{
    return clientWidth;
}

int Graphics::GetClientHeight() const
{
    return clientHeight;
}
