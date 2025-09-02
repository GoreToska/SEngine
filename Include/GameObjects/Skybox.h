//
// Created by gorev on 29.08.2025.
//

#ifndef SKYBOX_H
#define SKYBOX_H
#include "GameObject.h"
#include "Component/SkyBoxComponent.h"


class Skybox : public GameObject
{
    typedef GameObject Super;

public:
    Skybox(const std::filesystem::path& path, const std::vector<std::wstring>& textures);

private:
    std::shared_ptr<SkyBoxComponent> skyboxComponent;
};


#endif //SKYBOX_H
