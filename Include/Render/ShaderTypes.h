//
// Created by gorev on 14.08.2025.
//

#ifndef SHADERTYPES_H
#define SHADERTYPES_H

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

class VertexShader
{
public:
    void Initialize(const Microsoft::WRL::ComPtr<ID3D11VertexShader>& shader,
                    const Microsoft::WRL::ComPtr<ID3DBlob>& shaderBuffer,
                    const Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout);

    Microsoft::WRL::ComPtr<ID3D11VertexShader> GetShader();

    Microsoft::WRL::ComPtr<ID3DBlob> GetBuffer();

    Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout();

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader
{
public:
    void Initialize(const Microsoft::WRL::ComPtr<ID3D11PixelShader>& shader,
                    const Microsoft::WRL::ComPtr<ID3DBlob>& shaderBuffer);

    Microsoft::WRL::ComPtr<ID3D11PixelShader> GetShader();

    Microsoft::WRL::ComPtr<ID3DBlob> GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
};

class GeometryShader
{
public:
    bool Initialize(std::wstring shaderPath);

    ID3D11GeometryShader* GetShader();

    ID3DBlob* GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
};

class ComputeShader
{
public:
    bool Initialize(std::wstring shaderPath);

    ID3D11ComputeShader* GetShader();

    ID3DBlob* GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
};

#endif //SHADERTYPES_H
