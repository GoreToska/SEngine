//
// Created by gorev on 17.08.2025.
//

#ifndef GBUFFER_H
#define GBUFFER_H
#include <wrl/client.h>
#include <d3d11.h>

class GBuffer
{
public:
    GBuffer();

    void SetRenderTargets() const;

    void PSBindResourceViews(int startIndex) const;

    void PSClearResourceViews(int startIndex) const;

    void ClearRenderTargets() const;

private:
    // TODO: depth and specular can be combined in one texture
    // or depth can be coded in a channel of normals and specular can be coded in a channel of diffuse
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depth;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> normal;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> diffuse;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> specular;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthSRV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalSRV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseSRV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularSRV;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> depthRTV;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> normalRTV;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> diffuseRTV;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> specularRTV;
};


#endif //GBUFFER_H
