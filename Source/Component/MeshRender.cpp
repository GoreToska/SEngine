//
// Created by gorev on 15.08.2025.
//

#include "Component/MeshRender.h"

#include <iostream>

#include "Engine/Engine.h"
#include "Engine/ModelImporter.h"
#include "Utilities/Asserter.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, const Material& material,
           const Matrix& parentMatrix)
    : vertices(vertices),
      indices(indices),
      material(material),
      parentMatrix(parentMatrix)
{
    ThrowIfFailed(
        vertexBuffer.Initialize(
            this->vertices.data(),
            this->vertices.size()),
        "Failed to initialize vertex buffer");

    if (!indices.empty())
    {
        ThrowIfFailed(
            indexBuffer.Initialize(
                this->indices.data(),
                this->indices.size()),
            "Failed to initialize index buffer");
    }

    ThrowIfFailed(
        objectMaterialBuffer.Initialize(),
        "Failed to initialize object material buffer.");
}

Mesh::Mesh(const Mesh& other)
{
    vertices = other.vertices;
    indices = other.indices;
    material = other.material;
    parentMatrix = other.parentMatrix;

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

    ThrowIfFailed(
        objectMaterialBuffer.Initialize(),
        "Failed to initialize object material buffer.");
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices)),
      indices(std::move(other.indices)),
      vertexBuffer(std::move(other.vertexBuffer)),
      indexBuffer(std::move(other.indexBuffer)),
      material(std::move(other.material)),
      objectMaterialBuffer(std::move(other.objectMaterialBuffer)),
      parentMatrix(std::move(other.parentMatrix))

{
    other.vertices.clear();
    other.indices.clear();
}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (&other == this)
        return *this;

    vertices = other.vertices;
    indices = other.indices;
    material = other.material;
    parentMatrix = other.parentMatrix;

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

    ThrowIfFailed(
        objectMaterialBuffer.Initialize(),
        "Failed to initialize object material buffer.");

    return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (&other == this)
        return *this;

    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    material = std::move(other.material);
    vertexBuffer = std::move(other.vertexBuffer);
    indexBuffer = std::move(other.indexBuffer);
    objectMaterialBuffer = std::move(other.objectMaterialBuffer);
    parentMatrix = std::move(other.parentMatrix);
    other.vertices.clear();
    other.indices.clear();
    return *this;
}

void Mesh::Render()
{
    ID3D11ShaderResourceView* nullResource = nullptr;
    for (size_t i = 0; i < 7; i++)
    {
        SDeviceContext->PSSetShaderResources(i, 1, &nullResource);
    }

    if (material.albedoTexture.GetTextureSRV().Get())
        SDeviceContext->PSSetShaderResources(0, 1, material.albedoTexture.GetTextureSRV().GetAddressOf());
    if (material.metallicTexture.GetTextureSRV().Get())
        SDeviceContext->PSSetShaderResources(1, 1, material.metallicTexture.GetTextureSRV().GetAddressOf());
    if (material.normalTexture.GetTextureSRV().Get())
    {
        SDeviceContext->PSSetShaderResources(2, 1, material.normalTexture.GetTextureSRV().GetAddressOf());
        SDeviceContext->VSSetShaderResources(0, 1, material.normalTexture.GetTextureSRV().GetAddressOf());
    }
    if (material.roughnessTexture.GetTextureSRV().Get())
        SDeviceContext->PSSetShaderResources(3, 1, material.roughnessTexture.GetTextureSRV().GetAddressOf());

    SDeviceContext->PSSetConstantBuffers(0, 1, objectMaterialBuffer.GetAddressOf());
    objectMaterialBuffer.GetData()->diffuseColor = material.albedoColor;
    objectMaterialBuffer.GetData()->specularColor = material.specularColor;
    objectMaterialBuffer.GetData()->emissiveColor = material.emissiveColor;
    objectMaterialBuffer.GetData()->normalMapEnabled = material.normalMapEnabled;
    objectMaterialBuffer.GetData()->metallic = material.metallic;
    objectMaterialBuffer.ApplyChanges();

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

const Matrix& Mesh::GetParentMatrix() const
{
    return parentMatrix;
}

MeshRender::MeshRender(const std::weak_ptr<Transform>& transform,
                       const std::filesystem::path& path,
                       const std::string& vs,
                       const std::string& ps, const std::string& gs, D3D_PRIMITIVE_TOPOLOGY topology)
    : Super(transform, vs, ps, gs, topology)
{
    ModelImporter::LoadModel(path, meshes);
}

MeshRender::MeshRender(const std::weak_ptr<Transform>& transform, std::vector<Vertex>& vertices, const Color color,
                       const std::string& vs, const std::string& ps, const std::string& gs,
                       D3D_PRIMITIVE_TOPOLOGY topology)
    : Super(transform, vs, ps, gs, topology)
{
    Material material;
    material.albedoTexture.InitializeTextureWithColor(color);
    meshes.push_back(Mesh(vertices, {}, material));
}

void MeshRender::Render(std::weak_ptr<CameraComponent> camera)
{
    SetShaders();
    Super::Render(camera);

    const auto cameraComponent = camera.lock();
    if (!cameraComponent)
        return;

    auto view = cameraComponent->GetViewMatrix();
    auto projection = cameraComponent->GetProjectionMatrix();
    objectMatrixBuffer.GetData()->view = view.Transpose();
    objectMatrixBuffer.GetData()->projection = projection.Transpose();
    objectMatrixBuffer.GetData()->viewProjection = (view * projection).Transpose();
    objectMatrixBuffer.GetData()->inverseView = view.Invert().Transpose();
    objectMatrixBuffer.GetData()->inverseProjection = projection.Invert().Transpose();

    for (auto& mesh: meshes)
    {
        if (auto transform = GetTransform().lock())
        {
            auto world = mesh.GetParentMatrix() * transform->GetWorldMatrix();
            objectMatrixBuffer.GetData()->world = world.Transpose();
            objectMatrixBuffer.GetData()->inverseWorld = world.Invert().Transpose();
            objectMatrixBuffer.GetData()->worldView = (world * view).Transpose();
            objectMatrixBuffer.GetData()->worldViewProjection = (world * view * projection).Transpose();

            objectMatrixBuffer.ApplyChanges();

            mesh.Render();
        }
    }
}
