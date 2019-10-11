//----------------------------------------------------------------------------------
// Win32SampleApplication.cpp : Win32 Sample Application class definition.
//----------------------------------------------------------------------------------
#pragma once

#include "framework.h"
#include <AppFrameworkWin32.h>
#include <IEngine.h>
#include <memory>

//
// Win32 Sample Application
//
class Win32SampleApplication : public ING::AppFramework::Win32::Window
{
public:
	Win32SampleApplication(HINSTANCE hInstance);
	virtual ~Win32SampleApplication();

    virtual HRESULT Create(LPWSTR szTitle, int width, int height, bool fullscreen) override;

	virtual HRESULT PrepareMessageHandlers() override;
	virtual HRESULT DoRun() override;

private:
	HRESULT OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    HRESULT OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    std::unique_ptr<ING::IEngine> m_spEngine;
    bool m_windowSizing = false;
};
