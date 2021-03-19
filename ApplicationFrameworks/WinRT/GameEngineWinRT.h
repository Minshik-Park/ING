#pragma once
#include <GameEngine.h>

namespace ING
{
    class GameEngineWinRT : public GameEngine
    {
    public:
        GameEngineWinRT();
        virtual ~GameEngineWinRT();

        /// <summary>
        /// Initialize Game Engine
        /// </summary>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Initialize() override;

        /// <summary>
        /// Initialize Game Window to start rendering
        /// </summary>
        /// <param name="width">The width of window in pixel</param>
        /// <param name="height">The height of window in pixel</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode InitializeWindow(unsigned int width, unsigned int height) override;

        /// <summary>
        /// Main Game loop
        /// </summary>
        /// <param name="systemEventHandler">Custom system event handler function to be called in game loop</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Run(std::function<WindowState()>&& systemEventHandler) override;

        virtual ErrorCode OnUpdate() override;
        virtual ErrorCode OnRender() override;
        virtual ErrorCode OnWindowSizeUpdated(unsigned int width, unsigned int height) override;
    };
}