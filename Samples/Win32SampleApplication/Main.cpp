//----------------------------------------------------------------------------------
// Main.cpp : Defines the entry point for the application.
//----------------------------------------------------------------------------------

#include "resource.h"
#include "Win32SampleApplication.h"
#include <memory>

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
	hr = appWindow->RegisterWindowClass(
		szWindowClass,
		MAKEINTRESOURCEW(IDC_WIN32SAMPLEAPPLICATION),
		CS_HREDRAW | CS_VREDRAW,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32SAMPLEAPPLICATION)),
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)),
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1));
	if (FAILED(hr))
	{
		printf("Failed to Register application window class (0x%08x)\n", hr);
		goto Cleanup;
	}

	// Register Windows message handlers
	hr = appWindow->PrepareMessageHandlers();
	if (FAILED(hr))
	{
		printf("Failed to add message handlers (0x%08x)\n", hr);
		goto Cleanup;
	}

	// Create Window
	hr = appWindow->Create(szTitle, 800, 600, false);
	if (FAILED(hr))
	{
		printf("Failed to Create application window (0x%08x)\n", hr);
		goto Cleanup;
	}

	appWindow->Show();
	appWindow->SetForeground();
	appWindow->SetFocus();

	hr = appWindow->Run(true, LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32SAMPLEAPPLICATION)));
	if (FAILED(hr))
	{
		printf("Failed to Run application window (0x%08x)\n", hr);
		goto Cleanup;
	}

Cleanup:
	return (int)hr;
}
