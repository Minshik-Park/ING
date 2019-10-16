//----------------------------------------------------------------------------------
// FrameDX12.h : Definition the ING Graphics frame for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "IFrame.h"

namespace ING {
namespace Graphics {

    class FrameDX12 : public IFrame
    {
    public:
        FrameDX12(const int index, ID3D12Device *pD3DDevice);
        virtual ~FrameDX12();

        virtual result_code_t Initialize() override;
        virtual result_code_t Render() override;

        virtual result_code_t OnSizeChanged(const int width, const int height) override;
        virtual void ReleaseSizeDependentResources() override;

        result_code_t SetBackBuffer(ID3D12Resource* pBackBuffer, DXGI_FORMAT format);

        uint64_t FenceValue() const { return m_fenceValue; }
        void SetFenceValue(const uint64_t value) { m_fenceValue = value; }
        void IncreaseFenceValue() { m_fenceValue++; }

        D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
        {
            return m_spRTVHeap->GetCPUDescriptorHandleForHeapStart();
        }
        D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
        {
            return m_spDSVHeap->GetCPUDescriptorHandleForHeapStart();
        }
        ID3D12GraphicsCommandList* GetCommandList() const
        {
            return m_spCommandList.Get();
        }

    private:
        Microsoft::WRL::ComPtr<ID3D12Device>                m_spD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_spCommandAllocator;   // Command Allocator
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_spCommandList;
        Microsoft::WRL::ComPtr<ID3D12Resource>              m_spBackBuffer;         // Back buffer (Render Target)
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_spRTVHeap;            // Render Target View Heap

        Microsoft::WRL::ComPtr<ID3D12Resource>              m_spDepthStencilBuffer; // Depth Stencil buffer
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_spDSVHeap;            // Depth Stensil View Heap

        DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_UNKNOWN;
        D3D12_VIEWPORT                                      m_screenViewport = {};

        uint64_t m_fenceValue = 0;
    };

}}
