//----------------------------------------------------------------------------------
// Engine.h : Definition the ING Engine class.
//----------------------------------------------------------------------------------
#pragma once
#include <IEngine.h>
#include "Graphics/IGraphics.h"
#include <memory>

namespace ING
{
    class Engine : public IEngine
    {
    public:
        Engine(window_t wnd, Graphics::graphics_type_t graphicsType);
        virtual ~Engine();

        // Public methods
        virtual result_code_t Initialize() override;

        virtual result_code_t Update() override;
        virtual result_code_t Render() override;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) override;
        virtual result_code_t OnSuspend() override;
        virtual result_code_t OnResume() override;

    private:
        window_t m_window = NULL;
        Graphics::graphics_type_t m_graphicsType = Graphics::graphics_type_t::Unknown;

        std::shared_ptr<Graphics::IGraphics> m_spGraphics;
    };
}