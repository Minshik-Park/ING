//----------------------------------------------------------------------------------
// IEngine.h : Definition the ING Engine interface class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>

namespace ING
{
    class ING_API IEngine
    {
    public:
        virtual ~IEngine();

        // Factory static methods
        static IEngine* Create(window_t wnd, Graphics::graphics_type_t graphicsType);

        // Public methods
        virtual result_code_t Initialize() = 0;

        virtual result_code_t Update() = 0;
        virtual result_code_t Render() = 0;

        virtual void PauseRendering() = 0;
        virtual void ResumeRendering() = 0;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) = 0;

    protected:
        IEngine();
    };
}
