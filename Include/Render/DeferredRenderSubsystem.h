//
// Created by gorev on 13.08.2025.
//

#ifndef DEFERREDRENDERSUBSYSTEM_H
#define DEFERREDRENDERSUBSYSTEM_H
#include "GBuffer.h"
#include "RenderSubsystem.h"


class DeferredRenderSubsystem : public RenderSubsystem
{
    typedef RenderSubsystem Super;

public:
    virtual void Initialize(HWND hwnd, int width, int height) override;

    virtual void Render(std::vector<std::weak_ptr<IRenderComponent>>& objectsToRender,
                        std::weak_ptr<CameraComponent> cameraComponent,
                        std::vector<std::weak_ptr<LightComponent>>& lightComponents) override;

protected:
    std::unique_ptr<GBuffer> gBuffer;

    void DrawLight(std::vector<std::weak_ptr<LightComponent>>& lightComponents);

    void SetLightBuffer(std::shared_ptr<LightComponent>& lightComponent);

    void DrawFullScreenQuad();
};


#endif //DEFERREDRENDERSUBSYSTEM_H
