//----------------------------------------------------------------------------------
// GraphicsDX12.h : Definition of ING Graphics for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include <IGraphics.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <memory>
#include <vector>

namespace ING {
namespace Graphics {

    class GraphicsDX12 : public IGraphics
    {
    public:
        GraphicsDX12();
        virtual ~GraphicsDX12();

        virtual result_code_t Initialize() override;
        virtual result_code_t GetAdapterAt(const int index, Adapter** ppAdapter) override;

    private:
        result_code_t GenerateAdapterList();

    private:
        Microsoft::WRL::ComPtr<IDXGIFactory4>       m_spDXGIFactory;
        Microsoft::WRL::ComPtr<ID3D12Device>        m_spD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue>  m_spCommandQueue;

        std::vector<std::shared_ptr<Adapter>>    m_adapters;
    };

}}