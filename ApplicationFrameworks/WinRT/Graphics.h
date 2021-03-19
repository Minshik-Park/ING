#pragma once

namespace ING
{
    enum class GraphicsApi
    {
        Unknown = 0,
        DX11,
        DX12,
        OpenGL
    };

    class Graphics
    {
    public:
        /// <summary>
        /// Factory method to create Graphics object.
        /// </summary>
        /// <param name="api">Desired graphics API to use</param>
        /// <returns></returns>
        static Graphics* CreateInstance(GraphicsApi api);

        /// <summary>
        /// Initialize Graphics Device
        /// </summary>
        /// <param name="width">Screen width in pixel</param>
        /// <param name="height">Screen height in pixel</param>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Initialize(const unsigned int width, const unsigned int height) = 0;

        /// <summary>
        /// Destroy and release any graphic resource
        /// </summary>
        /// <returns>Error code if failed</returns>
        virtual ErrorCode Destroy() = 0;

        /// <summary>
        /// Window size updated event handler
        /// </summary>
        /// <param name="width">Window width in pixel</param>
        /// <param name="height">Window height in pixel</param>
        /// <returns></returns>
        virtual ErrorCode OnWindowSizeUpdated(const unsigned int width, const unsigned int height) = 0;

    protected:
        static const unsigned int c_frameCount = 2; // Double buffering.

        unsigned int m_windowWidth = 0;
        unsigned int m_windowHeight = 0;
    };
}