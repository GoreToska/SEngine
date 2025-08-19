//
// Created by gorev on 30.06.2025.
//

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <d3d11.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <Wrl/client.h>

#include "ShaderTypes.h"

struct ShaderDescription
{
    std::string name;
    std::string path;

    bool operator<(const ShaderDescription& other) const
    {
        return name < other.name;
    }
};

class ShaderManager
{
    enum class ShaderType
    {
        Vertex,
        Pixel,
        Geometry,
        Compute,
        Unknown
    };

public:
    ShaderManager(const std::string& path);

    ~ShaderManager();

    void CompileShaders();

    template<typename T>
    std::shared_ptr<T> CompileShader(const std::string name)
    {
        if constexpr (std::is_same_v<T, VertexShader>)
        {
            if (!v_shaders.empty() && v_shaders.contains(name))
                return v_shaders[name];
            return CompileVertexShader(name);
        }

        if constexpr (std::is_same_v<T, PixelShader>)
        {
            if (!p_shaders.empty() && p_shaders.contains(name))
                return p_shaders[name];
            return CompilePixelShader(name);
        }

        if constexpr (std::is_same_v<T, GeometryShader>)
        {
            if (!g_shaders.empty() && g_shaders.contains(name))
                return g_shaders[name];
            return CompileGeometryShader(name);
        }

        if constexpr (std::is_same_v<T, ComputeShader>)
        {
            if (!c_shaders.empty() && c_shaders.contains(name))
                return c_shaders[name];

            return CompileComputeShader(name);
        }
    }

    void SetShader();

    template<typename T>
    std::shared_ptr<T> GetShader(const std::string name)
    {
        if constexpr (std::is_same_v<T, VertexShader>)
        {
            if (v_shaders.contains(name))
                return v_shaders[name];
        }

        if constexpr (std::is_same_v<T, PixelShader>)
        {
            if (p_shaders.contains(name))
                return p_shaders[name];
        }

        if constexpr (std::is_same_v<T, GeometryShader>)
        {
            if (g_shaders.contains(name))
                return g_shaders[name];
        }

        if constexpr (std::is_same_v<T, ComputeShader>)
        {
            if (c_shaders.contains(name))
                return c_shaders[name];
        }

        return nullptr;
    }

    static std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc();

    Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout(const void* pShaderBytecode, const size_t pShaderSize);

private:
    std::map<std::string, std::shared_ptr<VertexShader>> v_shaders;
    std::map<std::string, std::shared_ptr<PixelShader>> p_shaders;
    std::map<std::string, std::shared_ptr<GeometryShader>> g_shaders;
    std::map<std::string, std::shared_ptr<ComputeShader>> c_shaders;

    std::string path;

    std::shared_ptr<VertexShader> CompileVertexShader(const std::string name);

    std::shared_ptr<PixelShader> CompilePixelShader(const std::string name);

    std::shared_ptr<GeometryShader> CompileGeometryShader(const std::string name);

    std::shared_ptr<ComputeShader> CompileComputeShader(const std::string name);

    void SaveShader(const std::string& name, Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob);

    bool TryToLoadShader(const std::string& name);

    ShaderType GetShaderType(Microsoft::WRL::ComPtr<ID3D10Blob> blob);
};


#endif //SHADERMANAGER_H
