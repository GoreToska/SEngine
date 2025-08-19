//
// Created by gorev on 30.06.2025.
//

#include "Render/ShaderManager.h"

#include <d3dcompiler.h>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "Render/GraphicsDevice.h"
#include "Utilities/Asserter.h"
#include "Utilities/FileSystemUtilities.h"
#include "Utilities/Logger.h"

ShaderManager::ShaderManager(const std::string& path) : path(path)
{
    // TODO: search for compiled shaders
    v_shaders = {};
    p_shaders = {};
    g_shaders = {};
    CompileShaders();
}

ShaderManager::~ShaderManager()
{
    v_shaders.clear();
    p_shaders.clear();
    g_shaders.clear();
    c_shaders.clear();
}

void ShaderManager::CompileShaders()
{
    std::filesystem::path path(std::filesystem::current_path().parent_path() / "Shaders");
    if (exists(path) && is_directory(path))
    {
        for (const auto& entry: std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                if (DirectoryContains(entry.path().stem().string(),
                                      std::filesystem::current_path() / "Shaders"))
                {
                    TryToLoadShader(entry.path().stem().string());
                }
                else
                {
                    if (entry.path().stem().string().find("_VS") != std::string::npos)
                    {
                        CompileShader<VertexShader>(entry.path().stem().string());
                    }
                    if (entry.path().stem().string().find("_PS") != std::string::npos)
                    {
                        CompileShader<PixelShader>(entry.path().stem().string());
                    }
                    if (entry.path().stem().string().find("_GS") != std::string::npos)
                    {
                        CompileShader<GeometryShader>(entry.path().stem().string());
                    }
                    if (entry.path().stem().string().find("_CS") != std::string::npos)
                    {
                        CompileShader<ComputeShader>(entry.path().stem().string());
                    }
                }
            }
        }
    }
}

void ShaderManager::SetShader()
{
}

std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderManager::GetInputLayoutDesc()
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
    layout.reserve(3);

    layout.emplace_back("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
    layout.emplace_back("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                        D3D11_INPUT_PER_VERTEX_DATA, 0);
    layout.emplace_back("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                        D3D11_INPUT_PER_VERTEX_DATA, 0);

    return layout;
}

Microsoft::WRL::ComPtr<ID3D11InputLayout> ShaderManager::GetInputLayout(const void* pShaderBytecode,
                                                                        const size_t pShaderSize)
{
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderReflection;
    ThrowIfFailed(
        D3DReflect(
            pShaderBytecode,
            pShaderSize,
            IID_ID3D11ShaderReflection,
            reinterpret_cast<void**>(shaderReflection.GetAddressOf())),
        "Failed to create shader reflection.");

    D3D11_SHADER_DESC shaderDesc;
    ThrowIfFailed(shaderReflection->GetDesc(&shaderDesc), "Failed to get shader description.");
    std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

    for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        shaderReflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc = {};
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;

        switch (paramDesc.Mask)
        {
            case 1: // X
                elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                break;
            case 3: // XY
                elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                break;
            case 7: // XYZ
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                break;
            case 15: // XYZW
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                break;
        }

        layout.push_back(elementDesc);
    }

    ThrowIfFailed(
        SDevice->CreateInputLayout(
            layout.data(),
            static_cast<UINT>(layout.size()),
            pShaderBytecode,
            pShaderSize,
            &inputLayout),
        "Failed to create input layout.");

    return inputLayout;
}


void ShaderManager::SaveShader(const std::string& name, Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob)
{
    std::filesystem::path shadersPath =
            std::filesystem::current_path() / "Shaders" / (name + ".cso");
    std::ofstream outputFile(shadersPath, std::ios::binary);
    outputFile.write(static_cast<const char*>(shaderBlob->GetBufferPointer()), shaderBlob->GetBufferSize());
    outputFile.close();
}

bool ShaderManager::TryToLoadShader(const std::string& name)
{
    Microsoft::WRL::ComPtr<ID3DBlob> shader_bc;
    Microsoft::WRL::ComPtr<ID3DBlob> errorCode;

    std::filesystem::path shadersPath =
            std::filesystem::current_path() / "Shaders" / (name + ".cso");
    if (!exists(shadersPath))
        return false;

    ThrowIfFailed(D3DReadFileToBlob(shadersPath.c_str(), &shader_bc),
                  "Failed to load shader");

    if (name.find("_VS") != std::string::npos)
    {
        Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
        auto resultShader = std::make_shared<VertexShader>();
        auto inputLayoutDesc = GetInputLayoutDesc();

        ThrowIfFailed(
            SDevice->CreateVertexShader(
                shader_bc->GetBufferPointer(),
                shader_bc->GetBufferSize(),
                nullptr,
                &shader), "Failed to create vertex shader.");

        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout =
                GetInputLayout(shader_bc->GetBufferPointer(), shader_bc->GetBufferSize());

        resultShader->Initialize(shader, shader_bc, inputLayout);
        v_shaders[name] = resultShader;
        return true;
    }
    if (name.find("_PS") != std::string::npos)
    {
        Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
        auto resultShader = std::make_shared<PixelShader>();

        ThrowIfFailed(
            SDevice->CreatePixelShader(
                shader_bc->GetBufferPointer(),
                shader_bc->GetBufferSize(),
                nullptr,
                &shader), "Failed to create vertex shader.");

        resultShader->Initialize(shader, shader_bc);
        p_shaders[name] = resultShader;
        return true;
    }
    if (name.find("_GS") != std::string::npos)
    {
        SERROR("Not implemented");
        return true;
    }
    if (name.find("_CS") != std::string::npos)
    {
        SERROR("Not implemented");
        return true;
    }

    return false;
}

std::shared_ptr<VertexShader> ShaderManager::CompileVertexShader(const std::string name)
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexBC;
    Microsoft::WRL::ComPtr<ID3DBlob> errorVertexCode;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
    auto resultShader = std::make_shared<VertexShader>();
    auto inputLayoutDesc = GetInputLayoutDesc();

    std::filesystem::path shadersPath =
            std::filesystem::current_path().parent_path() / "Shaders" / (name + ".hlsl");
    std::cout << "Current shaders compilation directory: " << shadersPath << std::endl;

    if (!std::filesystem::exists(shadersPath))
    {
        SERROR("Shader file does not exist: " + shadersPath.string());
        return nullptr;
    }

    SSHADERERROR(
        D3DCompileFromFile(shadersPath.c_str(),
            nullptr,
            nullptr,
            "main",
            "vs_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            &vertexBC,
            &errorVertexCode),
        errorVertexCode);

    ThrowIfFailed(
        SDevice->CreateVertexShader(
            vertexBC->GetBufferPointer(),
            vertexBC->GetBufferSize(),
            nullptr,
            &shader), "Failed to create vertex shader.");

    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout =
            GetInputLayout(vertexBC->GetBufferPointer(), vertexBC->GetBufferSize());

    resultShader->Initialize(shader, vertexBC, inputLayout);
    v_shaders[name] = resultShader;
    SaveShader(name, vertexBC);
    return resultShader;
}

std::shared_ptr<PixelShader> ShaderManager::CompilePixelShader(const std::string name)
{
    Microsoft::WRL::ComPtr<ID3DBlob> pixelBC;
    Microsoft::WRL::ComPtr<ID3DBlob> errorPixelCode;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
    auto resultShader = std::make_shared<PixelShader>();

    std::filesystem::path shadersPath =
            std::filesystem::current_path().parent_path() / "Shaders" / (name + ".hlsl");
    std::cout << "Current shaders compilation directory: " << shadersPath << std::endl;

    if (!std::filesystem::exists(shadersPath))
    {
        SERROR("Shader file does not exist: " + shadersPath.string());
        return nullptr;
    }

    SSHADERERROR(
        D3DCompileFromFile(shadersPath.c_str(),
            nullptr,
            nullptr,
            "main",
            "ps_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            &pixelBC,
            &errorPixelCode),
        errorPixelCode);

    ThrowIfFailed(
        SDevice->CreatePixelShader(
            pixelBC->GetBufferPointer(),
            pixelBC->GetBufferSize(),
            nullptr,
            &shader),
        "Failed to create pixel shader.");

    resultShader->Initialize(shader, pixelBC);
    p_shaders[name] = resultShader;
    SaveShader(name, pixelBC);
    return resultShader;
}

std::shared_ptr<GeometryShader> ShaderManager::CompileGeometryShader(const std::string name)
{
    SERROR("Not implemented");
    return nullptr;
}

std::shared_ptr<ComputeShader> ShaderManager::CompileComputeShader(const std::string name)
{
    SERROR("Not implemented");
    return nullptr;
}
