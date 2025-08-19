//
// Created by gorev on 15.08.2025.
//

#include "Render/ModelBuffer.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::IndexBuffer(const IndexBuffer& rhs)
{
    this->buffer = rhs.buffer;
    this->bufferSize = rhs.bufferSize;
}

ID3D11Buffer* IndexBuffer::Get() const
{
    return buffer.Get();
}

ID3D11Buffer* const* IndexBuffer::GetAddressOf() const
{
    return buffer.GetAddressOf();
}

UINT IndexBuffer::BufferSize() const
{
    return bufferSize;
}

HRESULT IndexBuffer::Initialize(DWORD* data, UINT numIndices)
{
    if (buffer.Get() != nullptr)
    {
        buffer.Reset();
    }

    bufferSize = numIndices;

    CD3D11_BUFFER_DESC indexBufferDesc{};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * numIndices;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA indexBufferData{};
    indexBufferData.pSysMem = data;

    HRESULT hr = SDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, buffer.GetAddressOf());

    return hr;
}
