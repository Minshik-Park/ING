#include "pch.h"
#include "GameEngineWinRT.h"
#include "GraphicsDX12.h"

using namespace winrt::Windows::UI::Core;
using namespace ING;

/// <summary>
/// Constructor
/// </summary>
GameEngineWinRT::GameEngineWinRT()
{
}

/// <summary>
/// Destructor
/// </summary>
GameEngineWinRT::~GameEngineWinRT()
{
}

/// <summary>
/// Initialize Game Engine
/// </summary>
/// <returns>Error code if failed</returns>
ErrorCode GameEngineWinRT::Initialize()
{
    ResultCode rc(__FUNCTION__);

    //m_spGraphics = std::make_shared<GraphicsDX12>();
    //IFFALSE_EXIT(m_spGraphics != nullptr, ErrorCode::EOutOfMemory);

    m_spGraphics.reset(Graphics::CreateInstance(GraphicsApi::DX12));
    IFFALSE_EXIT(m_spGraphics != nullptr, ErrorCode::EOutOfMemory);

    return rc.Result();
}

/// <summary>
/// Initialize Game Window to start rendering
/// </summary>
/// <param name="width">The width of window in pixel</param>
/// <param name="height">The height of window in pixel</param>
/// <returns>Error code if failed</returns>
ErrorCode GameEngineWinRT::InitializeWindow(unsigned int width, unsigned int height)
{
    ResultCode rc(__FUNCTION__);
    IFFAILED_EXIT(m_spGraphics->Initialize(width, height));

    return rc.Result();
}

ErrorCode GameEngineWinRT::Run(std::function<WindowState()>&& systemEventHandler)
{
    ResultCode rc(__FUNCTION__);

    while (!m_exit)
    {
        auto systemWindowState = systemEventHandler();

        switch (systemWindowState)
        {
        case WindowState::Exit:
            m_exit = true;
            m_visible = false;
            break;
        default:
            break;
        }

        // OnUpdate();

        if (m_visible)
        {
            // OnRender();
        }
    }

    return rc.Result();
}

ErrorCode GameEngineWinRT::OnUpdate()
{
    ResultCode rc(__FUNCTION__);

    return rc.Result();
}

ErrorCode GameEngineWinRT::OnRender()
{
    ResultCode rc(__FUNCTION__);

    return rc.Result();
}

ErrorCode GameEngineWinRT::OnWindowSizeUpdated(unsigned int width, unsigned int height)
{
    ResultCode rc(__FUNCTION__);

    if (m_spGraphics)
    {
        m_spGraphics->OnWindowSizeUpdated(width, height);
    }

    return rc.Result();
}
