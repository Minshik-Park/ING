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
#include <array>

namespace ING {
namespace Graphics {

    class GraphicsDX12 : public IGraphics
    {
    public:
        GraphicsDX12();
        virtual ~GraphicsDX12();

        // Public methods
        virtual result_code_t Initialize(window_t wnd) override;
        virtual result_code_t Initialize(core_window_t wnd) override;

        virtual result_code_t GetAdapterAt(const int index, IAdapter** ppAdapter) override;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) override;

    private:
        result_code_t GenerateAdapterList();

        result_code_t CreateDeviceIndependentResources();
        result_code_t CreateDeviceResources();
        result_code_t CreateWindowSizeDependentResources(const int width, const int height);

        void ReleaseDeviceIndependentResources();
        void ReleaseDeviceResources();
        void ReleaseWindowSizeDependentResources();


    private:
        // Core Window for store apps
        Microsoft::WRL::ComPtr<IUnknown>                    m_spCoreWindow;

        // DXGI Resources
        Microsoft::WRL::ComPtr<IDXGIFactory4>               m_spDXGIFactory;
        Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_spSwapChain;

        // D3D12 Resources
        Microsoft::WRL::ComPtr<ID3D12Device>                m_spD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_spCommandQueue;

        // HW resources
        std::vector<std::shared_ptr<IAdapter>>              m_adapters;

        // Frame resources
        std::array<std::shared_ptr<IFrame>, c_frameCount>   m_frames;
    };

}}