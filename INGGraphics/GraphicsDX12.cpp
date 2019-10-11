//----------------------------------------------------------------------------------
// GraphicsDX12.cpp : Implementation of ING Graphics for DX12 class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include "GraphicsDX12.h"
#include "AdapterDX12.h"
#include <Exception.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

using namespace ING;
using namespace ING::Graphics;
using namespace Microsoft::WRL;

///
/// Default Constructor.
///
GraphicsDX12::GraphicsDX12(DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthBufferFormat) :
    m_backBufferFormat(backBufferFormat),
    m_depthBufferFormat(depthBufferFormat)
{
}

///
/// Default Destructor.
///
GraphicsDX12::~GraphicsDX12()
{
    ReleaseWindowSizeDependentResources();
    ReleaseDeviceResources();
    ReleaseDeviceIndependentResources();

    m_spCoreWindow.Reset();
}

///
/// Initializing Graphics for Win32 native application.
///
result_code_t GraphicsDX12::Initialize(window_t wnd)
{
    RECT rcClient;
    m_window = wnd;
    m_spCoreWindow.Reset();

    // Get Window client rect area.
    GetClientRect(m_window, &rcClient);
    m_windowWidth  = rcClient.right - rcClient.left;
    m_windowHeight = rcClient.bottom - rcClient.top;

    // Create any device independent resources.
    RETURN_IF_FAILED(CreateDeviceIndependentResources());

    // Create device resources.
    RETURN_IF_FAILED(CreateDeviceResources());

    // Create window size dependent resources.
    RETURN_IF_FAILED(CreateWindowSizeDependentResources(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top));

    return result_code_t::succeeded;
}

///
/// Initializing Graphics for Windows store application.
///
result_code_t GraphicsDX12::Initialize(core_window_t wnd)
{
    int width = 0;
    int height = 0;
    m_window = NULL;
    m_spCoreWindow = wnd;

    RETURN_IF_FAILED(CreateDeviceIndependentResources());

    RETURN_IF_FAILED(CreateDeviceResources());

    // ToDo: Calculate width and height from CoreWindow.
    RETURN_IF_FAILED(CreateWindowSizeDependentResources(width, height));

    return result_code_t::succeeded;
}

///
///
///
result_code_t GraphicsDX12::GetAdapterAt(const int index, IAdapter** ppAdapter)
{
    if (m_adapters.empty())
    {
        RETURN_IF_FAILED(GenerateAdapterList());
    }

    RETURN_IF_FALSE(index >= 0 && index < m_adapters.size(), result_code_t::out_of_bound);
    RETURN_IF_FALSE(ppAdapter, result_code_t::invalid_pointer);

    *ppAdapter = m_adapters[index].get();

    return result_code_t::succeeded;
}

///
///
///
void GraphicsDX12::PauseRendering()
{
    m_pause = true;
    ING_DebugWrite(L"Info: Rendering Paused\n");
}

///
///
///
void GraphicsDX12::ResumeRendering()
{
    m_pause = false;
    ING_DebugWrite(L"Info: Rendering Resumed\n");
}

///
///
///
result_code_t GraphicsDX12::OnWindowSizeChanged(const int width, const int height)
{
    if (m_spD3DDevice == nullptr)
    {
        ING_DebugWrite(L"Warning: %S(%d) - D3D12 Device is not created yet. Ignore window size changed event.\n", __FUNCTION__, __LINE__);
    }
    else if (width != m_windowWidth || height != m_windowHeight)
    {
        auto result = CreateWindowSizeDependentResources(width, height);

        if (result == result_code_t::rendering_device_removed)
        {
            RETURN_IF_FAILED(HandleDeviceLost());
        }
    }

    return result_code_t::succeeded;
}

///
///
///
result_code_t GraphicsDX12::GenerateAdapterList()
{
    ComPtr<IDXGIAdapter1> spAdapter;

    RETURN_IF_FALSE(m_spDXGIFactory != nullptr, result_code_t::not_initialized);

    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_spDXGIFactory->EnumAdapters1(adapterIndex, &spAdapter); adapterIndex++)
    {
        std::shared_ptr<IAdapter> adapter(new AdapterDX12(spAdapter.Get()));
        m_adapters.push_back(adapter);
    }

    return result_code_t::succeeded;
}

///
///
///
result_code_t GraphicsDX12::HandleDeviceLost()
{
    result_code_t result = result_code_t::rendering_device_removed;
    int retryCount = 0;
    const int maxRetry = 10;

    if (m_spD3DDevice)
    {
        HRESULT hrReason = m_spD3DDevice->GetDeviceRemovedReason();
        ING_DebugWrite(L"Warning: Rendering Device has been lost (0x%08x). Trying to recover\n", hrReason);
    }

    ReleaseWindowSizeDependentResources();
    ReleaseDeviceResources();
    ReleaseDeviceIndependentResources();

    while (result == result_code_t::rendering_device_removed && retryCount < maxRetry)
    {
        result = CreateDeviceIndependentResources();

        if (result == result_code_t::succeeded)
        {
            result = CreateDeviceResources();

            if (result == result_code_t::succeeded)
            {
                result = CreateWindowSizeDependentResources(m_windowWidth, m_windowHeight);
            }
        }

        if (result == result_code_t::rendering_device_removed)
        {
            retryCount++;
            ING_DebugWrite(L"Warning: Rendering Device not been restored. Retry %d\n", retryCount);
            Sleep(1000);
        }
        else
        {
            THROW_IF_FAILED(result);
        }
    }

    if (retryCount >= maxRetry)
    {
        ING_DebugWrite(L"Fatal: Rendering Device is not in healthy.\n");
        THROW_IF_FAILED(result_code_t::rendering_device_removed);
    }

    return result;
}

///
///
///
result_code_t GraphicsDX12::CreateDeviceIndependentResources()
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

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

///
///
///
void GraphicsDX12::ReleaseDeviceIndependentResources()
{
    m_spDXGIFactory.Reset();
    m_adapters.clear();
}

///
///
///
result_code_t GraphicsDX12::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    //
    // Create DX12 Device
    //
    {
        std::shared_ptr<AdapterDX12> spHwAdapter;

        // Select HW adapter
        for (auto &adapter : m_adapters)
        {
            if (adapter->IsHardware())
            {
                spHwAdapter = std::dynamic_pointer_cast<AdapterDX12>(adapter);
                ING_DebugWrite(L"Info: %s Adapter is selected\n", adapter->Description());
                break;
            }
        }
        RETURN_IF_FALSE(spHwAdapter, result_code_t::not_found);

        // Create the Direct3D 12 API device object
        hr = D3D12CreateDevice(
            spHwAdapter->DX12Adapter(),      // The hardware adapter.
            D3D_FEATURE_LEVEL_11_0,         // Minimum feature level this app can support.
            IID_PPV_ARGS(&m_spD3DDevice));  // Returns the Direct3D device created.

#if defined(_DEBUG)
        if (FAILED(hr) && hr != DXGI_ERROR_DEVICE_REMOVED && hr != DXGI_ERROR_DEVICE_RESET && hr != DXGI_ERROR_DRIVER_INTERNAL_ERROR)
        {
            // If the initialization fails, fall back to the WARP device.
            // For more information on WARP, see: 
            // https://go.microsoft.com/fwlink/?LinkId=286690

            ComPtr<IDXGIAdapter> spWarpAdapter;

            ING_DebugWrite(L"Failed to create D3D12 device with hardware adapter (0x%08x). Trying with WARP adapter.\n", hr);
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

    //
    // Create and initialize Frames.
    //
    for (auto &frame : m_frames)
    {
        frame.reset(new FrameDX12(m_spD3DDevice.Get()));
        RETURN_IF_FAILED(frame->Initialize());
    }

    //
    // Create synchronization objects.
    //
    {
        auto currentFrame = CurrentFrameDX12();
        GOTO_IF_HR_FAILED(m_spD3DDevice->CreateFence(currentFrame->FenceValue(), D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_spFence)), Cleanup);
        currentFrame->IncreaseFenceValue();

        // Create fence event
        m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        GOTO_IF_FALSE_HR(m_hFenceEvent != NULL, HRESULT_FROM_WIN32(GetLastError()), Cleanup);
    }

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

///
///
///
void GraphicsDX12::ReleaseDeviceResources()
{
    for (auto &frame : m_frames)
    {
        frame.reset();
    }

    if (m_hFenceEvent)
    {
        CloseHandle(m_hFenceEvent);
    }

    m_spFence.Reset();
    m_spCommandQueue.Reset();
    m_spD3DDevice.Reset();
}

///
///
///
result_code_t GraphicsDX12::CreateWindowSizeDependentResources(const int width, const int height)
{
    HRESULT hr = S_OK;

    ING_DebugWrite(L"%S(%d) width = %d, height = %d\n", __FUNCTION__, __LINE__, width, height);

    // Wait until all previous GPU work is complete.
    WaitForGPU();

    // Clear the previous window size specific content and update the tracked fence values.
    auto currentFenceValue = CurrentFrameDX12()->FenceValue();
    for (auto &frame : m_frames)
    {
        frame->ReleaseSizeDependentResources();
        auto frameDX12 = std::dynamic_pointer_cast<FrameDX12>(frame);
        frameDX12->SetFenceValue(currentFenceValue);
    }

    m_windowWidth = width;
    m_windowHeight = height;

    // Create Window size dependent resources.
    if (m_spSwapChain)
    {
        // If swap chain is already exist, try to resize.
        GOTO_IF_HR_FAILED(m_spSwapChain->ResizeBuffers(c_frameCount, m_windowWidth, m_windowHeight, m_backBufferFormat, 0), Cleanup);
    }
    else
    {
        // Create swap chain
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

        swapChainDesc.Width = m_windowWidth;                        // Match the size of the window.
        swapChainDesc.Height = m_windowHeight;
        swapChainDesc.Format = m_backBufferFormat;
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1;                         // Don't use multi-sampling.
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = c_frameCount;                   // Use triple-buffering to minimize latency.
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   // All Windows Universal apps must use _FLIP_ SwapEffects.
        swapChainDesc.Flags = 0;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        ComPtr<IDXGISwapChain1> spSwapChain;
        GOTO_IF_HR_FAILED(
            m_spDXGIFactory->CreateSwapChainForHwnd(
                m_spCommandQueue.Get(), // Swap chains need a reference to the command queue in DirectX 12.
                m_window,               // Window Handle
                &swapChainDesc,         // Swap chain descriptor
                nullptr,                // Full screen descriptor
                nullptr,                // Restrict
                &spSwapChain), Cleanup);

        GOTO_IF_HR_FAILED(spSwapChain.As(&m_spSwapChain), Cleanup);
    }

    // Set current buffer index
    m_currentFrame = m_spSwapChain->GetCurrentBackBufferIndex();

    // Update frames
    for (unsigned int i = 0; i < m_frames.size(); i++)
    {
        ComPtr<ID3D12Resource> spBackBuffer;

        auto& frame = m_frames[i];
        auto frameDX12 = std::dynamic_pointer_cast<FrameDX12>(frame);

        GOTO_IF_HR_FAILED(m_spSwapChain->GetBuffer(i, IID_PPV_ARGS(&spBackBuffer)), Cleanup);

        RETURN_IF_FAILED(frameDX12->SetBackBuffer(spBackBuffer.Get(), m_backBufferFormat));
        RETURN_IF_FAILED(frame->OnSizeChanged(width, height));
    }

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

///
///
///
void GraphicsDX12::ReleaseWindowSizeDependentResources()
{
    for (auto& frame : m_frames)
    {
        frame->ReleaseSizeDependentResources();
    }
}

///
///
///
void GraphicsDX12::WaitForGPU()
{
    HRESULT hr = S_OK;
    auto currentFrame = CurrentFrameDX12();

    // Schedule a Signal command in the queue.
    THROW_IF_HR_FAILED(m_spCommandQueue->Signal(m_spFence.Get(), currentFrame->FenceValue()));

    // Wait until the fence has been crossed.
    THROW_IF_HR_FAILED(m_spFence->SetEventOnCompletion(currentFrame->FenceValue(), m_hFenceEvent));
    WaitForSingleObjectEx(m_hFenceEvent, INFINITE, FALSE);

    // Increment the fence value for the current frame.
    currentFrame->IncreaseFenceValue();
}

///
///
///
void GraphicsDX12::MoveToNextFrame()
{
    HRESULT hr = S_OK;

    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = CurrentFrameDX12()->FenceValue();
    THROW_IF_HR_FAILED(m_spCommandQueue->Signal(m_spFence.Get(), currentFenceValue));

    // Advance the frame index.
    m_currentFrame = m_spSwapChain->GetCurrentBackBufferIndex();
    auto advancedFrame = CurrentFrameDX12();

    // Check to see if the next frame is ready to start.
    auto fenceCompletedValue = m_spFence->GetCompletedValue();

    if (fenceCompletedValue < advancedFrame->FenceValue())
    {
        THROW_IF_HR_FAILED(m_spFence->SetEventOnCompletion(advancedFrame->FenceValue(), m_hFenceEvent));
        WaitForSingleObjectEx(m_hFenceEvent, INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    advancedFrame->SetFenceValue(currentFenceValue + 1);
}
