//
// Created by gorev on 15.08.2025.
//

#include "Engine/ModelImporter.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "assimp/cimport.h"
#include "Utilities/Logger.h"


void ModelImporter::LoadModel(const std::string& path, std::vector<Mesh>& out_meshes)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SERROR("Error loading model: " + std::string(importer.GetErrorString()));
    }

    out_meshes.clear();
    out_meshes.reserve(scene->mNumMeshes);
    ProcessNode(*scene->mRootNode, *scene, out_meshes);
}

void ModelImporter::PrintSupportedFormats()
{
    unsigned int formatCount = aiGetImportFormatCount();
    std::stringstream stream;

    for (unsigned int i = 0; i < formatCount; ++i)
    {
        auto formatDesc = aiGetImportFormatDescription(i);
        stream << formatDesc->mFileExtensions << ", ";
    }

    SLOG("Supported formats: " + stream.str());
}

void ModelImporter::ProcessNode(const aiNode& node, const aiScene& scene, std::vector<Mesh>& out_meshes)
{
    for (size_t i = 0; i < scene.mNumMeshes; ++i)
    {
        auto mesh = scene.mMeshes[i];
        out_meshes.emplace_back(ProcessMesh(*mesh, scene, *scene.mMaterials[mesh->mMaterialIndex]));
    }

    for (size_t i = 0; i < node.mNumChildren; ++i)
    {
        ProcessNode(*node.mChildren[i], scene, out_meshes);
    }
}

Mesh ModelImporter::ProcessMesh(const aiMesh& mesh, const aiScene& scene, const aiMaterial& material)
{
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;
    vertices.reserve(mesh.mNumVertices);
    indices.reserve(mesh.mNumFaces * 3);

    for (size_t i = 0; i < mesh.mNumVertices; ++i)
    {
        Vertex vertex;

        vertex.position.x = mesh.mVertices[i].x;
        vertex.position.y = mesh.mVertices[i].y;
        vertex.position.z = mesh.mVertices[i].z;

        vertex.normal.x = mesh.mNormals[i].x;
        vertex.normal.y = mesh.mNormals[i].y;
        vertex.normal.z = mesh.mNormals[i].z;

        vertex.texCoord.x = mesh.mTextureCoords[0] ? mesh.mTextureCoords[0][i].x : 0.0f;
        vertex.texCoord.y = mesh.mTextureCoords[0] ? mesh.mTextureCoords[0][i].y : 0.0f;

        vertices.emplace_back(vertex);
    }

    for (size_t i = 0; i < mesh.mNumFaces; ++i)
    {
        const aiFace face = mesh.mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    Material mesh_material;
    // TODO: get textures
    material.Get(AI_MATKEY_COLOR_DIFFUSE, mesh_material.diffuseColor);
    material.Get(AI_MATKEY_COLOR_SPECULAR, mesh_material.specularColor);
    material.Get(AI_MATKEY_COLOR_EMISSIVE, mesh_material.emissiveColor);
    material.Get(AI_MATKEY_SHININESS, mesh_material.shininess);

    mesh_material = GetMaterial(aiTextureType_DIFFUSE, material);

    return Mesh(vertices, indices, mesh_material);
}

Material ModelImporter::GetMaterial(aiTextureType type, const aiMaterial& material)
{
    Material res_material;
    if (material.GetTextureCount(type) == 0)
    {
        res_material.diffuseTexture.InitializeTextureWithColor(ERROR_COLOR);
        return res_material;
    }


    std::cout << "Has texture!" << std::endl;
    res_material.diffuseTexture.InitializeTextureWithColor(ERROR_COLOR);
    return res_material;
}
