cmake_minimum_required(VERSION 3.31)
project(SEngine)

include(FetchContent)
add_custom_target(shaders ALL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/build)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Shaders)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

find_package(assimp QUIET)
if (NOT assimp_FOUND)
    message(STATUS "Assimp not found. Fetching from Git.")
    FetchContent_Declare(
            assimp
            GIT_REPOSITORY https://github.com/assimp/assimp.git
            GIT_TAG v6.0.2
    )
    FetchContent_MakeAvailable(assimp)
else ()
    message(STATUS "Assimp found.")
endif ()

find_package(DirectXTK QUIET)
if (NOT DirectXTK_FOUND)
    message(STATUS "DirectXTK not found. Fetching from Git.")
    FetchContent_Declare(
            DirectXTK
            GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
            GIT_TAG mar2025
    )
    FetchContent_MakeAvailable(DirectXTK)
else ()
    message(STATUS "DirectXTK found.")
endif ()

add_library(${PROJECT_NAME}_lib STATIC
        Source/Render/RenderSubsystem.cpp
        Include/Render/RenderSubsystem.h
        Source/Engine/Engine.cpp
        Include/Engine/Engine.h
        Source/Render/ShaderManager.cpp
        Include/Render/ShaderManager.h
        Source/Render/RenderWindow.cpp
        Include/Render/RenderWindow.h
        Source/Render/WindowContainer.cpp
        Include/Render/WindowContainer.h
        Source/Render/Graphics.cpp
        Include/Render/Graphics.h
        Include/Utilities/Logger.h
        Include/Utilities/Delegate.h
        Source/Input/Keyboard.cpp
        Include/Input/Keyboard.h
        Include/Engine/EngineTypes.h
        Source/Render/DeferredRenderSubsystem.cpp
        Include/Render/DeferredRenderSubsystem.h
        Include/Utilities/AdapterReader.h
        Source/Render/GraphicsDevice.cpp
        Include/Render/GraphicsDevice.h
        Include/Utilities/Asserter.h
        Include/Utilities/StringUtilities.h
        Include/GameObjects/GameObject.h
        Source/Component/IComponent.cpp
        Include/Component/IComponent.h
        Source/Component/Transform.cpp
        Include/Component/Transform.h
        Source/Component/IRenderComponent.cpp
        Include/Component/IRenderComponent.h
        Include/Render/ConstantBufferTypes.h
        Source/Engine/Timer.cpp
        Include/Engine/Timer.h
        Source/Render/ShaderTypes.cpp
        Include/Render/ShaderTypes.h
        Source/Utilities/FileSystemUtilities.cpp
        Include/Utilities/FileSystemUtilities.h
        Include/Render/ConstBuffer.h
        Source/Component/MeshRender.cpp
        Include/Component/MeshRender.h
        Source/Engine/ModelImporter.cpp
        Include/Engine/ModelImporter.h
        Include/Render/Material.h
        Source/Render/ModelBuffer.cpp
        Include/Render/ModelBuffer.h
        Source/Render/GBuffer.cpp
        Include/Render/GBuffer.h
        Source/Component/CameraComponent.cpp
        Include/Component/CameraComponent.h
        Source/GameObjects/SCamera.cpp
        Include/GameObjects/SCamera.h
        Include/Utilities/SMath.h
        Source/GameObjects/FirstPersonPlayer.cpp
        Include/GameObjects/FirstPersonPlayer.h
        Include/Render/Texture.h
)

add_executable(${PROJECT_NAME}
        Source/Render/RenderSubsystem.cpp
        Include/Render/RenderSubsystem.h
        Source/Engine/Engine.cpp
        Include/Engine/Engine.h
        Source/Render/ShaderManager.cpp
        Include/Render/ShaderManager.h
        Source/main.cpp
        Source/Render/RenderWindow.cpp
        Include/Render/RenderWindow.h
        Source/Render/WindowContainer.cpp
        Include/Render/WindowContainer.h
        Source/Render/Graphics.cpp
        Include/Render/Graphics.h
        Include/Utilities/Logger.h
        Include/Utilities/Delegate.h
        Source/Input/Keyboard.cpp
        Include/Input/Keyboard.h
        Source/Input/Mouse.cpp
        Include/Input/Mouse.h
        Include/Engine/EngineTypes.h
        Source/Render/DeferredRenderSubsystem.cpp
        Include/Render/DeferredRenderSubsystem.h
        Include/Utilities/AdapterReader.h
        Source/Render/GraphicsDevice.cpp
        Include/Render/GraphicsDevice.h
        Include/Utilities/Asserter.h
        Include/Utilities/StringUtilities.h
        Include/GameObjects/GameObject.h
        Source/Component/IComponent.cpp
        Include/Component/IComponent.h
        Source/Component/Transform.cpp
        Include/Component/Transform.h
        Source/Component/IRenderComponent.cpp
        Include/Component/IRenderComponent.h
        Include/Render/ConstantBufferTypes.h
        Source/Engine/Timer.cpp
        Include/Engine/Timer.h
        Source/Render/ShaderTypes.cpp
        Include/Render/ShaderTypes.h
        Source/Utilities/FileSystemUtilities.cpp
        Include/Utilities/FileSystemUtilities.h
        Include/Render/ConstBuffer.h
        Source/Component/MeshRender.cpp
        Include/Component/MeshRender.h
        Source/Engine/ModelImporter.cpp
        Include/Engine/ModelImporter.h
        Include/Render/Material.h
        Source/Render/ModelBuffer.cpp
        Include/Render/ModelBuffer.h
        Source/Render/GBuffer.cpp
        Include/Render/GBuffer.h
        Source/Component/CameraComponent.cpp
        Include/Component/CameraComponent.h
        Source/GameObjects/SCamera.cpp
        Include/GameObjects/SCamera.h
        Include/Utilities/SMath.h
        Source/GameObjects/FirstPersonPlayer.cpp
        Include/GameObjects/FirstPersonPlayer.h
        Include/Render/Texture.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTK d3dcompiler dxguid dxgi d3d11 assimp)
target_link_libraries(${PROJECT_NAME}_lib PRIVATE DirectXTK d3dcompiler dxguid dxgi d3d11 assimp)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
        ${PROJECT_SOURCE_DIR}/Include
        ${DirectXTK_SOURCE_DIR}/Inc)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/Include
        ${DirectXTK_SOURCE_DIR}/Inc)

