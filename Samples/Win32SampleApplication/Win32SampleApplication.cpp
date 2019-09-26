//----------------------------------------------------------------------------------
// Win32SampleApplication.cpp : Win32 Sample Application class implementation.
//----------------------------------------------------------------------------------

#include "Win32SampleApplication.h"
#include "resource.h"

Win32SampleApplication::Win32SampleApplication(HINSTANCE hInstance) :
	Window(hInstance)
{
}

Win32SampleApplication::~Win32SampleApplication()
{
}

HRESULT Win32SampleApplication::PrepareMessageHandlers()
{
	HRESULT hr = S_OK;

	hr = AddMessageHandler(WM_COMMAND, std::bind(&Win32SampleApplication::OnCommand, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	if (FAILED(hr))
	{
		goto Cleanup;
	}

Cleanup:
	return hr;
}

HRESULT Win32SampleApplication::DoRun()
{
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
