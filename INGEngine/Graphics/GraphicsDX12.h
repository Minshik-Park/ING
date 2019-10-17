//----------------------------------------------------------------------------------
// GraphicsDX12.h : Definition of ING Graphics for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include "IGraphics.h"
#include "FrameDX12.h"
#include "StateDX12Sample.h"
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <memory>
#include <vector>
#include <array>
#include <map>

namespace ING {
namespace Graphics {

    class GraphicsDX12 : public IGraphics
    {
    public:
        GraphicsDX12(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT);
        virtual ~GraphicsDX12();

        // Public methods
        virtual result_code_t Initialize(window_t wnd) override;
        virtual result_code_t Initialize(core_window_t wnd) override;

        virtual result_code_t GetAdapterAt(const int index, IAdapter** ppAdapter) override;

        virtual void PauseRendering() override;
        virtual void ResumeRendering() override;

        virtual result_code_t Render() override;

        virtual result_code_t Present() override;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) override;

    private:
        result_code_t GenerateAdapterList();

        result_code_t HandleDeviceLost();

        result_code_t CreateDeviceIndependentResources();
        result_code_t CreateDeviceResources();
        result_code_t CreateWindowSizeDependentResources(const int width, const int height);

        void ReleaseDeviceIndependentResources();
        void ReleaseDeviceResources();
        void ReleaseWindowSizeDependentResources();

        void WaitForGPU();
        void MoveToNextFrame();

        std::shared_ptr<FrameDX12> CurrentFrameDX12()
        {
            return std::dynamic_pointer_cast<FrameDX12>(m_frames[m_currentFrame]);
        }

    private:
        // Configurations
        bool m_fRendering = false;

        // Core Window for store apps
        Microsoft::WRL::ComPtr<IUnknown>                        m_spCoreWindow;

        // DXGI Resources
        Microsoft::WRL::ComPtr<IDXGIFactory4>                   m_spDXGIFactory;
        Microsoft::WRL::ComPtr<IDXGISwapChain3>                 m_spSwapChain;

        // D3D12 Resources
        Microsoft::WRL::ComPtr<ID3D12Device>                    m_spD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue>              m_spCommandQueue;

        // HW resources
        std::vector<std::shared_ptr<IAdapter>>                  m_adapters;

        // Frame resources
        unsigned int m_currentFrame = 0;
        std::array<std::shared_ptr<IFrame>, c_frameCount>       m_frames;

        // Fence
        Microsoft::WRL::ComPtr<ID3D12Fence>                     m_spFence;
        HANDLE                                                  m_hFenceEvent = NULL;

        // State map
        std::map<std::wstring, std::shared_ptr<StateDX12Base>>  m_stateMap;

        // Settings
        DXGI_FORMAT                                             m_backBufferFormat = DXGI_FORMAT_UNKNOWN;
        DXGI_FORMAT                                             m_depthBufferFormat = DXGI_FORMAT_UNKNOWN;
    };

}}