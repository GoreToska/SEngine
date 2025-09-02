//
// Created by gorev on 29.08.2025.
//

#include "GameObjects/Skybox.h"

Skybox::Skybox(const std::filesystem::path& path, const std::vector<std::wstring>& textures)
    : Super()
{
    skyboxComponent = AddComponent<SkyBoxComponent>(path, textures);
}
