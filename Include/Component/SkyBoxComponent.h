//
// Created by gorev on 29.08.2025.
//

#ifndef SKYBOXCOMPONENT_H
#define SKYBOXCOMPONENT_H
#include <filesystem>
#include <wrl/client.h>
#include <d3d11.h>

#include "CameraComponent.h"
#include "IComponent.h"
#include "Render/ModelBuffer.h"


class SkyBoxComponent : public IComponent
{
    typedef IComponent Super;

public:
    SkyBoxComponent(const std::filesystem::path& path,
                    const std::vector<std::wstring>& textures,
                    const std::string& vs = "Default_Skybox_VS",
                    const std::string& ps = "Default_Skybox_PS");

    void Render();

    void SetShaders() const;

private:
    std::filesystem::path path;
    std::vector<std::wstring> textureNames;

    std::vector<Vector3D> vertices;
    std::vector<DWORD> indices;
    VertexBuffer<Vector3D> vertexBuffer;
    IndexBuffer indexBuffer;

    std::string ps;
    std::string vs;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;

    void InitializeSkybox();

    Microsoft::WRL::ComPtr<ID3D11Texture2D> LoadSkyboxFace(const std::filesystem::path& path);
};


#endif //SKYBOXCOMPONENT_H
