//----------------------------------------------------------------------------------
// Main.cpp : Defines the entry point for the application.
//----------------------------------------------------------------------------------

#include "resource.h"
#include "Win32SampleApplication.h"
#include <memory>

#include <IGraphics.h>

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HRESULT hr = S_OK;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32SAMPLEAPPLICATION, szWindowClass, MAX_LOADSTRING);

	// Create application window instance
	std::unique_ptr<Win32SampleApplication> appWindow = std::make_unique<Win32SampleApplication>(hInstance);

	// Register window class
	GOTO_IF_HR_FAILED(appWindow->RegisterWindowClass(
			szWindowClass,
			MAKEINTRESOURCEW(IDC_WIN32SAMPLEAPPLICATION),
			CS_HREDRAW | CS_VREDRAW,
			LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32SAMPLEAPPLICATION)),
			LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)),
			LoadCursor(nullptr, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW + 1)), Cleanup);

	// Register Windows message handlers
	GOTO_IF_HR_FAILED(appWindow->PrepareMessageHandlers(), Cleanup);

	// Create Window
	GOTO_IF_HR_FAILED(appWindow->Create(szTitle, 800, 600, false), Cleanup);

	// Show Window
	appWindow->Show();
	appWindow->SetForeground();
	appWindow->SetFocus();

	// Run main message loop.
	GOTO_IF_HR_FAILED(appWindow->Run(true, LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32SAMPLEAPPLICATION))), Cleanup);

Cleanup:
	return (int)hr;
}
