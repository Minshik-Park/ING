#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <memory>

#include <ING/INGApi.h>

class INGWin32Sample
{
public:
    INGWin32Sample(HINSTANCE hInstance);
    virtual ~INGWin32Sample();

    bool Initialize();
    void Shutdown();
    void Run();

    LRESULT CALLBACK MessageProc(HWND, UINT, WPARAM, LPARAM);

private:
    void InitialzeWindow(int &screenWidth, int &screenHeight);
    void ShutdownWindow();

    HINSTANCE m_hInstance = NULL;
    HWND m_hWnd = NULL;
    bool m_fullScreen = false;

    std::unique_ptr<ING::INGEngine> m_spINGEngine;
};
