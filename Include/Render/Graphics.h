//
// Created by gorev on 13.08.2025.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <set>

#include "Engine/Engine.h"
#include "RenderSubsystem.h"
#include "ShaderManager.h"
#include "Component/CameraComponent.h"
#include "Component/IRenderComponent.h"
#include "Component/LightComponent.h"
#include "Utilities/SMath.h"

class Graphics
{
public:
    enum RenderType
    {
        FORWARD = 0,
        DEFERRED = 1,
    };

    bool Initialize(HWND hwnd, int width, int height, RenderType renderType = DEFERRED);

    void RenderFrame();

    void RenderParticles();

    void SetClientSize(int width, int height);

    ShaderManager& GetShaderManager() const;

    void AddObjectToRenderPool(std::shared_ptr<IRenderComponent> object);

    void AddLightToLightPool(std::shared_ptr<LightComponent> object);

    void AddCamera(const std::weak_ptr<CameraComponent>& camera);

    void SetCurrentCamera(const std::weak_ptr<CameraComponent>& camera);

    void SetCurrentCamera(int index);

    int GetClientWidth() const;

    int GetClientHeight() const;

    RenderSubsystem& GetRenderSubsystem() const;

    RenderType GetRenderType() const;

    std::weak_ptr<CameraComponent> GetCurrentCamera() const;

private:
    void RenderObjects();

    void RenderShadowMap();

    void CreateViewport();

    void InitializeShaders();

    int clientWidth;
    int clientHeight;
    RenderType renderType;

    std::unique_ptr<RenderSubsystem> render_subsystem;
    std::unique_ptr<ShaderManager> shader_manager;

    std::vector<std::weak_ptr<IRenderComponent>> renderObjects;
    std::vector<std::weak_ptr<LightComponent>> lightObjects;

    std::set<std::weak_ptr<CameraComponent>, smath::WeakPtrComparator> cameraComponents;
    std::weak_ptr<CameraComponent> currentCamera;
};


#endif //GRAPHICS_H
