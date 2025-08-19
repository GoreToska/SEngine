//
// Created by gorev on 14.08.2025.
//

#ifndef ICOMPONENT_H
#define ICOMPONENT_H


class IComponent
{
public:
    IComponent() = default;

    virtual ~IComponent();

    virtual void Update(const float& deltaTime);

    bool IsEnabled() const;

    void SetEnabled(bool value);

protected:
    bool m_isEnabled = true;
};


#endif //ICOMPONENT_H
