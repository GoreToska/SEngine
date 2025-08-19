//
// Created by gorev on 15.08.2025.
//

#ifndef CONSTBUFFER_H
#define CONSTBUFFER_H

#include<d3d11.h>
#include <wrl/client.h>

#include "GraphicsDevice.h"
#include "Utilities/Logger.h"


template<class T>
class ConstBuffer
{
    ConstBuffer(const ConstBuffer<T>& rhs);

public:
    ConstBuffer()
    {
    }

    void SetData(T data)
    {
        this->data = data;
    }

    T* GetData()
    {
        return &data;
    }

    ID3D11Buffer* Get() const
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return buffer.GetAddressOf();
    }

    HRESULT Initialize()
    {
        if (buffer.Get() != nullptr)
        {
            buffer.Reset();
        }

        D3D11_BUFFER_DESC constBufferDesc{};
        constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constBufferDesc.MiscFlags = 0;
        // 16 byte alignment
        constBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16));
        constBufferDesc.StructureByteStride = 0;

        HRESULT hr = SDevice->CreateBuffer(&constBufferDesc, 0, buffer.GetAddressOf());

        return hr;
    }

    bool ApplyChanges()
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        HRESULT hr = SDeviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

        if (FAILED(hr))
        {
            SERROR(hr, "Failed to create constant buffer.");
            return false;
        }

        CopyMemory(mappedSubresource.pData, &data, sizeof(T));
        SDeviceContext->Unmap(buffer.Get(), 0);

        return true;
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    T data;
};
#endif //CONSTBUFFER_H
