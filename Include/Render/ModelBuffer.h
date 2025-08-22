//
// Created by gorev on 15.08.2025.
//

#ifndef MODELBUFFER_H
#define MODELBUFFER_H
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>

#include "GraphicsDevice.h"

class IndexBuffer
{
public:
    IndexBuffer();

    IndexBuffer(const IndexBuffer& rhs);

    ID3D11Buffer* Get() const;

    ID3D11Buffer* const* GetAddressOf() const;

    UINT BufferSize() const;

    HRESULT Initialize(DWORD* data, UINT numIndices);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    UINT bufferSize = 0;
};

template<class T>
class VertexBuffer
{
public:
    VertexBuffer()
    {
    }

    VertexBuffer(const VertexBuffer& other)
    {
        this->buffer = other.buffer;
        this->stride = other.stride;
        this->bufferSize = other.bufferSize;
    }

    VertexBuffer(VertexBuffer&& other) noexcept
        : buffer(std::move(other.buffer)),
          stride(std::move(other.stride)),
          bufferSize(std::move(other.bufferSize))
    {
        other.buffer = nullptr;
        other.stride = 0;
        other.bufferSize = 0;
    }

    VertexBuffer& operator =(const VertexBuffer& other)
    {
        if (this == &other)
            return *this;

        this->buffer = other.buffer;
        this->stride = other.stride;
        this->bufferSize = other.bufferSize;
        
        return *this;
    }

    VertexBuffer& operator =(VertexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        this->buffer = std::move(other.buffer);
        this->stride = std::move(other.stride);
        this->bufferSize = std::move(other.bufferSize);
        other.buffer = nullptr;
        other.stride = 0;
        other.bufferSize = 0;
        return *this;
    }

    ID3D11Buffer* Get() const
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return buffer.GetAddressOf();
    }

    UINT BufferSize() const
    {
        return bufferSize;
    }

    UINT StridePtr() const
    {
        return stride;
    }

    HRESULT Initialize(T* data, UINT numVertices)
    {
        if (buffer.Get() != nullptr)
        {
            buffer.Reset();
        }

        bufferSize = numVertices;
        stride = sizeof(T);

        CD3D11_BUFFER_DESC vertexBufferDesc{};
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData{};
        vertexBufferData.pSysMem = data;

        HRESULT hr = SDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());

        return hr;
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    UINT stride;
    UINT bufferSize = 0;
};


#endif //MODELBUFFER_H
