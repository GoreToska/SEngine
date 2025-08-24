//
// Created by gorev on 15.08.2025.
//

#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H
#include <string>

#include "assimp/scene.h"
#include "Component/MeshRender.h"

struct aiMesh;

class ModelImporter
{
    enum TextureStorage
    {
        None,
        EmbeddedCompressed,
        EmbeddedNonCompressed,
        EmbeddedIndexCompressed,
        EmbeddedIndexNonCompressed,
        Disk
    };

public:
    static void LoadModel(const std::filesystem::path& path, std::vector<Mesh>& out_meshes);

    static void PrintSupportedFormats();

private:
    static void ProcessNode(const std::filesystem::path& path, const aiNode& node, const aiScene& scene,
                            std::vector<Mesh>& out_meshes);

    static Mesh ProcessMesh(const std::filesystem::path& path, const aiMesh& mesh, const aiScene& scene,
                            const aiMaterial& material);

    static Texture GetColorTexture(const aiMaterial& material, aiTextureType type);

    static Texture GetTexture(const std::filesystem::path& path, const aiScene& scene, const aiMaterial& material,
                              aiTextureType type, BOOL& has_texture);

    static TextureStorage GetTextureStorageType(const aiScene& scene, const aiMaterial& material,
                                                size_t index, aiTextureType type);
};


#endif //MODELIMPORTER_H
