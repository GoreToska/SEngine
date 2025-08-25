//
// Created by gorev on 14.08.2025.
//

#ifndef IRENDER_H
#define IRENDER_H
#include <memory>

#include "../GameObjects/GameObject.h"
#include "IComponent.h"
#include "Transform.h"
#include "Render/ConstBuffer.h"


struct PS_MaterialBuffer;

class IRenderComponent : public IComponent
{
    typedef IComponent Super;

public:
    IRenderComponent(const std::weak_ptr<Transform>& transform, const std::string& vs = "Default_Deferred_VS",
            const std::string& ps = "Default_Deferred_PS",
            const std::string& gs = "", D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    virtual ~IRenderComponent();

    virtual void SetShaders();

    virtual void Update(const float& deltaTime) override;

    virtual void Render();

    const std::weak_ptr<Transform>& GetTransform() const;

protected:
    std::weak_ptr<Transform> transform;

    std::string v_shader;
    std::string p_shader;
    std::string g_shader;
    D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};


#endif //IRENDER_H
