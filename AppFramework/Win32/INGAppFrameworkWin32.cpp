//----------------------------------------------------------------------------------
// INGAppFrameworkWin32.cpp : Defines the functions for the static library.
//----------------------------------------------------------------------------------

#include "INGAppFrameworkWin32.h"
#include <assert.h>

using namespace ING::AppFramework::Win32;

// Map of window handle and Window pointer to find window instance in message handler.
std::map<HWND, Window*> Window::s_mapThis;

///
/// Constructor.
///
Window::Window(HINSTANCE hInstance) :
	m_hInstance(hInstance)
{
	// Initialize COM
	if (FAILED(CoInitialize(nullptr)))
	{
		assert(!L"CoInitialize failed");
	}
}

///
/// Destructor.
///
Window::~Window()
{
	Shutdown();

	// Release COM
	CoUninitialize();
}

///
/// Register Window class.
///
HRESULT Window::RegisterWindowClass(
	LPWSTR szClass,
	LPWSTR szMenuName,
	UINT windowStyle,
	HICON hIcon,
	HICON hIconSm,
	HCURSOR hCursor,
	HBRUSH hBackground)
{
	HRESULT hr = S_OK;
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = windowStyle;
	wcex.lpfnWndProc = Window::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = hIcon;
	wcex.hCursor = hCursor;
	wcex.hbrBackground = hBackground;
	wcex.lpszMenuName = szMenuName;
	wcex.lpszClassName = szClass;
	wcex.hIconSm = hIconSm;

	if (RegisterClassExW(&wcex) == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		// Succeeded to register class. Cache class name.
		m_class = szClass;
	}

	return hr;
}

///
/// Create Window
///
HRESULT Window::Create(LPWSTR szTitle, int width, int height, bool fullscreen)
{
	m_fullscreen = fullscreen;
	int windowPositionX = 0;
	int windowPositionY = 0;

	if (m_fullscreen)
	{
		m_width = GetSystemMetrics(SM_CXSCREEN);
		m_height = GetSystemMetrics(SM_CYSCREEN);

		DEVMODE dmScreenSettings = {};

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (DWORD)width;
		dmScreenSettings.dmPelsHeight = (DWORD)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		windowPositionX = 0;
		windowPositionY = 0;
	}
	else
	{
		m_width = width;
		m_height = height;
		windowPositionX = CW_USEDEFAULT;
		windowPositionY = CW_USEDEFAULT;
	}

	m_hWnd = CreateWindowW(
		m_class.c_str(),        // Class Name
		szTitle,                // Window Title
		m_dwWindowStyle,		// Window Style
		windowPositionX,        // x
		windowPositionY,        // y
		m_width,				// width
		m_height,				// height
		m_hParent,              // hWndParent
		NULL,                   // hMenu
		m_hInstance,            // hInstance
		this);                  // lpParam

	if (!m_hWnd)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// Get the client rect.
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	m_clientWidth = clientRect.right - clientRect.left;
	m_clientHeight = clientRect.bottom - clientRect.top;

	return S_OK;
}

///
/// Run event loop
///
HRESULT Window::Run(bool fPeek, HACCEL hAccelTable)
{
	MSG msg = {};

	if (fPeek)
	{
		bool exit = false;

		// Main message loop
		while (!exit)
		{
			// Hnadle Windows system messages
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
			{
				exit = true;
			}
			else
			{
				if (FAILED(DoRun()))
				{
					OutputDebugString(L"DoRun() Failed\n");
				}
			}
		}
	}
	else
	{
		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return (HRESULT)msg.wParam;
}

///
/// Shutdown Window instance and close window.
///
void Window::Shutdown()
{
	if (m_hWnd)
	{
		if (m_fullscreen)
		{
			ChangeDisplaySettings(nullptr, 0);
		}

		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	if (!m_class.empty())
	{
		UnregisterClass(m_class.c_str(), m_hInstance);
		m_hInstance = NULL;
	}
}

///
/// Add Windows Message Handler.
///
HRESULT Window::AddMessageHandler(UINT message, WndCallbackFunc callback)
{
	m_mapCallback[message] = std::move(callback);

	return S_OK;
}

///
/// Handle Windows Messages.
///
HRESULT Window::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Handle default behaviour
	switch (message)
	{
	case WM_SIZE:
		// Update client area size.
		m_clientWidth  = LOWORD(lParam);
		m_clientHeight = HIWORD(lParam);
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	auto callback = m_mapCallback.find(message);

	if (callback != m_mapCallback.end())
	{
		return callback->second(hWnd, message, wParam, lParam);
	}

	return E_NOTIMPL;
}

///
/// Show window.
///
void Window::Show()
{
	::ShowWindow(m_hWnd, SW_SHOW);
}

///
/// Hide window.
///
void Window::Hide()
{
	::ShowWindow(m_hWnd, SW_HIDE);
}

///
/// Set this window as foreground window.
///
void Window::SetForeground()
{
	::SetForegroundWindow(m_hWnd);
}

///
/// Give focus to this window.
///
void Window::SetFocus()
{
	::SetFocus(m_hWnd);
}

///
/// Windows event procedure.
///
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT result = E_NOTIMPL;
	Window* pThis = nullptr;

	switch (message)
	{
	case WM_NCCREATE:
	{
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);;
		if (nullptr == pCreateStruct)
		{
			return FALSE;
		}

		pThis = reinterpret_cast<Window*>(pCreateStruct->lpCreateParams);
		if (nullptr == pThis)
		{
			return FALSE;
		}

		s_mapThis[hWnd] = pThis;
	}
	break;
	case WM_DESTROY:
	{
		pThis = s_mapThis[hWnd];

		// Windows is destroyed. Erase map entity.
		s_mapThis.erase(hWnd);
	}
	break;
	default:
		pThis = s_mapThis[hWnd];
	}

	if (pThis)
	{
		// Invoke Window message handler.
		result = pThis->HandleMessage(hWnd, message, wParam, lParam);
	}

	return result == E_NOTIMPL ? ::DefWindowProc(hWnd, message, wParam, lParam) : (LRESULT)result;
}