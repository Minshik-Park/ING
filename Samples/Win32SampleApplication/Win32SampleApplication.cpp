//----------------------------------------------------------------------------------
// Win32SampleApplication.cpp : Win32 Sample Application class implementation.
//----------------------------------------------------------------------------------

#include "Win32SampleApplication.h"
#include "resource.h"
#include <Exception.h>

Win32SampleApplication::Win32SampleApplication(HINSTANCE hInstance) :
	Window(hInstance)
{
}

Win32SampleApplication::~Win32SampleApplication()
{
}

HRESULT Win32SampleApplication::Create(LPWSTR szTitle, int width, int height, bool fullscreen)
{
    HRESULT hr = S_OK;

    // Create Window
    GOTO_IF_HR_FAILED(Window::Create(szTitle, width, height, fullscreen), Cleanup);

    // Create ING Engine
    try
    {
        m_spEngine.reset(ING::IEngine::Create(m_hWnd, ING::Graphics::GraphicsType::DX12));

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

Cleanup:
    return hr;
}


HRESULT Win32SampleApplication::PrepareMessageHandlers()
{
	HRESULT hr = S_OK;

	// Register WM_COMMAND event handler.
	GOTO_IF_HR_FAILED(AddMessageHandler(WM_COMMAND,
                                        std::bind(&Win32SampleApplication::OnCommand,
		                                this,
		                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)), Cleanup);

	// Register WM_SIZE event handler.
	GOTO_IF_HR_FAILED(AddMessageHandler(WM_SIZE,
                                        std::bind(&Win32SampleApplication::OnSize,
		                                this,
		                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)), Cleanup);
Cleanup:
	return hr;
}

HRESULT Win32SampleApplication::DoRun()
{
	// ToDo: Add code to custom action for loop.
	return S_OK;
}

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

HRESULT Win32SampleApplication::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_spEngine)
    {
        auto result = m_spEngine->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        if (result != ING::result_code_t::succeeded)
        {
            return E_FAIL;
        }

        return S_OK;
    }

    return E_NOTIMPL;
}
