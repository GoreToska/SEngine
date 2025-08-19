//
// Created by gorev on 15.08.2025.
//

#include "Component/MeshRender.h"

#include "Engine/Engine.h"
#include "Engine/ModelImporter.h"
#include "Utilities/Asserter.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, const Material& material)
    : vertices(vertices),
      indices(indices),
      material(material)
{
    ThrowIfFailed(
        vertexBuffer.Initialize(
            this->vertices.data(),
            this->vertices.size()),
        "Failed to initialize vertex buffer");

    ThrowIfFailed(
        indexBuffer.Initialize(
            this->indices.data(),
            this->indices.size()),
        "Failed to initialize index buffer");
}

void Mesh::Render()
{
    ID3D11ShaderResourceView* nullResource = nullptr;
    for (size_t i = 0; i < 7; i++)
    {
        SDeviceContext->PSSetShaderResources(i, 1, &nullResource);
    }

    if (material.diffuseTexture.Get())
        SDeviceContext->PSSetShaderResources(0, 1, material.diffuseTexture.GetAddressOf());
    if (material.specularTexture.Get())
        SDeviceContext->PSSetShaderResources(1, 1, material.specularTexture.GetAddressOf());
    if (material.normalTexture.Get())
        SDeviceContext->PSSetShaderResources(2, 1, material.normalTexture.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    SDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    if (indices.size() > 0)
    {
        SDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        SDeviceContext->DrawIndexed(indices.size(), 0, 0);
    }
    else
    {
        SDeviceContext->Draw(vertices.size(), 0);
    }
}

MeshRender::MeshRender(const std::shared_ptr<GameObject>& gameObject, const std::string& path, const std::string& vs,
                       const std::string& ps, const std::string& gs, D3D_PRIMITIVE_TOPOLOGY topology)
    : Super(gameObject, vs, ps, gs, topology)
{
    ModelImporter::LoadModel(path, meshes);
}

void MeshRender::Render()
{
    SetShaders();
    Super::Render();

    for (auto& mesh: meshes)
    {
        mesh.Render();
    }
}
