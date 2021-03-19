#pragma once
#include "Graphics.h"
#include <functional>

namespace ING
{
    enum class WindowState
    {
        Unknown,
        Shown,
        Hidden,
        Exit,
        Max
    };

    class GameEngine
    {
    public:
        GameEngine();
        virtual ~GameEngine();

        /// <summary>
        /// Initialize Game Engine
        /// </summary>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Initialize() = 0;

        /// <summary>
        /// Initialize Game Window to start rendering
        /// </summary>
        /// <param name="width">The width of window in pixel</param>
        /// <param name="height">The height of window in pixel</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode InitializeWindow(unsigned int width, unsigned int height) = 0;

        /// <summary>
        /// Initialize Game Window to start rendering
        /// </summary>
        /// <param name="logicalWidth">The logical width of screen</param>
        /// <param name="logicalHeight">The logical height of screen</param>
        /// <param name="dpi">The dots per inch of screen</param>
        /// <returns>Error code if failed</returns>
        ErrorCode InitializeWindow(float logicalWidth, float logicalHeight, float dpi)
        {
            return InitializeWindow(ConvertLogicalToPixels(logicalWidth, dpi), ConvertLogicalToPixels(logicalHeight, dpi));
        }

        /// <summary>
        /// Main Game loop
        /// </summary>
        /// <param name="systemEventHandler">Custom system event handler function to be called in game loop</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Run(std::function<WindowState()> &&systemEventHandler) = 0;

        virtual ErrorCode OnUpdate() = 0;
        virtual ErrorCode OnRender() = 0;
        virtual ErrorCode OnWindowSizeUpdated(float logicalWidth, float logicalHeight, float dpi)
        {
            return OnWindowSizeUpdated(ConvertLogicalToPixels(logicalWidth, dpi), ConvertLogicalToPixels(logicalHeight, dpi));
        }
        virtual ErrorCode OnWindowSizeUpdated(unsigned int width, unsigned int height) = 0;

    protected:
        inline unsigned int ConvertLogicalToPixels(float logicalSize, float dpi)
        {
            static const float dipsPerInch = 96.0f;
            return static_cast<unsigned int>(floorf(logicalSize * dpi / dipsPerInch + 0.5f)); // Round to nearest integer.
        }

    protected:
        bool m_exit = false;
        bool m_visible = false;

        std::shared_ptr<Graphics> m_spGraphics;
    };
}