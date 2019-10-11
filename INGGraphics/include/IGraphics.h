//----------------------------------------------------------------------------------
// Graphics.h : Defines the ING Graphics interface class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <IAdapter.h>
#include <IFrame.h>

namespace ING {
namespace Graphics {

    ///
    /// Type of Graphics implementation
    ///
    enum class GraphicsType
    {
        Unknown = 0,
        DX12
    };

    static const UINT c_frameCount = 3; // Use triple buffering.

    ///
    /// Graphics class
    ///
    class ING_API IGraphics
    {
    public:
        virtual ~IGraphics();

        // Factory
        static IGraphics *Create(GraphicsType type);

        // Public methods
        virtual result_code_t Initialize(window_t wnd) = 0;
#ifdef _WIN32
        virtual result_code_t Initialize(core_window_t wnd) = 0;
#endif
        virtual result_code_t GetAdapterAt(const int index, IAdapter **ppAdapter) = 0;

        virtual void PauseRendering() = 0;
        virtual void ResumeRendering() = 0;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) = 0;

    protected:
        IGraphics();

    protected:
        window_t m_window = NULL;
        int m_windowWidth = 0;
        int m_windowHeight = 0;
    };
}}
