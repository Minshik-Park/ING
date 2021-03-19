#pragma once
#include "Graphics.h"
#include <dxgi1_6.h>
#include <d3d12.h>

namespace ING
{
    class GraphicsDX12 : public Graphics
    {
    public:
        GraphicsDX12();
        virtual ~GraphicsDX12();

        /// <summary>
        /// Initialize DirectX 12 based rendering system
        /// </summary>
        /// <param name="width">Render target width in pixel</param>
        /// <param name="height">Render target height in pixel</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Initialize(const unsigned int width, const unsigned int height) override;

        /// <summary>
        /// Destroy and release all resources
        /// </summary>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Destroy() override;

        /// <summary>
        /// Update Window (Render target) size
        /// </summary>
        /// <param name="width">Render target width in pixel</param>
        /// <param name="height">Rnder target height in pixel</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode OnWindowSizeUpdated(const unsigned int width, const unsigned int height) override;

    private:
        ErrorCode GetHardwareAdapter(
            winrt::com_ptr<IDXGIAdapter1>& spAdapter,
            bool requestHighPerformanceAdapter);

        ErrorCode PopulateCommandList();

        ErrorCode WaitForPreviousFrame();

    private:
        winrt::com_ptr<IDXGIFactory4>               m_spDxgiFactory;
        winrt::com_ptr<IDXGISwapChain3>             m_spSwapChain;
        winrt::com_ptr<ID3D12Device>                m_spD3DDevice;
        winrt::com_ptr<ID3D12CommandQueue>          m_spCommandQueue;
        winrt::com_ptr<ID3D12DescriptorHeap>        m_spRtvHeap;
        UINT m_rtvDescriptorSize = 0;
        winrt::com_ptr<ID3D12Resource>              m_spRenderTargets[c_frameCount];
        winrt::com_ptr<ID3D12CommandAllocator>      m_spCommandAllocator;
        winrt::com_ptr<ID3D12GraphicsCommandList>   m_spCommandList;
        winrt::com_ptr<ID3D12PipelineState>         m_spPipelineState;
        winrt::com_ptr<ID3D12Fence>                 m_spFence;
        UINT64 m_fenceValue = 0;
        HANDLE m_hFenceEvent = NULL;
        UINT m_frameIndex = 0;
    };
}