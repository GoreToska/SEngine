//
// Created by gorev on 13.08.2025.
//

#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <d3d11.h>
#include <Wrl/client.h>

#define SDeviceContext GraphicsDevice::GetInstance().GetContext()
#define SDeviceContextAddress GraphicsDevice::GetInstance().GetContextAddress()
#define SDevice GraphicsDevice::GetInstance().GetDevice()
#define SDeviceAddress GraphicsDevice::GetInstance().GetDeviceAddress()

class GraphicsDevice
{
public:
    static GraphicsDevice& GetInstance();

    ID3D11Device* GetDevice() const;

    ID3D11Device** GetDeviceAddress();

    ID3D11DeviceContext* GetContext() const;

    ID3D11DeviceContext** GetContextAddress();

private:
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};


#endif //GRAPHICSDEVICE_H
