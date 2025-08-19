//
// Created by gorev on 13.08.2025.
//

#ifndef ADAPTERREADER_H
#define ADAPTERREADER_H

#include<dxgi.h>
#include <vector>
#include <Wrl/client.h>

#include "Logger.h"

class AdapterData
{
public:
    AdapterData(IDXGIAdapter* pAdapter)
    {
        this->pAdapter = pAdapter;
        HRESULT hr = pAdapter->GetDesc(&description);

        if (FAILED(hr))
        {
            SERROR(hr, "Failed to get description for IDXGIAdapter.");
        }
    }

    IDXGIAdapter& GetAdapter() const
    {
        return *pAdapter.Get();
    }

private:
    Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
    DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
    inline static std::vector<AdapterData>& GetAdapters()
    {
        if (!adapters.empty())
            return adapters;

        Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));

        if (FAILED(hr))
        {
            SERROR(hr, "Failed to create IDXGIFactory for enumerating adapters.");
            exit(-1);
        }

        IDXGIAdapter* pAdapter;
        UINT index = 0;

        while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
        {
            adapters.push_back(AdapterData(pAdapter));
            index += 1;
        }

        return adapters;
    }

private:
    inline static std::vector<AdapterData> adapters;
};

#endif //ADAPTERREADER_H
