//
// Created by gorev on 14.08.2025.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <map>
#include <memory>
#include <typeindex>

#include "Component/IComponent.h"
#include "Component/Transform.h"
#include "Engine/EngineTypes.h"
#include "Utilities/Logger.h"


class GameObject
{
public:
    GameObject(const Vector3D position = Vector3D::Zero,
               const Quaternion rotation = Quaternion::Identity,
               const Vector3D scale = Vector3D::One)
    {
        transform = std::make_shared<Transform>(position, rotation, scale);
    };

    virtual ~GameObject()
    {
    };

    bool IsEnabled() const
    {
        return m_isEnabled;
    }

    void SetEnabled(const bool value)
    {
        m_isEnabled = value;
    }

    virtual void Update(const float deltaTime)
    {
        if (!IsEnabled())
            return;

        transform->Update(deltaTime);

        for (const auto& component: components)
        {
            component.second->Update(deltaTime);
        }
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<IComponent, T>>, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args)
    {
        if constexpr (std::is_same_v<T, Transform>)
        {
            SERROR("Can't add Transform, this GameObject already has one.");
            return nullptr;
        }

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        components[std::type_index(typeid(T))] = component;
        return component;
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<IComponent, T>>>
    std::shared_ptr<T> GetComponent()
    {
        if constexpr (std::is_same_v<T, Transform>)
            return transform;

        for (const auto& it: components)
        {
            auto result = dynamic_pointer_cast<T>(it.second);

            if (result)
            {
                return result;
            }
        }

        return nullptr;
    }

    std::weak_ptr<Transform> GetTransform()
    {
        return transform;
    }

protected:
    std::map<std::type_index, std::shared_ptr<IComponent>> components;
    std::shared_ptr<Transform> transform;
    bool m_isEnabled = true;
};


#endif //GAMEOBJECT_H
