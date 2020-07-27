//----------------------------------------------------------------------------------
// FrameDX12.cpp : Implementation the ING Graphics frame for DX12 class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "FrameDX12.h"
#include <Exception.h>

using namespace ING;
using namespace ING::Graphics;
using namespace Microsoft::WRL;

///
/// Default Constructor.
///
FrameDX12::FrameDX12(const int index, ID3D12Device* pD3DDevice) :
    IFrame(index),
    m_spD3DDevice(pD3DDevice)
{
    TraceScopeVoid();
}

///
/// Default Destrcutor.
///
FrameDX12::~FrameDX12()
{
    TraceScopeVoid();
}

///
///
///
result_code_t FrameDX12::Initialize()
{
    HRESULT hr = S_OK;
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};

    // Create descriptor heaps for render target views.
    rtvHeapDesc.NumDescriptors = 1;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    GOTO_IF_HR_FAILED(m_spD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_spRTVHeap)), Cleanup);

    // Create descriptor heaps for depth stencil views.
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    GOTO_IF_HR_FAILED(m_spD3DDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_spDSVHeap)), Cleanup);

    // Create Command allocator
    GOTO_IF_HR_FAILED(m_spD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_spCommandAllocator.ReleaseAndGetAddressOf())), Cleanup);

    // Create a command list.
    GOTO_IF_HR_FAILED(m_spD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_spCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_spCommandList)), Cleanup);

    // Command List initial state is opened. Close it and Reset before using it.
    GOTO_IF_HR_FAILED(m_spCommandList->Close(), Cleanup);

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

///
///
///
result_code_t FrameDX12::Render()
{
    HRESULT hr = S_OK;
    
    GOTO_IF_HR_FAILED(m_spCommandAllocator->Reset(), Cleanup);
    GOTO_IF_HR_FAILED(m_spCommandList->Reset(m_spCommandAllocator.Get(), nullptr), Cleanup);

    // Clear the render target.
    {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_spBackBuffer.Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        m_spCommandList->ResourceBarrier(1, &barrier);

        FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };

        m_spCommandList->ClearRenderTargetView(GetRenderTargetView(), clearColor, 0, nullptr);

        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        m_spCommandList->ResourceBarrier(1, &barrier);
    }

    GOTO_IF_HR_FAILED(m_spCommandList->Close(), Cleanup);

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}


///
///
///
result_code_t FrameDX12::OnSizeChanged(const int width, const int height)
{
    HRESULT hr = S_OK;

    D3D12_HEAP_PROPERTIES depthHeapProperties = {};
    D3D12_RESOURCE_DESC depthResourceDesc = {};
    D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

    // Create Depth Stensil buffer.
    depthHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    depthHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    depthHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    depthHeapProperties.CreationNodeMask = 1;
    depthHeapProperties.VisibleNodeMask = 1;

    depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResourceDesc.Alignment = 0;
    depthResourceDesc.Width = width;
    depthResourceDesc.Height = height;
    depthResourceDesc.DepthOrArraySize = 1;
    depthResourceDesc.MipLevels = 1;
    depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthResourceDesc.SampleDesc.Count = 1;
    depthResourceDesc.SampleDesc.Quality = 0;
    depthResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    GOTO_IF_HR_FAILED(m_spD3DDevice->CreateCommittedResource(
        &depthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthResourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        IID_PPV_ARGS(m_spDepthStencilBuffer.ReleaseAndGetAddressOf())
    ), Cleanup);

    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

    // Create Depth Stensil view.
    m_spD3DDevice->CreateDepthStencilView(m_spDepthStencilBuffer.Get(), &dsvDesc, GetDepthStencilView());

    // Set the 3D rendering viewport to target the entire window.
    m_screenViewport = { 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f };

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

///
///
///
void FrameDX12::ReleaseSizeDependentResources()
{
    m_spBackBuffer.Reset();
    m_backBufferFormat = DXGI_FORMAT_UNKNOWN;
}

///
///
///
result_code_t FrameDX12::SetBackBuffer(ID3D12Resource* pBackBuffer, DXGI_FORMAT format)
{
    m_spBackBuffer = pBackBuffer;
    m_backBufferFormat = format;

    // Create Render Target View
    m_spD3DDevice->CreateRenderTargetView(m_spBackBuffer.Get(), nullptr, GetRenderTargetView());

    return result_code_t::succeeded;
}

