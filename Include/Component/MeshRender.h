//
// Created by gorev on 15.08.2025.
//

#ifndef MESHRENDER_H
#define MESHRENDER_H
#include "IRenderComponent.h"
#include "Render/Material.h"
#include "Render/ModelBuffer.h"

class Mesh
{
    // TODO: may be every material should have separate shader?
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, const Material& material,
         const Matrix& parentMatrix = Matrix::Identity);

    Mesh(const Mesh& other);

    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(const Mesh& other);

    Mesh& operator=(Mesh&& other) noexcept;

    void Render();

    const Matrix& GetParentMatrix() const;

    //void SetShaders();

private:
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;
    VertexBuffer<Vertex> vertexBuffer;
    IndexBuffer indexBuffer;

    Material material;
    ConstBuffer<MaterialBuffer> objectMaterialBuffer;

    Matrix parentMatrix;
};

class MeshRender : public IRenderComponent
{
    typedef IRenderComponent Super;

public:
    MeshRender(const std::weak_ptr<Transform>& transform, const std::filesystem::path& path,
               const std::string& vs = "Default_Deferred_VS",
               const std::string& ps = "Default_Deferred_PS",
               const std::string& gs = "",
               D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    MeshRender(const std::weak_ptr<Transform>& transform,
               std::vector<Vertex>& vertices,
               const Color color,
               const std::string& vs = "Default_Deferred_VS",
               const std::string& ps = "Default_Deferred_PS",
               const std::string& gs = "",
               D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    virtual void Render(std::weak_ptr<CameraComponent> camera) override;

protected:
    std::vector<Mesh> meshes;
};

#endif //MESHRENDER_H
