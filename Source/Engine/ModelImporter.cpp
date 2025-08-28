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


void ModelImporter::LoadModel(const std::filesystem::path& path, std::vector<Mesh>& out_meshes)
{
    if (path.empty())
    {
        SLOG("This path is empty, can't load model.");
        return;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(),
                                             aiProcess_Triangulate |
                                             aiProcess_ConvertToLeftHanded |
                                             aiProcess_GenNormals |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SERROR("Error loading model: " + std::string(importer.GetErrorString()));
    }

    out_meshes.clear();
    out_meshes.reserve(scene->mNumMeshes);
    ProcessNode(path, *scene->mRootNode, *scene, out_meshes);
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

void ModelImporter::ProcessNode(const std::filesystem::path& path, const aiNode& node, const aiScene& scene,
                                std::vector<Mesh>& out_meshes)
{
    for (size_t i = 0; i < scene.mNumMeshes; ++i)
    {
        auto mesh = scene.mMeshes[i];
        out_meshes.emplace_back(ProcessMesh(path, *mesh, scene, *scene.mMaterials[mesh->mMaterialIndex]));
    }

    /*for (size_t i = 0; i < node.mNumChildren; ++i)
    {
        ProcessNode(path, *node.mChildren[i], scene, out_meshes);
    }*/
}

Mesh ModelImporter::ProcessMesh(const std::filesystem::path& path, const aiMesh& mesh, const aiScene& scene,
                                const aiMaterial& material)
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
        vertex.position.z = -mesh.mVertices[i].z;

        vertex.normal.x = mesh.mNormals[i].x;
        vertex.normal.y = mesh.mNormals[i].y;
        vertex.normal.z = mesh.mNormals[i].z;

        if (mesh.mTangents)
        {
            vertex.tangent.x = mesh.mTangents[i].x;
            vertex.tangent.y = mesh.mTangents[i].y;
            vertex.tangent.z = mesh.mTangents[i].z;
        }
        else
        {
            vertex.tangent = Vector3D(0.0f, 0.0f, 0.0f);
        }

        if (mesh.mBitangents)
        {
            vertex.bitangent.x = mesh.mBitangents[i].x;
            vertex.bitangent.y = mesh.mBitangents[i].y;
            vertex.bitangent.z = mesh.mBitangents[i].z;
        }
        else
        {
            vertex.bitangent = Vector3D(0.0f, 0.0f, 0.0f);
        }


        if (mesh.mTextureCoords[0])
        {
            vertex.texCoord.x = mesh.mTextureCoords[0][i].x;
            vertex.texCoord.y = mesh.mTextureCoords[0][i].y;
        }

        vertices.emplace_back(vertex);
    }

    for (int i = 0; i < mesh.mNumFaces; ++i)
    {
        const aiFace face = mesh.mFaces[i];

        for (int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Material mesh_material;
    material.Get(AI_MATKEY_COLOR_DIFFUSE, mesh_material.albedoColor);
    material.Get(AI_MATKEY_METALLIC_FACTOR, mesh_material.metallic);
    material.Get(AI_MATKEY_ROUGHNESS_FACTOR, mesh_material.roughness);

    BOOL has_texture;
    mesh_material.albedoTexture = GetTexture(path, scene, material, Texture::TextureType::Albedo, has_texture);
    mesh_material.normalTexture = GetTexture(path, scene, material, Texture::TextureType::Normal,
                                             mesh_material.normalMapEnabled);
    mesh_material.metallicTexture = GetTexture(path, scene, material, Texture::TextureType::Metallic, has_texture);
    if (!has_texture)
    {
        mesh_material.metallicTexture.InitializeTextureWithColor({0, 0, 0, 1});
    }

    mesh_material.roughnessTexture = GetTexture(path, scene, material, Texture::TextureType::Roughness, has_texture);
    if (!has_texture)
    {
        mesh_material.roughnessTexture.InitializeTextureWithColor({1, 1, 1, 1});
    }

    return Mesh(vertices, indices, mesh_material);
}

Texture ModelImporter::GetColorTexture(const aiMaterial& material, aiTextureType type)
{
    Texture texture;
    Color out_color;
    switch (type)
    {
        case aiTextureType_DIFFUSE:
        {
            if (material.Get(AI_MATKEY_COLOR_DIFFUSE, out_color) == AI_SUCCESS)
            {
                texture.InitializeTextureWithColor(out_color);
            }
            else
            {
                texture.InitializeTextureWithColor(ERROR_COLOR);
            }
            return texture;
        }

        case aiTextureType_SPECULAR:
        {
            material.Get(AI_MATKEY_COLOR_SPECULAR, out_color);
            texture.InitializeTextureWithColor(out_color);
            return texture;
        }

        case aiTextureType_EMISSIVE:
        {
            SERROR("Emissive texture doesn't exist yet.");
            break;
        }
    }

    texture.InitializeTextureWithColor(ERROR_COLOR);
    return texture;
}

Texture ModelImporter::GetTexture(const std::filesystem::path& path, const aiScene& scene, const aiMaterial& material,
                                  Texture::TextureType type, BOOL& has_texture)
{
    // TODO: we can have any count of textures blended together
    // We should consider to use foreach loop
    // But for now this should work fine

    Texture texture;
    aiString texturePath;
    TextureStorage storageType = TextureStorage::None;
    aiReturn result = aiReturn_FAILURE;
    has_texture = FALSE;

    if (type == Texture::TextureType::Albedo)
    {
        result = material.GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
        if (result == AI_SUCCESS)
        {
            storageType = GetTextureStorageType(scene, material, 0, aiTextureType_DIFFUSE);
            has_texture = TRUE;
        }
    }
    else if (type == Texture::TextureType::Normal)
    {
        result = material.GetTexture(aiTextureType_NORMALS, 0, &texturePath);
        if (result == AI_SUCCESS)
        {
            storageType = GetTextureStorageType(scene, material, 0, aiTextureType_NORMALS);
            has_texture = TRUE;
        }
    }
    else if (type == Texture::TextureType::Metallic)
    {
        result = material.GetTexture(AI_MATKEY_METALLIC_TEXTURE, &texturePath);
        if (result == AI_SUCCESS)
        {
            storageType = GetTextureStorageType(scene, material, 0, aiTextureType_METALNESS);
            has_texture = TRUE;
        }
    }
    else if (type == Texture::TextureType::Roughness)
    {
        result = material.GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &texturePath);
        if (result == AI_SUCCESS)
        {
            storageType = GetTextureStorageType(scene, material, 0, aiTextureType_DIFFUSE_ROUGHNESS);
            has_texture = TRUE;
        }
    }
    else if (type == Texture::TextureType::Occlusion)
    {
        result = material.GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texturePath);
        if (result == AI_SUCCESS)
        {
            storageType = GetTextureStorageType(scene, material, 0, aiTextureType_AMBIENT_OCCLUSION);
            has_texture = TRUE;
        }
    }

    switch (storageType)
    {
        case TextureStorage::Disk:
        {
            std::string fileName = (path.parent_path() / texturePath.C_Str()).string();
            std::cout << fileName << std::endl;
            return Texture(fileName, type);
        }
    }

    texture.InitializeTextureWithColor(ERROR_COLOR);
    return texture;
}

ModelImporter::TextureStorage ModelImporter::GetTextureStorageType(const aiScene& scene, const aiMaterial& material,
                                                                   size_t index, aiTextureType type)
{
    if (material.GetTextureCount(type) == 0)
        return TextureStorage::None;

    aiString path;
    material.GetTexture(type, index, &path);
    std::string texturePath = path.C_Str();

    if (texturePath[0] == '*')
    {
        if (scene.mTextures[0]->mHeight == 0)
        {
            return TextureStorage::EmbeddedIndexCompressed;
        }
        else
        {
            SERROR("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!");
            return TextureStorage::EmbeddedIndexNonCompressed;
        }
    }

    if (auto texture = scene.GetEmbeddedTexture(texturePath.c_str()))
    {
        if (texture->mHeight == 0)
        {
            return TextureStorage::EmbeddedCompressed;
        }
        else
        {
            SERROR("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!");
            return TextureStorage::EmbeddedNonCompressed;
        }
    }

    if (texturePath.find('.') != std::string::npos)
    {
        return TextureStorage::Disk;
    }

    return TextureStorage::None;
}
