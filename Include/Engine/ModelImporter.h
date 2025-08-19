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
public:
    static void LoadModel(const std::string& path, std::vector<Mesh>& out_meshes);

    static void PrintSupportedFormats();

private:
    static void ProcessNode(const aiNode& node, const aiScene& scene, std::vector<Mesh>& out_meshes);

    static Mesh ProcessMesh(const aiMesh& mesh, const aiMaterial& material);
};


#endif //MODELIMPORTER_H
