#include "pch.h"
#include <GameEngineWinRT.h>

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;
using namespace Windows::System;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
    //CompositionTarget m_target{ nullptr };
    //VisualCollection m_visuals{ nullptr };
    //Visual m_selected{ nullptr };
    float2 m_offset{};

    /// <summary>
    /// Create application view and return.
    /// </summary>
    /// <returns>Implementation of IFrameworkView.</returns>
    IFrameworkView CreateView()
    {
        FunctionTrace();

        return *this;
    }

    /// <summary>
    /// Initialize Application.
    /// Called after CreateView().
    /// </summary>
    void Initialize(CoreApplicationView const &)
    {
        FunctionTrace();

        m_gameEngine = std::make_unique<ING::GameEngineWinRT>();
        m_gameEngine->Initialize();
    }

    /// <summary>
    /// Initialize resources binded to window.
    /// Called after Initialize().
    /// </summary>
    /// <param name="window">Reference of CoreWindow</param>
    void SetWindow(CoreWindow const& window)
    {
        FunctionTrace();

        m_dpi = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView().LogicalDpi();
        auto ClientRect = window.Bounds();
        m_gameEngine->InitializeWindow(ClientRect.Width, ClientRect.Height, m_dpi);

        m_currentWindowSize.Width = ClientRect.Width;
        m_currentWindowSize.Height = ClientRect.Height;

        //Compositor compositor;
        //ContainerVisual root = compositor.CreateContainerVisual();
        //m_target = compositor.CreateTargetForCurrentView();
        //m_target.Root(root);
        //m_visuals = root.Children();

        // Register Event handlers
        window.PointerPressed({ this, &App::OnPointerPressed });
        window.PointerMoved({ this, &App::OnPointerMoved });
        window.PointerReleased([&](auto && ...)
        {
            // m_selected = nullptr;
        });
        window.Closed([&](auto && ...)
        {
            FunctionTrace();
            m_windowClosed = true;
        });
        window.KeyDown([&](IInspectable const&, IKeyEventArgs const& args)
        {
            if (args.VirtualKey() == VirtualKey::Escape)
            {
                m_windowClosed = true;
            }
            FunctionTrace();
        });
        window.ResizeCompleted([this](auto && ...)
        {
            ING::Debug::DebugPrintf("Windows Resize completed [%f, %f] - %f\n", m_currentWindowSize.Width, m_currentWindowSize.Height, m_dpi);
            m_gameEngine->OnWindowSizeUpdated(m_currentWindowSize.Width, m_currentWindowSize.Height, m_dpi);
        });
        window.SizeChanged([this](IInspectable const&, IWindowSizeChangedEventArgs const& args)
        {
            ING::Debug::DebugPrintf("Window Size changed [%f, %f] - %f\n", args.Size().Width, args.Size().Height, m_dpi);
            m_currentWindowSize = args.Size();
        });
    }

    /// <summary>
    /// Place to load resources after window related resources are initialized.
    /// Called after SetWindow().
    /// </summary>
    void Load(hstring const&)
    {
        FunctionTrace();
    }

    void Uninitialize()
    {
        FunctionTrace();
    }

    void Run()
    {
        ING::ResultCode rc(__FUNCTION__);

        // Activate window
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();

        // Run main game loop
        rc = m_gameEngine->Run([&]()->ING::WindowState
        {
            window.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            return m_windowClosed ? ING::WindowState::Exit : ING::WindowState::Shown;
        });
    }

    void OnPointerPressed(IInspectable const &, PointerEventArgs const & args)
    {
        FunctionTrace();
        float2 const point = args.CurrentPoint().Position();

        //for (Visual visual : m_visuals)
        //{
        //    float3 const offset = visual.Offset();
        //    float2 const size = visual.Size();

        //    if (point.x >= offset.x &&
        //        point.x < offset.x + size.x &&
        //        point.y >= offset.y &&
        //        point.y < offset.y + size.y)
        //    {
        //        m_selected = visual;
        //        m_offset.x = offset.x - point.x;
        //        m_offset.y = offset.y - point.y;
        //    }
        //}

        //if (m_selected)
        //{
        //    m_visuals.Remove(m_selected);
        //    m_visuals.InsertAtTop(m_selected);
        //}
        //else
        //{
        //    AddVisual(point);
        //}
    }

    void OnPointerMoved(IInspectable const &, PointerEventArgs const & args)
    {
        float2 const point = args.CurrentPoint().Position();
        //if (m_selected)
        //{
        //    float2 const point = args.CurrentPoint().Position();

        //    m_selected.Offset(
        //    {
        //        point.x + m_offset.x,
        //        point.y + m_offset.y,
        //        0.0f
        //    });
        //}
    }

    void AddVisual(float2 const /* point */)
    {
        FunctionTrace();
        //Compositor compositor = m_visuals.Compositor();
        //SpriteVisual visual = compositor.CreateSpriteVisual();

        //static Color colors[] =
        //{
        //    { 0xDC, 0x5B, 0x9B, 0xD5 },
        //    { 0xDC, 0xED, 0x7D, 0x31 },
        //    { 0xDC, 0x70, 0xAD, 0x47 },
        //    { 0xDC, 0xFF, 0xC0, 0x00 }
        //};

        //static unsigned last = 0;
        //unsigned const next = ++last % _countof(colors);
        //visual.Brush(compositor.CreateColorBrush(colors[next]));

        //float const BlockSize = 100.0f;

        //visual.Size(
        //{
        //    BlockSize,
        //    BlockSize
        //});

        //visual.Offset(
        //{
        //    point.x - BlockSize / 2.0f,
        //    point.y - BlockSize / 2.0f,
        //    0.0f,
        //});

        //m_visuals.InsertAtTop(visual);

        //m_selected = visual;
        //m_offset.x = -BlockSize / 2.0f;
        //m_offset.y = -BlockSize / 2.0f;
    }

private:
    bool m_windowClosed = false;
    Size m_currentWindowSize;
    float m_dpi;
    std::unique_ptr<ING::GameEngine> m_gameEngine;
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    FunctionTrace();
    CoreApplication::Run(make<App>());
}
