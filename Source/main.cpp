//
// Created by gorev on 13.08.2025.
//

#include "Component/CameraComponent.h"
#include "../Include/GameObjects/GameObject.h"
#include "Component/IRenderComponent.h"
#include "Component/PointLightComponent.h"
#include "Engine/Engine.h"
#include "Engine/ModelImporter.h"
#include "GameObjects/DirectionalLight.h"
#include "GameObjects/FirstPersonPlayer.h"
#include "GameObjects/PointLight.h"
#include "GameObjects/SCamera.h"
#include "Input/Keyboard.h"

int main()
{
    std::string applicationName = "SEngine";
    std::string windowClass = "WindowClass";
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    auto modelPath = std::filesystem::current_path().parent_path() / "Data";
    constexpr float resolution = 70;
    SEngine.Initialize(hInstance, applicationName, windowClass, 16 * resolution, 9 * resolution);
    auto point_light = std::make_shared<PointLight>(Vector3D(0, 2, 10), Quaternion::Identity);
    SEngine.SpawnGameObject(point_light);

    auto go = std::make_shared<GameObject>(Vector3D(4, 0, 0));
    //go->AddComponent<MeshRender>(go, modelPath / "NanoSuit" / "nanosuit.obj");
    //go->AddComponent<MeshRender>(go, modelPath / "Orange" / "orange_disktexture.fbx");
    go->AddComponent<MeshRender>(go->GetTransform(), (modelPath / "Card" / "MemoryCard.obj").string());
    SEngine.SpawnGameObject(go);

    auto go2 = std::make_shared<GameObject>(Vector3D(4, 0, 4), Quaternion::Identity, Vector3D(5, 5, 5));
    go2->AddComponent<MeshRender>(go2->GetTransform(), modelPath / "Mark2" / "Mark2.gltf");
    SEngine.SpawnGameObject(go2);

    auto go3 = std::make_shared<GameObject>(Vector3D(-10, 0, 10), Quaternion::Identity, Vector3D(5, 5, 5));
    go3->AddComponent<MeshRender>(go3->GetTransform(), modelPath / "BrickWall" / "brick_wall.obj");
    SEngine.SpawnGameObject(go3);


    //auto direct_light = std::make_shared<DirectionalLight>();
    //SEngine.SpawnGameObject(direct_light);

    ModelImporter::PrintSupportedFormats();

    /*auto cameraObj = std::make_shared<SCamera>();
    SEngine.SpawnGameObject(cameraObj);
    SEngine.GetGraphics().SetCurrentCamera(cameraObj->GetCamera());*/

    auto player = std::make_shared<FirstPersonPlayer>();
    SEngine.SpawnGameObject(player);

    //ModelImporter::PrintSupportedFormats();

    while (SEngine.IsRunning())
    {
        if (Keyboard::Instance().IsKeyPressed('X'))
            point_light->GetTransform().lock()->SetPosition(player->GetTransform().lock()->GetPosition());

        //direct_light->GetTransform().lock()->AddRotation({1, 1, 0}, 0.001);
        SEngine.Run();
    }

    return 0;
}
