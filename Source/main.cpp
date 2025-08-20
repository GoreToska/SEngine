//
// Created by gorev on 13.08.2025.
//

#include "Component/CameraComponent.h"
#include "../Include/GameObjects/GameObject.h"
#include "Component/IRenderComponent.h"
#include "Engine/Engine.h"
#include "Engine/ModelImporter.h"
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
    go->AddComponent<MeshRender>(go, (modelPath / "NanoSuit/nanosuit.obj").string());
    SEngine.SpawnGameObject(go);

    /*auto cameraObj = std::make_shared<SCamera>();
    SEngine.SpawnGameObject(cameraObj);
    SEngine.GetGraphics().SetCurrentCamera(cameraObj->GetCamera());*/

    auto player = std::make_shared<FirstPersonPlayer>();
    SEngine.SpawnGameObject(player);

    //ModelImporter::PrintSupportedFormats();

    while (SEngine.IsRunning())
    {
        SEngine.Run();
    }

    return 0;
}
