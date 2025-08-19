//
// Created by gorev on 13.08.2025.
//

#include "Render/GraphicsDevice.h"

GraphicsDevice& GraphicsDevice::GetInstance()
{
    static GraphicsDevice instance;
    return instance;
}

ID3D11Device* GraphicsDevice::GetDevice() const
{
    return device.Get();
}

ID3D11Device** GraphicsDevice::GetDeviceAddress()
{
    return device.GetAddressOf();
}

ID3D11DeviceContext* GraphicsDevice::GetContext() const
{
    return deviceContext.Get();
}

ID3D11DeviceContext** GraphicsDevice::GetContextAddress()
{
    return deviceContext.GetAddressOf();
}
