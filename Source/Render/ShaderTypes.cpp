//
// Created by gorev on 14.08.2025.
//

#include "Render/ShaderTypes.h"


void VertexShader::Initialize(const Microsoft::WRL::ComPtr<ID3D11VertexShader>& shader,
                              const Microsoft::WRL::ComPtr<ID3DBlob>& shaderBuffer,
                              const Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout)
{
    this->shader = shader;
    this->shaderBuffer = shaderBuffer;
    this->inputLayout = inputLayout;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader::GetShader()
{
    return shader;
}

Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetBuffer()
{
    return shaderBuffer;
}

Microsoft::WRL::ComPtr<ID3D11InputLayout> VertexShader::GetInputLayout()
{
    return inputLayout;
}

void PixelShader::Initialize(const Microsoft::WRL::ComPtr<ID3D11PixelShader>& shader,
                             const Microsoft::WRL::ComPtr<ID3DBlob>& shaderBuffer)
{
    this->shader = shader;
    this->shaderBuffer = shaderBuffer;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader::GetShader()
{
    return shader;
}

Microsoft::WRL::ComPtr<ID3DBlob> PixelShader::GetBuffer()
{
    return shaderBuffer;
}
