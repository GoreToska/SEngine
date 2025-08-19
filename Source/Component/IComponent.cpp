//
// Created by gorev on 14.08.2025.
//

#include "Component/IComponent.h"

IComponent::~IComponent()
{
}

void IComponent::Update(const float& deltaTime)
{
}

bool IComponent::IsEnabled() const
{
    return m_isEnabled;
}

void IComponent::SetEnabled(bool value)
{
    m_isEnabled = value;
}
