//
// Created by gorev on 13.08.2025.
//

#include "Component/CameraComponent.h"
#include "../Include/GameObjects/GameObject.h"
#include "Component/IRenderComponent.h"
#include "Engine/Engine.h"
#include "Engine/ModelImporter.h"
#include "GameObjects/DirectionalLight.h"
#include "GameObjects/FirstPersonPlayer.h"
#include "GameObjects/SCamera.h"

int main()
{
    std::string applicationName = "SEngine";
    std::string windowClass = "WindowClass";
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    auto modelPath = std::filesystem::current_path().parent_path() / "Data";
    constexpr float resolution = 70;
    SEngine.Initialize(hInstance, applicationName, windowClass, 16 * resolution, 9 * resolution);
    auto go = std::make_shared<GameObject>();
    //go->AddComponent<MeshRender>(go, modelPath / "NanoSuit" / "nanosuit.obj");
    //go->AddComponent<MeshRender>(go, modelPath / "Orange" / "orange_disktexture.fbx");
    go->AddComponent<MeshRender>(go, (modelPath / "Card" / "MemoryCard.obj").string());
    SEngine.SpawnGameObject(go);

    auto go2 = std::make_shared<GameObject>(Vector3D(4, 0, 0), Quaternion::Identity, Vector3D(5, 5, 5));
    go2->AddComponent<MeshRender>(go2, modelPath / "Mark2" / "Mark2.gltf");
    SEngine.SpawnGameObject(go2);

    auto go3 = std::make_shared<GameObject>(Vector3D(-4, 0, 0), Quaternion::Identity, Vector3D(5, 5, 5));
    go3->AddComponent<MeshRender>(go2, modelPath / "BrickWall" / "brick_wall.obj");
    SEngine.SpawnGameObject(go3);


    auto direct_light = std::make_shared<DirectionalLight>();
    SEngine.SpawnGameObject(direct_light);

    ModelImporter::PrintSupportedFormats();

    /*auto cameraObj = std::make_shared<SCamera>();
    SEngine.SpawnGameObject(cameraObj);
    SEngine.GetGraphics().SetCurrentCamera(cameraObj->GetCamera());*/

    auto player = std::make_shared<FirstPersonPlayer>();
    SEngine.SpawnGameObject(player);

    //ModelImporter::PrintSupportedFormats();

    while (SEngine.IsRunning())
    {
        direct_light->GetTransform().lock()->AddRotation({1, 1, 0}, 0.001);
        SEngine.Run();
    }

    return 0;
}
