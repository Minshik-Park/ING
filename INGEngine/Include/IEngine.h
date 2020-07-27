//----------------------------------------------------------------------------------
// IEngine.h : Definition the ING Engine interface class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <memory>

namespace ING
{
    class ING_API IEngine
    {
    public:
        virtual ~IEngine();

        // Factory static methods
        static IEngine* Create(window_t wnd, Graphics::graphics_type_t graphicsType);

        static void DestoryEngine(void* pEngine)
        {
            if (pEngine)
            {
                delete pEngine;
            }
        }

        static std::shared_ptr<IEngine> CreateShared(window_t wnd, Graphics::graphics_type_t graphicsType);

        // Public methods
        virtual result_code_t Initialize() = 0;

        virtual result_code_t Update() = 0;
        virtual result_code_t Render() = 0;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) = 0;
        virtual result_code_t OnSuspend() = 0;
        virtual result_code_t OnResume() = 0;

    protected:
        IEngine();
    };
}
