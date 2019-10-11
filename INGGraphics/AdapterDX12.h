//----------------------------------------------------------------------------------
// AdapterDX12.h : Definition the ING Adapter for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include <IAdapter.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <string>

namespace ING {
namespace Graphics {

    class AdapterDX12 : public IAdapter
    {
    public:
        AdapterDX12(IDXGIAdapter1 *pAdapter);
        virtual ~AdapterDX12();

        virtual const wchar_t* Description() override;
        virtual bool IsHardware() override;

        IDXGIAdapter1* DX12Adapter() { return m_spAdapter.Get(); }

    private:
        Microsoft::WRL::ComPtr<IDXGIAdapter1> m_spAdapter;
        DXGI_ADAPTER_DESC1 m_descriptor = {};
    };

}}
