// ING_Win32_Sample.cpp : Defines the entry point for the application.
//
#include "ING_Win32_Sample.h"
#include <memory>

// Global Variables:
const CHAR szTitle[] = "ING Project Win32 Sample";    // The title bar text
const CHAR szWindowClass[] = "INGSampleClass";        // the main window class name

#define DEFAULT_WINDOW_WIDTH    800
#define DEFAULT_WINDOW_HEIGHT   600

// Global singleton
static std::unique_ptr<INGWin32Sample> g_spINGSample;

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        if (g_spINGSample)
        {
            return g_spINGSample->MessageProc(hWnd, message, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

//
// INGWin32Sample
//
INGWin32Sample::INGWin32Sample(HINSTANCE hInstance) :
    m_hInstance(hInstance)
{
}

INGWin32Sample::~INGWin32Sample()
{
    Shutdown();
}

bool INGWin32Sample::Initialize()
{
    ING::result_code_t result = ING::result_succeeded;
    ING::INGEngine *pEngine = nullptr;
    int screenWidth = 0;
    int screenHeight = 0;

    InitialzeWindow(screenWidth, screenHeight);
    
    result = ING::CreateEngine(ING::renderer_dx_12, &pEngine);
    m_spINGEngine.reset(pEngine);

    return (result == ING::result_succeeded);
}

void INGWin32Sample::Shutdown()
{
    ShutdownWindow();
}

void INGWin32Sample::Run()
{
    MSG msg = {};
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
            // ToDo: Do my job
        }
    }
}

LRESULT CALLBACK INGWin32Sample::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void INGWin32Sample::InitialzeWindow(int &screenWidth, int &screenHeight)
{
    // Register Class
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = wcex.hIcon;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;

    RegisterClassEx(&wcex);

    int windowPositionX = 0;
    int windowPositionY = 0;

    if (m_fullScreen)
    {
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);

        DEVMODE dmScreenSettings = {};

        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth  = (DWORD)screenWidth;
        dmScreenSettings.dmPelsHeight = (DWORD)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        windowPositionX = 0;
        windowPositionY = 0;
    }
    else
    {
        screenWidth  = DEFAULT_WINDOW_WIDTH;
        screenHeight = DEFAULT_WINDOW_HEIGHT;
        windowPositionX = CW_USEDEFAULT;
        windowPositionY = CW_USEDEFAULT;
    }

    m_hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szWindowClass,
        szTitle,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | (m_fullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW),
        windowPositionX,
        windowPositionY,
        screenWidth,
        screenHeight,
        NULL,
        NULL,
        m_hInstance,
        NULL);

    ShowWindow(m_hWnd, SW_SHOW);
    SetForegroundWindow(m_hWnd);
    SetFocus(m_hWnd);
}

void INGWin32Sample::ShutdownWindow()
{
    if (m_hWnd)
    {
        if (m_fullScreen)
        {
            ChangeDisplaySettings(nullptr, 0);
        }

        DestroyWindow(m_hWnd);
        m_hWnd = NULL;

        UnregisterClass(szWindowClass, m_hInstance);
        m_hInstance = NULL;
    }
}

//
// Win Main
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    g_spINGSample = std::make_unique<INGWin32Sample>(hInstance);

    if (g_spINGSample->Initialize())
    {
        // Main message loop:
        g_spINGSample->Run();
    }

    g_spINGSample->Shutdown();

    return 0;
}