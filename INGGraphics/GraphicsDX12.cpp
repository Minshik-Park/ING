//----------------------------------------------------------------------------------
// GraphicsDX12.cpp : Implementation of ING Graphics for DX12 class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include "GraphicsDX12.h"
#include "AdapterDX12.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

using namespace ING;
using namespace ING::Graphics;
using namespace Microsoft::WRL;

GraphicsDX12::GraphicsDX12()
{
}

GraphicsDX12::~GraphicsDX12()
{
}

result_code_t GraphicsDX12::Initialize()
{
    HRESULT hr = S_OK;

#ifdef _DEBUG
    // If the project is in a debug build, enable debugging via SDK Layers.
    {
        ComPtr<ID3D12Debug> spDebugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&spDebugController))))
        {
            spDebugController->EnableDebugLayer();
        }
    }
#endif

    // Create DXGI Factory
    GOTO_IF_HR_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_spDXGIFactory)), Cleanup);

    // Generate Adapter List
    RETURN_IF_FAILED(GenerateAdapterList());

    //
    // Create DX12 Device
    //
    {
        AdapterDX12* pHwAdapter = nullptr;

        // Select HW adapter
        for (auto adapter : m_adapters)
        {
            if (adapter->IsHardware())
            {
                pHwAdapter = dynamic_cast<AdapterDX12*>(adapter.get());
                break;
            }
        }
        RETURN_IF_FALSE(pHwAdapter, result_code_t::result_not_found);

        // Create the Direct3D 12 API device object
        hr = D3D12CreateDevice(
            pHwAdapter->DX12Adapter(),      // The hardware adapter.
            D3D_FEATURE_LEVEL_11_0,         // Minimum feature level this app can support.
            IID_PPV_ARGS(&m_spD3DDevice));  // Returns the Direct3D device created.

#if defined(_DEBUG)
        if (FAILED(hr))
        {
            // If the initialization fails, fall back to the WARP device.
            // For more information on WARP, see: 
            // https://go.microsoft.com/fwlink/?LinkId=286690

            ComPtr<IDXGIAdapter> spWarpAdapter;

            ING_DebugWrite(L"Failed to create D3D12 device with hardware adapter. Trying with WARP adapter.\n");
            GOTO_IF_HR_FAILED(m_spDXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&spWarpAdapter)), Cleanup);

            hr = D3D12CreateDevice(spWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_spD3DDevice));
        }
#endif
        GOTO_IF_HR_FAILED(hr, Cleanup);
    }

    //
    // Create the command queue.
    //
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        GOTO_IF_HR_FAILED(m_spD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_spCommandQueue)), Cleanup);
    }

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

result_code_t GraphicsDX12::GetAdapterAt(const int index, Adapter** ppAdapter)
{
    result_code_t result = result_code_t::result_succeeded;

    if (m_adapters.empty())
    {
        RETURN_IF_FAILED(GenerateAdapterList());
    }

    RETURN_IF_FALSE(index >= 0 && index < m_adapters.size(), result_code_t::result_out_of_bound);
    RETURN_IF_FALSE(ppAdapter, result_code_t::result_invalid_pointer);

    *ppAdapter = m_adapters[index].get();

    return result;
}

result_code_t GraphicsDX12::GenerateAdapterList()
{
    ComPtr<IDXGIAdapter1> spAdapter;

    RETURN_IF_FALSE(m_spDXGIFactory.Get(), result_code_t::result_not_initialized);

    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_spDXGIFactory->EnumAdapters1(adapterIndex, &spAdapter); adapterIndex++)
    {
        DXGI_ADAPTER_DESC1 desc;
        spAdapter->GetDesc1(&desc);

        std::shared_ptr<Adapter> adapter(new AdapterDX12(spAdapter.Get()));
        m_adapters.push_back(adapter);
    }

    return result_code_t::result_succeeded;
}
