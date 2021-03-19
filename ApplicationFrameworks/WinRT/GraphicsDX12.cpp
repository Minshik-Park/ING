#include "pch.h"
#include "GraphicsDX12.h"
#include "d3dx12.h"

using namespace winrt;
using namespace ING;

GraphicsDX12::GraphicsDX12()
{
}

GraphicsDX12::~GraphicsDX12()
{
}

ErrorCode GraphicsDX12::Initialize(const unsigned int width, const unsigned int height)
{
    ResultCode rc(__FUNCTION__);
    UINT dxgiFactoryFlags = 0;
    IFFALSE_EXIT(!m_spSwapChain, ErrorCode::EAlreadyInitialized);

#if _DEBUG
    com_ptr<ID3D12Debug> debugInterface;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
    {
        debugInterface->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
    else
    {
        Debug::DebugPrint("WARNING:  Unable to enable D3D12 debug validation layer\n");
    }
#endif

    // Obtain the DXGI factory
    IFFAILED_EXIT(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_spDxgiFactory.put())));

    // Find high performance hardware adapter
    com_ptr<IDXGIAdapter1> spHardwareAdapter;
    IFFAILED_EXIT(GetHardwareAdapter(spHardwareAdapter, true));

    // Create D3D12 Device
    IFFAILED_EXIT(D3D12CreateDevice(spHardwareAdapter.get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_spD3DDevice)));

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
    IFFAILED_EXIT(m_spD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_spCommandQueue)));

    // Create command allocator.
    IFFAILED_EXIT(m_spD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_spCommandAllocator.put())));

    // Create command list.
    IFFAILED_EXIT(m_spD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_spCommandAllocator.get(), nullptr, IID_PPV_ARGS(m_spCommandList.put())));

    // Command lists are created in the recording state, but there is nothing
    // to record yet. The main loop expects it to be closed, so close it now.
    IFFAILED_EXIT(m_spCommandList->Close());

    // Create synchronization objects.
    IFFAILED_EXIT(m_spD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_spFence)));
    m_fenceValue = 1;

    // Create an event handle to use for frame synchronization.
    m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    IFNULL_EXIT(m_hFenceEvent, HRESULT_FROM_WIN32(GetLastError()));

    ///////////////////////////////////////////////////////////////
    // Create window size dependent resources.
    ///////////////////////////////////////////////////////////////
    IFFAILED_EXIT(OnWindowSizeUpdated(width, height));

    return rc.Result();
}

ErrorCode GraphicsDX12::OnWindowSizeUpdated(const unsigned int width, const unsigned int height)
{
    ResultCode rc(__FUNCTION__);

    if (m_windowWidth != width || m_windowHeight != height)
    {
        com_ptr<IUnknown> spCoreWindow;
        winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread().as(spCoreWindow);

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = c_frameCount;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        // Resize Swap chain if exist, unless create new Swap chain.
        if (m_spSwapChain)
        {
            // Release all Render Target Views for each frame.
            for (UINT n = 0; n < c_frameCount; n++)
            {
                m_spRenderTargets[n] = nullptr;
            }
            m_spRtvHeap = nullptr;

            IFFAILED_EXIT(m_spSwapChain->ResizeBuffers(c_frameCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
        }
        else
        {
            com_ptr<IDXGISwapChain1> spSwapChain;

            IFFAILED_EXIT(m_spDxgiFactory->CreateSwapChainForCoreWindow(
                m_spCommandQueue.get(), // Swap chain needs the queue so that it can force a flush on it.
                spCoreWindow.get(),
                &swapChainDesc,
                nullptr,
                spSwapChain.put()));

            IFFALSE_EXIT(spSwapChain.try_as(m_spSwapChain), E_NOINTERFACE);
        }

        m_windowWidth = width;
        m_windowHeight = height;

        // Create Render Target
        {
            // Describe and create a render target view (RTV) descriptor heap.
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = c_frameCount;
            rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            IFFAILED_EXIT(m_spD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_spRtvHeap.put())));

            m_rtvDescriptorSize = m_spD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            // Create frame resources.
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_spRtvHeap->GetCPUDescriptorHandleForHeapStart());

            // Create a RTV for each frame.
            for (UINT n = 0; n < c_frameCount; n++)
            {
                IFFAILED_EXIT(m_spSwapChain->GetBuffer(n, IID_PPV_ARGS(m_spRenderTargets[n].put())));
                m_spD3DDevice->CreateRenderTargetView(m_spRenderTargets[n].get(), nullptr, rtvHandle);
                rtvHandle.Offset(1, m_rtvDescriptorSize);
            }
        }
    }

    return rc.Result();
}

ErrorCode GraphicsDX12::Destroy()
{
    ResultCode rc(__FUNCTION__);
    // Ensure that the GPU is no longer referencing resources that are about to be
    // cleaned up by the destructor.
    IFFAILED_EXIT(WaitForPreviousFrame());

    if (m_hFenceEvent)
    {
        CloseHandle(m_hFenceEvent);
        m_hFenceEvent = NULL;
    }

    return rc.Result();
}

ErrorCode GraphicsDX12::GetHardwareAdapter(
    com_ptr<IDXGIAdapter1> &spHardwareAdapter,
    bool requestHighPerformanceAdapter)
{
    ResultCode rc(__FUNCTION__, ErrorCode::ENotFound);
    com_ptr<IDXGIAdapter1> spAdapter;
    com_ptr<IDXGIFactory6> spFactory6;
    spHardwareAdapter = nullptr;

    if (SUCCEEDED(m_spDxgiFactory->QueryInterface(IID_PPV_ARGS(&spFactory6))))
    {
        DXGI_GPU_PREFERENCE gpuPreference = requestHighPerformanceAdapter ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED;

        for (UINT i = 0; DXGI_ERROR_NOT_FOUND != spFactory6->EnumAdapterByGpuPreference(i, gpuPreference, IID_PPV_ARGS(&spAdapter)); i++)
        {
            DXGI_ADAPTER_DESC1 desc;
            spAdapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, use WRAP device.
                Debug::DebugPrintf(L"%s Software adapter is skipped\n", desc.Description);
            }
            else if (SUCCEEDED(D3D12CreateDevice(spAdapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
            {
                // Check to see whether the adapter supports Direct3D 12, but don't create the actual device yet.
                Debug::DebugPrintf(L"%s adapter selected\n", desc.Description);
                spHardwareAdapter.attach(spAdapter.detach());
                rc = ErrorCode::Succeeded;
                break;
            }
        }
    }
    else
    {
        for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_spDxgiFactory->EnumAdapters1(i, spAdapter.put()); i++)
        {
            DXGI_ADAPTER_DESC1 desc;
            spAdapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, use WRAP device.
                Debug::DebugPrintf(L"%s Software adapter is skipped\n", desc.Description);
            }
            else if (SUCCEEDED(D3D12CreateDevice(spAdapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
            {
                // Check to see whether the adapter supports Direct3D 12, but don't create the actual device yet.
                Debug::DebugPrintf(L"%s adapter selected\n", desc.Description);
                spAdapter.attach(spAdapter.detach());
                rc = ErrorCode::Succeeded;
                break;
            }
        }
    }

    return rc.Result();
}

ErrorCode GraphicsDX12::PopulateCommandList()
{
    ResultCode rc(__FUNCTION__);

    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    IFFAILED_EXIT(m_spCommandAllocator->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    IFFAILED_EXIT(m_spCommandList->Reset(m_spCommandAllocator.get(), m_spPipelineState.get()));

    // Indicate that the back buffer will be used as a render target.
    CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_spRenderTargets[m_frameIndex].get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_spCommandList->ResourceBarrier(1, &transition);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_spRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_spCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Indicate that the back buffer will now be used to present.
    transition = CD3DX12_RESOURCE_BARRIER::Transition(m_spRenderTargets[m_frameIndex].get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_spCommandList->ResourceBarrier(1, &transition);

    IFFAILED_EXIT(m_spCommandList->Close());

    return rc.Result();
}

ErrorCode GraphicsDX12::WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to
    // maximize GPU utilization.

    // Signal and increment the fence value.
    ResultCode rc(__FUNCTION__);

    const UINT64 fence = m_fenceValue;
    IFFAILED_EXIT(m_spCommandQueue->Signal(m_spFence.get(), fence));
    m_fenceValue++;

    // Wait until the previous frame is finished.
    if (m_spFence->GetCompletedValue() < fence)
    {
        IFFAILED_EXIT(m_spFence->SetEventOnCompletion(fence, m_hFenceEvent));
        WaitForSingleObject(m_hFenceEvent, INFINITE);
    }

    m_frameIndex = m_spSwapChain->GetCurrentBackBufferIndex();

    return rc.Result();
}
