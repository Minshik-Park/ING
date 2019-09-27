//----------------------------------------------------------------------------------
// Win32SampleApplication.cpp : Win32 Sample Application class definition.
//----------------------------------------------------------------------------------
#pragma once

#include "framework.h"
#include <INGAppFrameworkWin32.h>

//
// Win32 Sample Application
//
class Win32SampleApplication : public ING::AppFramework::Win32::Window
{
public:
	Win32SampleApplication(HINSTANCE hInstance);
	virtual ~Win32SampleApplication();

	virtual HRESULT PrepareMessageHandlers() override;
	virtual HRESULT DoRun() override;

private:
	HRESULT OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
