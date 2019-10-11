//----------------------------------------------------------------------------------
// IEngine.h : Definition the ING Engine interface class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <IGraphics.h>

namespace ING
{
    class ING_API IEngine
    {
    public:
        virtual ~IEngine();

        // Factory static methods
        static IEngine* Create(window_t wnd, Graphics::GraphicsType graphicsType);

        // Public methods
        virtual result_code_t Initialize() = 0;

        virtual void PauseRendering() = 0;
        virtual void ResumeRendering() = 0;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) = 0;

    protected:
        IEngine();
    };
}
