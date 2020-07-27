//----------------------------------------------------------------------------------
// Win32SampleApplication.cpp : Win32 Sample Application class implementation.
//----------------------------------------------------------------------------------

#include "Win32SampleApplication.h"
#include "resource.h"
#include <Exception.h>

///
/// Default Constructor.
///
Win32SampleApplication::Win32SampleApplication(HINSTANCE hInstance) :
	Window(hInstance)
{
}

///
/// Default Destructor.
///
Win32SampleApplication::~Win32SampleApplication()
{
}

///
/// Create window and initialize the Engine.
///
HRESULT Win32SampleApplication::Create(LPWSTR szTitle, int width, int height, bool fullscreen)
{
    HRESULT hr = S_OK;

    // Create Window
    GOTO_IF_HR_FAILED(Window::Create(szTitle, width, height, fullscreen), Cleanup);

    // Create ING Engine
    try
    {
        m_spEngine.reset(ING::IEngine::Create(m_hWnd, ING::Graphics::graphics_type_t::DX12));

        auto shEngine = ING::IEngine::CreateShared(m_hWnd, ING::Graphics::graphics_type_t::DX12);

        ING::result_code_t result = m_spEngine->Initialize();
        if (result != ING::result_code_t::succeeded)
        {
            hr = E_FAIL;
        }
    }
    catch (ING::ExceptionHr hrEx)
    {
        FatalAppExitA(0, hrEx.what());
    }
    catch (ING::Exception ex)
    {
        FatalAppExitA(0, ex.what());
    }
    catch (std::exception ex)
    {
        FatalAppExitA(0, ex.what());
    }

Cleanup:
    return hr;
}

///
/// Register Windows Message handlers.
///
void Win32SampleApplication::PrepareMessageHandlers()
{
	// Register WM_COMMAND event handler.
	AddMessageHandler(WM_COMMAND,
                      std::bind(&Win32SampleApplication::OnCommand,
                                this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	// Register WM_ENTERSIZEMOVE event handler.
	AddMessageHandler(WM_ENTERSIZEMOVE,
                      std::bind(&Win32SampleApplication::OnSize,
                                this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    // Register WM_EXITSIZEMOVE event handler.
	AddMessageHandler(WM_EXITSIZEMOVE,
                      std::bind(&Win32SampleApplication::OnSize,
                                this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    // Register WM_SIZE event handler to handle minimize and maximize events.
	AddMessageHandler(WM_SIZE,
                      std::bind(&Win32SampleApplication::OnSize,
                                this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    // Register WM_CLOSE event handler to handle minimize and maximize events.
	AddMessageHandler(WM_CLOSE,
                      std::bind(&Win32SampleApplication::OnClose,
                                this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

///
/// Do any custom action during message loop.
///
HRESULT Win32SampleApplication::DoRun()
{
    ING::result_code_t result = ING::result_code_t::succeeded;

    if (m_spEngine)
    {
        result = m_spEngine->Update();

        if (result == ING::result_code_t::succeeded)
        {
            result = m_spEngine->Render();
        }
    }

    return (result == ING::result_code_t::succeeded) ? S_OK : E_FAIL;
}

///
/// WM_COMMAND message handler.
///
HRESULT Win32SampleApplication::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	// Parse the menu selections:
	switch (wmId)
	{
	case IDM_EXIT:
		DestroyWindow(hWnd);
		return S_OK;
	default:
		return (HRESULT)DefWindowProc(hWnd, message, wParam, lParam);
	}

	return E_NOTIMPL;
}

///
/// Windows size messages handler.
///
HRESULT Win32SampleApplication::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_spEngine)
    {
        switch (message)
        {
        case WM_SIZE:
            switch (wParam)
            {
            case SIZE_MAXHIDE:
            case SIZE_MINIMIZED:
                // Stop rendering
                m_spEngine->OnSuspend();
                break;
            case SIZE_MAXSHOW:
                // Restart rendering
                m_spEngine->OnResume();
                break;
            case SIZE_MAXIMIZED:
            case SIZE_RESTORED:
                if (!m_windowSizing)
                {
                    // Window is restored from minimization or covered full screen window has been restored.
                    auto result = m_spEngine->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
                    if (result != ING::result_code_t::succeeded)
                    {
                        return E_FAIL;
                    }

                    // Window is shown. Make sure it's rendering.
                    m_spEngine->OnResume();
                }

                return S_OK;
            }
            break;
        case WM_ENTERSIZEMOVE:
            m_windowSizing = true;
            break;
        case WM_EXITSIZEMOVE:
            // Window resizing done. Get client rect and resize renderer.
            {
                RECT rcClient;

                m_windowSizing = false;
                GetClientRect(hWnd, &rcClient);

                auto result = m_spEngine->OnWindowSizeChanged(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
                if (result != ING::result_code_t::succeeded)
                {
                    return E_FAIL;
                }

                return S_OK;
            }
            break;
        }
    }

    return E_NOTIMPL;
}

///
/// Windows close messages handler. Release resources properly.
///
HRESULT Win32SampleApplication::OnClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_spEngine)
    {
        m_spEngine.reset();

        DestroyWindow(hWnd);
    }

    return E_NOTIMPL;
}