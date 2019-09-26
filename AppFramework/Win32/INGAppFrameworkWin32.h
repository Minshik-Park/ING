//----------------------------------------------------------------------------------
// INGAppFrameworkWin32.h : Wrapper classes for Win32 APIs.
//----------------------------------------------------------------------------------
#pragma once

#include <Windows.h>
#include <functional>
#include <map>
#include <string>

namespace ING {
namespace AppFramework {
namespace Win32 {

	///-----------------------------------------------------------------------------
	// Window Message handler callback type
	///-----------------------------------------------------------------------------
	typedef std::function<HRESULT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)> WndCallbackFunc;

	///-----------------------------------------------------------------------------
	/// Wrapper class of Win32 Window
    ///-----------------------------------------------------------------------------
	class Window
	{
	public:
		Window(HINSTANCE hInstance);
		virtual ~Window();

		virtual HRESULT RegisterWindowClass(
			LPWSTR szClass,
			LPWSTR szMenuName,
			UINT windowStyle,
			HICON hIcon,
			HICON hIconSm,
			HCURSOR hCursor,
			HBRUSH hBackground);
		virtual HRESULT Create(LPWSTR szTitle, int width, int height, bool fullscreen);
		virtual HRESULT PrepareMessageHandlers() = 0;
		virtual HRESULT Run(bool fPeek, HACCEL hAccelTable);
		virtual void Shutdown();

		// Window Control
		void Show();
		void Hide();
		void SetForeground();
		void SetFocus();

		// Window Properties
		bool IsFullScreen() { return m_fullscreen; }
		int Width() { return m_width; }
		int Height() { return m_height; }
		int ClientWidth() { return m_clientWidth; }
		int ClientHeight() { return m_clientHeight; }

	protected:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		virtual HRESULT DoRun() = 0;
		virtual HRESULT AddMessageHandler(UINT message, WndCallbackFunc callback);
		virtual HRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		HINSTANCE m_hInstance = NULL;
		HWND m_hWnd = NULL;
		HWND m_hParent = NULL;
		std::wstring m_class;

		// Window properties
		bool m_fullscreen = false;
		int m_width = 0;
		int m_height = 0;
		int m_clientWidth = 0;
		int m_clientHeight = 0;
		DWORD m_dwWindowStyle = WS_OVERLAPPEDWINDOW;

		// Event handler map
		std::map<UINT, WndCallbackFunc> m_mapCallback;

		// Static map of window handle and Window instance
		static std::map<HWND, Window*> s_mapThis;
	};

	///-----------------------------------------------------------------------------
	/// Wrapper class of Modal Dialog
	///-----------------------------------------------------------------------------

}}}
