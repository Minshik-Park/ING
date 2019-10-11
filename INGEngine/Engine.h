//----------------------------------------------------------------------------------
// Engine.h : Definition the ING Engine class.
//----------------------------------------------------------------------------------
#pragma once
#include <IEngine.h>
#include <IGraphics.h>
#include <memory>

namespace ING
{
    class Engine : public IEngine
    {
    public:
        Engine(window_t wnd, Graphics::GraphicsType graphicsType);
        virtual ~Engine();

        // Public methods
        virtual result_code_t Initialize() override;

        // System event handlers
        virtual result_code_t OnWindowSizeChanged(const int width, const int height) override;

    private:
        window_t m_window = NULL;
        Graphics::GraphicsType m_graphicsType = Graphics::GraphicsType::Unknown;

        std::shared_ptr<Graphics::IGraphics> m_spGraphics;
    };
}